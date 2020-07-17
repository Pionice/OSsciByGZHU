#include<stdio.h>
#include <sys/wait.h>
#include<stdlib.h>
#include<unistd.h>
int main( )
{
    int pid;
    pid=fork( );         //创建子进程
    switch(pid)
    {
        case  -1://创建失败
            printf("创建失败\n");
            exit(1);//1返回有错
        case  0://子进程
            execl("/home/yeshu/science2","./science2",NULL);
            printf("exec创建失败\n");//当execl函数调用失败时才会执行，否则由science2里
            exit(1);         //新程序内容替代
            
        default://父进程
            wait(NULL);//同步
            printf("父进程：\n");
            exit(0);
    }
}
