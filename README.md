# JudgeLight

## 设置限制

- 时间与内存限制： `setrlimit`
- 系统调用限制： `ptrace`

## 获取资源占用

- CPU 时间： `wait4`
- 实际时间： `gettimeofday`
- 内存占用： `/proc/pid/status`
