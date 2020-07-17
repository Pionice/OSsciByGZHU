#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
int i;
int main()
{
    if ( fork() == 0 )       // 子进程1
    {
        for(i = 0;i<5;i++)
            printf("创建子进程1\n");
        exit(0);
    }
    else
    {
        if ( fork() == 0 )   // 子进程2
        {
            for(i = 0;i<5;i++)
                printf("创建子进程2\n");
            exit(0);
        }
        
        else
        {
            for(i = 0;i<5;i++)
                printf("创建父进程\n");      // 父进程
            exit(0);
        }
    }
}
