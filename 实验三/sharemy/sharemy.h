#ifndef _SHAREMY_H_
#define _SHAREMY_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>

#include<pthread.h>
#include<fcntl.h>
#include<semaphore.h>
#include<unistd.h>

#define PROJID 127
#define PATHNAME "./sharemy.h"
#define SHM_SIZE 1024

sem_t *sem_sender;
sem_t *sem_receiver1;
sem_t *sem_receiver2;

key_t key;
int shmid;
void *shmp;

pthread_t r_thread, s_thread;

void init()
{
    sem_sender = sem_open("sender", O_CREAT, 0666, 1);
    sem_receiver1 = sem_open("receiver1", O_CREAT, 0666, 0);
    sem_receiver2 = sem_open("receiver2", O_CREAT, 0666, 0);

    // 生成消息队列的键值
    if ((key = ftok(PATHNAME, PROJID)) < 0)
    {
        printf("无法生成key\n");
        exit(-1);
    }

    // 创建共享内存
    if ((shmid = shmget(key, SHM_SIZE, 0666 | IPC_CREAT)) == -1)
    {
        printf("创建共享内存失败\n");
        exit(-1);
    }

    // 将共享内存段连接到进程地址空间
    // 共享内存标识  连接地址,NULL则由系统自动选择 SHM_RDONLY为只读模式，其他都是读写模式
    // 返回共享内存的首地址指针
    if ((long long int)(shmp = shmat(shmid, NULL, 0)) == -1)
    {
        printf("映射内存失败\n");
        exit(-1);
    }
}
#endif