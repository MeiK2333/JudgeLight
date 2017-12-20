#include "limit.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "listen.h"

unsigned long long str2long(char *t)
{
    long long ans = 0;
    for (int i = 0; t[i] != '\0'; i++)
    {
        ans *= 10;
        ans += t[i] - '0';
    }
    return ans;
}

void _listen(int operin, int operout)
{
#define READEND write(operout, "bilibili\0", 9)
    int n;
    char readbuffer[READ_MAX];
    while ((n = read(operin, readbuffer, READ_MAX)) > 0) // 监听指令
    {
        READEND;
        if (strcmp(readbuffer, "limit") == 0) // 限制资源指令
        {
            read(operin, readbuffer, READ_MAX);
            READEND;
            char tmp[READ_MAX]; // 接收具体限制数值
            read(operin, tmp, READ_MAX);
            READEND;

            unsigned long long l = str2long(tmp);
            setRunLimit(readbuffer, l);
        }
        else if (strcmp(readbuffer, "fd") == 0) // 重定向标准流向
        {
            read(operin, readbuffer, READ_MAX);
            READEND;
            char tmp[READ_MAX];
            read(operin, tmp, READ_MAX);
            READEND;

            int fd = (int)str2long(tmp);
            if (strcmp(readbuffer, "in") == 0)
            {
                dup2(fd, 0);
            }
            else if (strcmp(readbuffer, "out") == 0)
            {
                dup2(fd, 1);
            }
            else if (strcmp(readbuffer, "err") == 0)
            {
                dup2(fd, 2);
            }
            else if (strcmp(readbuffer, "close") == 0)
            {
                close(fd);
            }
        }
        else if (strcmp(readbuffer, "run") == 0)
        {
            read(operin, readbuffer, READ_MAX);
            READEND;

            int cnt = (int)str2long(readbuffer);
            char **args = (char **)malloc(sizeof(char *) * (cnt + 1));
            for (int i = 0; i < cnt; i++)
            {
                read(operin, readbuffer, READ_MAX);
                READEND;

                int len = (int)str2long(readbuffer);
                char *tmp = (char *)malloc(sizeof(char) * (len + 1));

                read(operin, readbuffer, READ_MAX);
                READEND;

                strcpy(tmp, readbuffer);
                args[i] = tmp;
            }
            args[cnt] = NULL;

            read(operin, readbuffer, READ_MAX);
            READEND;

            execvp(args[0], (char *const *)args);
            // exit(0);
        }
    }
}