#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define FIFO_NAME "./myfifo"
#define MAX_BUF_SIZE 1024

int main()
{
    int fd;

    //若存在则直接使用，否则创建它
    if ((mkfifo(FIFO_NAME, 0666) < 0) && (errno != EEXIST))
    {
        printf("创建管道失败\n");
        exit(1);
    }

    //以阻塞型 只写方式打开fifo
    if ((fd = open(FIFO_NAME, O_WRONLY)) == -1)
    {
        printf("打开fifo失败\n");
        exit(1);
    }

    int cnt = 0;
    while (1)
    {
        char buf[MAX_BUF_SIZE];
        sprintf(buf, "这是第 %d 次写入的数据", ++cnt);
        write(fd, buf, MAX_BUF_SIZE);
        printf("已经写入%dkb数据\n", cnt);
    }

    return 0;
}
