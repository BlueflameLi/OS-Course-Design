# OS-Course-Design
 操作系统课程设计

##  实验一

- 内核修改时有自己标签，用 dmesg 验证
  - Linux 内核标签（系统启动显示一次）
  - 显示当前系统名称和版本的系统调用（内核、用户都有显示）
  - 修改 nice 和 prio 值的系统调用功能（内核、用户都有显示）
  - 改变主机名称为自定义字符串的系统调用（内核、用户都有显示）

[实验一说明](./实验一/实验一.md)  

## 实验二

- 模块一，用 dmesg 验证
  - 设计一个模块，要求列出系统中所有内核线程的程序名、PID 、进程状态、进程优先级、父进程的 PID
  - 输出按列对齐
- 模块二，用 dmesg 验证
  - 设计一个带参数的模块,其参数为某个进程的 PID 号，模块的功能是列出该进程的家族信息，包括父进程、兄弟进程和子进程的程序名、PID 号及进程状态
  - 输出树结构，类似 pstree 的输出

[实验二说明](./实验二/实验二.md)

