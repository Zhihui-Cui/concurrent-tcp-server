#include <cstdio>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

// 请求长度上限，包含结尾的换行符。
constexpr std::size_t MAX_REQUEST_BYTES = 1024;

int main() {
    std::cout << "P1: single-request TCP echo server" << std::endl;

    // 1. 创建监听 socket。
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (listen_fd == -1) {
        perror("socket");
        return 1;
    }

    std::cout << "listen_fd = " << listen_fd << std::endl;

    // 2. 绑定本地地址。
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_port = htons(8080);
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    int result = bind(
        listen_fd,
        reinterpret_cast<const sockaddr*>(&address),
        sizeof(address)
    );

    if (result == -1) {
        perror("bind");
        close(listen_fd);
        return 1;
    }

    std::cout << "Bound to 127.0.0.1:8080" << std::endl;

    // 3. 开始监听。
    int listen_result = listen(listen_fd, 8);

    if (listen_result == -1) {
        perror("listen");
        close(listen_fd);
        return 1;
    }

    std::cout << "Listening on 127.0.0.1:8080" << std::endl;

    // 4. 等待并接受一个客户端连接。
    int client_fd = accept(listen_fd, nullptr, nullptr);

    if (client_fd == -1) {
        perror("accept");
        close(listen_fd);
        return 1;
    }

    std::cout << "Accepted client, fd = " << client_fd << std::endl;

    // 5. 接收请求，直到找到第一个换行符。
    char buffer[MAX_REQUEST_BYTES];
    std::string request;
    bool request_complete = false;

    while (!request_complete) {
        ssize_t received = recv(
            client_fd,
            buffer,
            sizeof(buffer),
            0
        );

        if (received > 0) {
            request.append(
                buffer,
                static_cast<std::size_t>(received)
            );

            auto newline_pos = request.find('\n');

            if (newline_pos != std::string::npos) {
                // 已找到换行符，只检查第一条请求的长度。
                if (newline_pos + 1 > MAX_REQUEST_BYTES) {
                    std::cerr
                        << "Request exceeds 1024 bytes including newline"
                        << std::endl;

                    close(client_fd);
                    close(listen_fd);
                    return 1;
                }

                // 保留第一条请求，包括结尾的换行符。
                request.resize(newline_pos + 1);
                request_complete = true;
            } else if (request.size() >= MAX_REQUEST_BYTES) {
                // 没有换行且已达到上限，无法再容纳结束符。
                std::cerr
                    << "Request exceeds 1024 bytes including newline"
                    << std::endl;

                close(client_fd);
                close(listen_fd);
                return 1;
            }
        } else if (received == 0) {
            // 对端结束发送，但我们仍未收到完整请求。
            std::cerr
                << "Client ended sending before completing a request"
                << std::endl;

            close(client_fd);
            close(listen_fd);
            return 1;
        } else {
            perror("recv");
            close(client_fd);
            close(listen_fd);
            return 1;
        }
    }

    // 6. 只有完整请求才能进入发送流程。
    if (request_complete) {
        std::cout << "Complete request: " << request;

        std::size_t sent_total = 0;

        while (sent_total < request.size()) {
            ssize_t sent = send(
                client_fd,
                request.data() + sent_total,
                request.size() - sent_total,
                MSG_NOSIGNAL
            );

            if (sent > 0) {
                sent_total += static_cast<std::size_t>(sent);
            } else if (sent == 0) {
                std::cerr << "send made no progress" << std::endl;

                close(client_fd);
                close(listen_fd);
                return 1;
            } else {
                perror("send");
                close(client_fd);
                close(listen_fd);
                return 1;
            }
        }

        std::cout << "Echoed " << sent_total << " bytes" << std::endl;
    }

    // 7. 释放连接 socket 和监听 socket。
    close(client_fd);
    close(listen_fd);

    return 0;
}
