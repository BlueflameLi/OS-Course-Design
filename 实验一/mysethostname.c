#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    printf("Input the new hostname：\n");
    char hostname[65];
    scanf("%s", hostname);
    if (syscall(442, hostname, strlen(hostname)) != -1)
        printf("The hostname changed to %s\n", hostname);
    else
        printf("Change Failure\n");
}
