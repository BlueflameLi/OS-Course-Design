#include "msg.h"

int main()
{
    key_t key;
    int msqid;
    my_msgbuf msgp;
    char buf[MSG_MAX];

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

    // 消息类型(收件人)指定
    msgp.mtype = RECEIVE;
    // 发件人
    msgp.sendid = SENDER1;
    while (1)
    {
        printf("等待mutex\n");
        sem_wait(mutex);
        printf("请输入要发送的消息：\n");
        scanf("%[^\n]%*c", buf);

        if (!strcmp(buf, "exit"))
        {
            strcpy(msgp.mtext, "end1");

            if (!msgsnd(msqid, (void *)&msgp, MSG_MAX, 0))
            {
                sem_post(mutex);
                break;
            }
            else
            {
                printf("sender1_over消息发送失败\n");
                exit(-1);
            }
        }
        strcpy(msgp.mtext, buf);
        if (msgsnd(msqid, (void *)&msgp, MSG_MAX, 0))
        {
            printf("sender1消息发送失败\n");
            exit(-1);
        }
        printf("发送成功\n");
        sem_post(mutex);
    }
    // sem_wait(sender1_over);
    // sem_wait(mutex);
    if (msgrcv(msqid, (void *)&msgp, MSG_MAX, SENDER1, 0) == -1)
    {
        printf("sender1_receive消息接收失败\n");
        exit(-1);
    }
    printf("收到了来自进程%d的消息：%s\n", msgp.sendid, msgp.mtext);
    sem_post(sender1_receive);
    // sem_post(mutex);
    return 0;
}