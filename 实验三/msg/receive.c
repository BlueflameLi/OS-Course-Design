#include "msg.h"

int main()
{

    key_t key;
    int msqid;
    my_msgbuf msgp;
    int over1 = 0, over2 = 0;

    sem_unlink("mutex");
    // sem_unlink("sender1_over");
    // sem_unlink("sender2_over");
    sem_unlink("sender1_receive");
    sem_unlink("sender2_receive");

    mutex = sem_open("mutex", O_CREAT, 0666, 1);
    // sender1_over = sem_open("sender1_over", O_CREAT, 0666, 0);
    // sender2_over = sem_open("sender2_over", O_CREAT, 0666, 0);
    sender1_receive = sem_open("sender1_receive", O_CREAT, 0666, 0);
    sender2_receive = sem_open("sender2_receive", O_CREAT, 0666, 0);

    // 生成消息队列的键值
    if ((key = ftok(PATHNAME, PROJID)) < 0)
    {
        printf("无法生成key\n");
        exit(-1);
    }

    // 根据 key 创建消息队列
    if ((msqid = msgget(key, 0666 | IPC_CREAT)) == -1)
    {
        printf("消息队列创建失败\n");
        exit(-1);
    }

    int first = 0;
    while (1)
    {
        // printf("等待mutex\n");
        // if (!first)
        //     first = 1;
        // else
        //     sem_wait(mutex);
        printf("等待消息\n");
        // 消息队列id 消息结构体指针 消息结构体字符数组大小
        // type为RECEIVE返回队列第一个类型为RECEIVE的消息 flag为0阻塞式接受消息
        if (msgrcv(msqid, (void *)&msgp, MSG_MAX, RECEIVE, 0) == -1)
        {
            printf("消息接收失败\n");
            exit(-1);
        }

        printf("收到了来自进程%d的消息：%s\n", msgp.sendid, msgp.mtext);

        if (!strcmp(msgp.mtext, "end1"))
        {
            // 消息类型 SENDER1 这里当作收件人用
            msgp.mtype = SENDER1;
            // 发件人
            msgp.sendid = RECEIVE;

            strcpy(msgp.mtext, "over1");

            // 发送消息 flag为0阻塞式
            if (!msgsnd(msqid, (void *)&msgp, MSG_MAX, 0))
            {
                printf("sender1消息发送成功\n");
                // sem_post(mutex);
                // sem_post(sender1_over);
                sem_wait(sender1_receive);
            }
            else
            {
                printf("sender1_receive消息发送失败\n");
                exit(-1);
            }
            over1 = 1;
        }
        else if (!strcmp(msgp.mtext, "end2"))
        {
            // 消息类型 SENDER2 这里当作收件人用
            msgp.mtype = SENDER2;
            // 发件人
            msgp.sendid = RECEIVE;

            strcpy(msgp.mtext, "over2");

            // 发送消息 flag为0阻塞式
            if (!msgsnd(msqid, (void *)&msgp, MSG_MAX, 0))
            {
                printf("sender2消息发送成功\n");
                // sem_post(mutex);
                // sem_post(sender2_over);
                sem_wait(sender2_receive);
            }
            else
            {
                printf("sender2_receive消息发送失败\n");
                exit(-1);
            }
            over2 = 1;
        }
        // else
            // sem_post(mutex);
        if (over1 && over2)
            break;
    }

    // sem_wait(mutex);
    if (msgctl(msqid, IPC_RMID, NULL) < 0)
    {
        printf("消息队列删除失败\n");
        exit(-1);
    }
    return 0;
}