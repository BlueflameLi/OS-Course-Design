#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <signal.h>
#include <fcntl.h>

#define MAX_BUF_SIZE 150

int main()
{
    //fd[0]读，fd[1]写
    int fd[2], pipe_num;
    //缓冲区
    char buf[MAX_BUF_SIZE];

    //互斥信号量
    sem_t *Mutex;
    // 同步信号量
    sem_t *send1, *send2, *send3;
    sem_t *receive1, *receive2, *receive3;

    pid_t pid1, pid2, pid3;

    // 清空缓冲区
    memset(buf, 0, sizeof(buf));

    // 清空所有信号量
    sem_unlink("Mutex");
    sem_unlink("send1");
    sem_unlink("send2");
    sem_unlink("send3");
    sem_unlink("receive1");
    sem_unlink("receive2");
    sem_unlink("receive3");

    // 创建信号量，0666权限，可读可写，1和0为初始值
    Mutex = sem_open("Mutex", O_CREAT, 0666, 1);
    send1 = sem_open("send1", O_CREAT, 0666, 1);
    send2 = sem_open("send2", O_CREAT, 0666, 1);
    send3 = sem_open("send3", O_CREAT, 0666, 1);
    receive1 = sem_open("receive1", O_CREAT, 0666, 0);
    receive2 = sem_open("receive2", O_CREAT, 0666, 0);
    receive3 = sem_open("receive3", O_CREAT, 0666, 0);

    //创建无名管道
    pipe_num = pipe(fd);

    if (pipe_num == -1) //创建失败
    {
        printf("管道创建失败\n");
        return 1;
    }

    //创建子进程
    pid1 = fork();
    if (pid1 > 0)
    {
        // 创建成功，则父进程再创建子进程2
        pid2 = fork();
        if (pid2 > 0)
            pid3 = fork();
    }

    // 子进程创建失败
    if (pid1 < 0 || pid2 < 0 || pid3 < 0)
    {
        //退出并清空信号量
        sem_unlink("Mutex");
        sem_unlink("send1");
        sem_unlink("send2");
        sem_unlink("send3");
        sem_unlink("receive1");
        sem_unlink("receive2");
        sem_unlink("receive3");
        printf("fork 失败%d %d %d\n", pid1, pid2, pid3);
        return 2;
    }

    if (pid1 == 0)
    {
        //关闭读管道口
        close(fd[0]);

        //同步访问，防止写的时候读
        sem_wait(send1);
        // 互斥访问
        sem_wait(Mutex);

        printf("进程1放入数据：\n");
        // 读入缓冲区
        scanf("%[^\n]%*c", buf);

        // 写入管道文件
        write(fd[1], buf, strlen(buf));

        sleep(1);

        // 释放信号量
        sem_post(Mutex);
        sem_post(receive1);
    }
    else if (pid2 == 0)
    {
        close(fd[0]);
        sem_wait(send2);
        sem_wait(Mutex);
        printf("进程2放入数据：\n");
        scanf("%[^\n]%*c", buf);
        write(fd[1], buf, strlen(buf));
        sleep(1);
        sem_post(Mutex);
        sem_post(receive2);
    }
    else if (pid3 == 0)
    {
        close(fd[0]);
        sem_wait(send3);
        sem_wait(Mutex);
        printf("进程3放入数据：\n");
        scanf("%[^\n]%*c", buf);
        write(fd[1], buf, strlen(buf));
        sleep(1);
        sem_post(Mutex);
        sem_post(receive3);
    }
    else
    {
        // 关闭写管道口，防止读的时候写
        close(fd[1]);

        sem_wait(receive1);
        sem_wait(receive2);
        sem_wait(receive3);
        sem_wait(Mutex);

        // 从读管道口读到缓冲区
        read(fd[0], buf, 150);
        printf("父进程接收数据:%s\n", buf);

        sleep(1);

        sem_post(Mutex);
        sem_post(send1);
        sem_post(send2);
        sem_post(send3);
    }
    return 0;
}