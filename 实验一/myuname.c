#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <stdlib.h>

#define __NEW_UTS_LEN 64
typedef struct
{
    char sysname[__NEW_UTS_LEN + 1];
    char nodename[__NEW_UTS_LEN + 1];
    char release[__NEW_UTS_LEN + 1];
    char version[__NEW_UTS_LEN + 1];
    char machine[__NEW_UTS_LEN + 1];
    char domainname[__NEW_UTS_LEN + 1];
} new_utsname;

int main()
{
    new_utsname name;
    syscall(440, &name);
    printf("Sysname:%s\nRelease:%s\nVersion:%s\n", name.sysname, name.release, name.version);
    return 0;
}