#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_CMD_LEN 50       //输入命令最大长度
#define CMD_COLLECTION_LEN 7 //命令数组长度

#define INVALID_COMMAND -1 //未识别命令
#define EXIT 0             //正常退出
#define CMD_1 1
#define CMD_2 2
#define CMD_3 3
#define FIND 4
#define GREP 5
#define QUIT 6
#define TRUE 1

char *cmdStr[CMD_COLLECTION_LEN] = {"exit", "cmd1", "cmd2", "cmd3", "find", "grep", "quit"};

void cmd_split(char *cmdStr, char **token)
{
    int i = 0;
    char str[MAX_CMD_LEN];
    int offset;
    while (~sscanf(cmdStr, "%s%n", str, &offset))
    {
        cmdStr += offset;
        token[i] = (char *)malloc(sizeof(str));
        strcpy(token[i++], str);
    }
    token[i] = NULL;
}

int getCmdIndex(char *cmd)
{
    for (int i = 0; i < CMD_COLLECTION_LEN; i++)
        if (!strcmp(cmd, cmdStr[i]))
            return i;
    return -1;
}

int myFork(int cmdIndex, char **token)
{
    pid_t pid;
    int status = -1;
    if ((pid = fork()) < 0)
        status = -1;
    else if (!pid)
    {
        // printf("child is running\n");
        switch (cmdIndex)
        {
        case CMD_1:
        case CMD_2:
        case CMD_3:
        {
            char path[MAX_CMD_LEN];
            sprintf(path, "./%s", token[0]);
            status = execv(path, token);
            break;
        }
        case FIND:
            status = execvp("find", token);
            break;
        case GREP:
            status = execvp("grep", token);
        default:
            printf("Invalid Command\n");
            break;
        }
    }
    else
        wait(&status);

    return status;
}
void runCMD(int cmdIndex, char **token)
{
    switch (cmdIndex)
    {
    case INVALID_COMMAND:
        printf("COMMAND NOT FOUND \n");
        break;
    case EXIT:
    case QUIT:
        exit(0);
        break;
    default:
        if (myFork(cmdIndex, token) < 0)
            printf("fork error!");
        break;
    }
}
int main()
{
    pid_t pid;
    char cmdStr[MAX_CMD_LEN];
    char *token[MAX_CMD_LEN];
    int cmdIndex;
    while (TRUE)
    {
        printf("> ");
        scanf("%[^\n]%*c", cmdStr);
        cmd_split(cmdStr, token);
        // printf("%s\n", token[0]);
        // int i = 0;
        // while (token[i])
        // {
        //     printf("%d %s\n", i++, token[0]);
        // }
        // printf("%s\n", token[0]);
        cmdIndex = getCmdIndex(token[0]);
        runCMD(cmdIndex, token);
        printf("\n");
    }
}
