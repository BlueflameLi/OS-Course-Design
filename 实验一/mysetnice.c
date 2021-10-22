#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    pid_t pid;
    int newnice;
    int prio = -1;
    int nice = -1;

    printf("Input the pid：\n");
    scanf("%d", &pid);

    printf("Input the new nice：\n");
    while (~scanf("%d", &newnice) && (newnice < -20 || newnice > 19))
        printf("nice out of the range\n");

    syscall(441, pid, newnice, &prio, &nice);

    printf("the nice is %d and the new prio is %d\n", nice, prio);
    return 0;
}