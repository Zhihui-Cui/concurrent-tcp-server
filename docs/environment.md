# P1 开发环境

使用现有 Ubuntu-24.04（WSL2）。Ubuntu 虚拟磁盘位于 D 盘，项目位于 E 盘，经 WSL 的 `/mnt/e` 访问；本次整理不迁移目录、不调整 WSL 或交换文件设置。此记录不代表已核验所有 WSL 缓存、交换文件的存放位置。

## 已记录的版本

下表来自现有本地 `build/environment.txt`，不是本次重新探测的结果。

| 工具 | 版本 |
| --- | --- |
| Linux 内核 | 6.18.33.2-microsoft-standard-WSL2 |
| GCC / g++ | 13.3.0 |
| CMake | 3.28.3 |
| GDB | 15.1 |
| Git | 2.43.0 |
| Python | 3.12.3 |

构建使用 C++17、CMake（最低 3.20）和 GNU Make，无额外第三方网络库。无需 GPU、Docker 或线程池项目。

## 在 Ubuntu 的仓库目录执行

```bash
bash dev.sh doctor
bash dev.sh build
./build/tcp_server
```

服务器等待客户端连接属于正常行为。在另一个 Ubuntu 终端按 [验证记录](validation.md) 运行客户端。

`bash scripts/check-env.sh` 检查工具，缺少时调用 apt 安装（可能要求 Linux 密码），将版本写入被忽略的 `build/environment.txt`，最后只编译，不启动服务器。编译通过不等于网络行为测试通过。

调试：

```bash
bash dev.sh debug
```

进入 GDB 后可依次输入 `break main`、`run`、`next`，用 `quit` 退出。

## VS Code

用 Windows 安装的 VS Code 通过 WSL 扩展打开 Ubuntu 中的项目，左下角应显示 **WSL: Ubuntu-24.04**；C/C++ 扩展在 WSL 侧运行。在 Ubuntu 仓库目录可使用 `code .`。

本项目使用 Linux 的 `netinet/in.h` 等头文件，普通 Windows 窗口下的本机 C++ 环境不能直接构建。Codex 插件不是编译、调试或完成学习的依赖，可以继续在独立聊天应用中讨论代码。

## 构建文件与已见问题

构建产物放入已忽略的 `build/`；`.vscode/` 保留为本地编辑器配置，不纳入提交。

此前运行日志出现过 GNU Make 的 `Clock skew detected` 和文件时间稍晚于当前时间的警告。已有运行结果有效，但警告原因未在本次整理中诊断或修复；它可能影响增量构建判断。若再次出现，应先检查 Windows / WSL 的时钟与文件时间，再重新配置构建；不要据此宣称已经消除警告。
