#ifndef _MSG_H_
#define _MSG_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/types.h>
#include<fcntl.h>
#include<semaphore.h>
#include<unistd.h>
#define PROJID 125
#define PATHNAME "./msg.h"
#define MSG_MAX 100
#define RECEIVE 3
#define SENDER1 1
#define SENDER2 2

// 消息队列缓冲区
typedef struct{
      long mtype;           // 消息类型，这里看作收件人
      int sendid;           // 发送者id，可以看作发件人
      char mtext[MSG_MAX];  // 消息内容
}my_msgbuf;

sem_t *mutex;
// sem_t *sender1_over;
sem_t *sender1_receive;
// sem_t *sender2_over;
sem_t *sender2_receive;
#endif