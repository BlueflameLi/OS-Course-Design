#include "sharemy.h"
void *send(void *arg)
{
    char r_buf[100], tmp[SHM_SIZE];

    while (1)
    {
        sem_wait(sem_sender);

        printf("请输入发送的消息：\n");
        // fflush(stdout);
        scanf("%[^\n]%*c", r_buf);

        sprintf(tmp, "%d:%s", *((pid_t *)arg), r_buf);
        strcpy((char *)shmp, tmp);

        sem_post(sem_receiver1);
        printf("消息发送成功\n");

        // 发送完 over 就结束这个进程
        if (!strcmp(r_buf, "over"))
        {
            // 帮接收线程断开
            pthread_cancel(r_thread);

            // 和共享内存脱离
            shmdt(shmp);
            break;
        }
    }
}

void *receive(void *arg)
{
    char w_buf[SHM_SIZE], msg[SHM_SIZE];
    int s_pid;
    while (1)
    {
        sem_wait(sem_receiver2);
        strcpy(w_buf, (char *)shmp);
        sscanf(w_buf, "%d:%[^\n]%*c", &s_pid, msg);

        // 如果连续接收到消息可以通过\r把提示信息覆盖重写
        printf("接收到来自进程%d的消息：%s\n", s_pid, msg);

        // 接收到 over 也结束掉这个进程
        if (!strcmp(msg, "over"))
        {
            // 帮发送线程断开
            pthread_cancel(s_thread);

            // 和共享内存脱离
            shmdt(shmp);

            // 由接收端删除共享内存
            shmctl(shmid, IPC_RMID, NULL);

            // 清空信号量
            sem_unlink("sender");
            sem_unlink("receiver1");
            sem_unlink("receiver2");

            break;
        }
        // printf("请输入发送的消息2：");
        sem_post(sem_sender);
    }
}

int main()
{
    pid_t pid = getpid();

    init();

    printf("进程%d启动成功\n", pid);

    if (pthread_create(&s_thread, NULL, send, &pid) || pthread_create(&r_thread, NULL, receive, &pid))
    {
        printf("线程创建失败\n");
        return 1;
    }

    // 等待线程结束
    pthread_join(s_thread, NULL);
    pthread_join(r_thread, NULL);
    printf("进程%d结束\n", pid);
    return 0;
}