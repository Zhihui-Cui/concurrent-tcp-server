# Concurrent TCP Server：最小请求路径练习

P1 当前实现是 **Linux 上的单连接、单请求 TCP echo 服务端**。客户端发送以换行符结束的一条请求，服务端原样返回这条请求后退出。仓库名称保留，当前版本尚未实现并发服务。

任务：[Issue #1](https://github.com/Zhihui-Cui/concurrent-tcp-server/issues/1) · 总控：[学习路线](https://github.com/Zhihui-Cui/ai-infra-roadmap/blob/main/roadmap.md)。

## 构建与运行

在 Ubuntu / WSL 终端进入仓库目录。使用 C++17、CMake、GNU Make；测试客户端只依赖 Python 3 标准库。

终端 A：

```bash
bash dev.sh build
./build/tcp_server
```

出现 `Listening on 127.0.0.1:8080` 后，在同一个 Ubuntu 环境的终端 B、仓库目录运行：

```bash
python3 tests/echo_client.py normal
```

预期客户端收到 `b"hello\n"`；服务端输出 `Echoed 6 bytes`，然后退出。**每测试一个案例，都需要在终端 A 重新启动服务端。**

```bash
bash dev.sh doctor          # 查看工具版本
bash scripts/check-env.sh  # 检查工具、必要时安装，并编译；不会启动监听
bash dev.sh run             # 编译后启动服务端
bash dev.sh debug           # 编译后进入 GDB
```

## 协议与范围

- 监听 `127.0.0.1:8080`，处理一个连接中的第一条请求。
- 请求以 LF（`\n`）结束，长度上限 **1024 字节，包含 LF**。
- 响应为第一条完整请求的原始字节，包括结尾 LF。
- 未收到 LF 就遇到 EOF，或请求无法在长度上限内结束：不发送应用层响应，关闭连接并以状态码 1 退出。
- 循环接收、累积实际读取的字节；按实际发送长度推进偏移。正常完成以状态码 0 退出。
- 尚无连续接客循环、并发、线程池、服务端读写超时、连接复用或性能测试。

## 已有验证与记录

学习过程中已运行普通 echo、分两次发送、不完整请求、恰好 1024 字节和超限输入案例。详细日志依据、预期结果与未验证项见 [验证记录](docs/validation.md)。本次整理将手动客户端用例保存为脚本；不将新脚本的保存视为已重跑测试。

- [设计与学习记录](docs/design-notes.md)：请求路径、资源归属、独立完成程度与已知限制。
- [环境说明](docs/environment.md)：工具版本、WSL 与 VS Code 使用方式。
- [验证记录](docs/validation.md)：复现命令及实际结果。

## 学习预算

沿用 2026-09-29 的范围调整：整个练习预算 **6–10 小时**，不延误后续模型训练。实际耗时尚未记录，不能把预算当作已花费时间。

线程池集成、多客户端与过载、完整压测继续延期，对应历史 Issue #2–#4。当前提交记录已有成果；Issue #1 的实际耗时、断开后重启重连专项验证仍待补齐，不据此声明全部验收完成。
