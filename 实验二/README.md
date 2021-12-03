# 实验二

## 模块一

进入目录 1

通过  `make`  编译模块

```shell
make
```

安装内核模块

```shell
sudo insmod mypsl.ko
```

`dmesg`  验证

卸载内核模块

```shell
sudo rmmod mypsl
```

## 模块二

进入目录 2

通过  `make`  编译模块

```shell
make
```

安装内核模块并指定 pid，不指定则为设置的初始值

```shell
sudo insmod mypstree.ko pid=1
```

`dmesg` 验证

卸载内核模块

```shell
sudo rmmod mypstree
```

