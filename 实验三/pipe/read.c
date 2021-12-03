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
    char r_buf[MAX_BUF_SIZE];
    int fd;
    int r_num;
    char flag = 'n';

    //若存在则直接使用，否则创建它
    if ((mkfifo(FIFO_NAME, 0666) < 0) && (errno != EEXIST))
    {
        printf("创建管道失败\n");
        exit(1);
    }

    //以阻塞型只读方式打开fifo
    //失败报错退出
    if ((fd = open(FIFO_NAME, O_RDONLY)) == -1)
    {
        printf("打开fifo失败\n");
        exit(1);
    }

    while (1)
    {
        printf("是否读取管道[y/n]: ");
        flag = getchar();
        getchar();
        if (flag == 'y')
        {
            r_num = read(fd, r_buf, MAX_BUF_SIZE);
            printf("读取到 %d bytes :%s\n", r_num, r_buf);
        }
        else
            break;
    }

    return 0;
}
