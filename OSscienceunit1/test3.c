#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
int i;
int main()
{
    if ( fork() == 0 )       // 子进程1
    {
        lockf(1,1,0);
        for(i = 0;i<5;i++)
            printf("创建子进程1\n");
        lockf(1,0,0);
        //exit(0);
        
    }
    else
    {
        if ( fork() == 0 )   // 子进程2
        {
            lockf(1,1,0);
            for(i = 0;i<5;i++)
                printf("创建子进程2\n");
            lockf(1,0,0);
            //exit(0);
            
        }
        
        else
        {
            lockf(1,1,0);
            for(i = 0;i<5;i++)
                printf("创建父进程\n");      // 父进程
            lockf(1,0,0);
        }
        //exit(0);
    }
}
