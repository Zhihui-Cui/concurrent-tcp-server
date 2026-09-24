# Concurrent TCP Server

Linux 并发任务服务学习项目：把外部请求接到已有 C++17 线程池，理解请求、队列、执行与响应。

**状态：已初始化项目说明与任务，尚未实现服务端，也尚无可运行构建目标。**

## 时间与前置

W6–W7：2026-10-11～10-24，限时两周。先完成 [线程池核心机制独立验收](https://github.com/Zhihui-Cui/cpp-thread-pool/issues/8)、Linux 验证与测量改进。以实际掌握情况调整，不因仓库建好就提前切换主任务。

总控：[ai-infra-roadmap](https://github.com/Zhihui-Cui/ai-infra-roadmap)。

## 按顺序推进

| 时间 | 任务 | 交付 |
| --- | --- | --- |
| W6 | [#1 最小协议与单请求](https://github.com/Zhihui-Cui/concurrent-tcp-server/issues/1) | Linux 构建、消息边界、部分读写、断开与正确响应 |
| W6 | [#2 接入线程池](https://github.com/Zhihui-Cui/concurrent-tcp-server/issues/2) | 确定性任务、结果交付、连接与对象所有权 |
| W7 | [#3 并发、容量与关闭](https://github.com/Zhihui-Cui/concurrent-tcp-server/issues/3) | 多客户端、过载策略、解除阻塞并回收 |
| W7 | [#4 测试与压测](https://github.com/Zhihui-Cui/concurrent-tcp-server/issues/4) | 可重复测试、吞吐/延迟/错误率及报告 |

第一步：解释一次请求如何从客户端到达服务端，再在设计笔记中写一组请求与响应示例。echo 可用于连通性练习，最终任务应有确定的输入输出和响应关联。

## 范围

- Linux 环境，C++17；Windows 可用合适的 Linux 环境进行练习。
- 使用简单 TCP 协议；必要时使用成熟库并说明取舍。
- 明确消息边界、长度上限、错误响应、容量和关闭行为。
- 固定版本复用 [cpp-thread-pool](https://github.com/Zhihui-Cui/cpp-thread-pool)。
- epoll 先理解就绪通知，不以复杂网络框架为验收目标。

## 文件组织

当前只有说明、设计模板和忽略规则。随任务逐步添加 CMakeLists.txt、include/、src/、tests/、benchmarks/。

[设计记录](docs/design-notes.md)：每次只记做了什么、为什么、如何验证、限制、独立完成程度。

## 构建与验证

待 Issue #1 添加源码和真实构建命令后填写。目前不把空工程或尚未运行的测试标为通过。

## 学习方式

本人先提出需求并实现第一版；卡住时请求提示、反例和解释，再进行代码审查。每项任务通过验收后记录提交及证据，再关闭 Issue。
