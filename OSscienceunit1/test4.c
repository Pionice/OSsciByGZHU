
#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>
int p1,p2;
void killchilds();
void child1();
void child2();

void killchilds()
{
    kill(p1,16);
    kill(p2,17);
    
}
void child1()
{
    printf("child process 1 is killed !by parent\n");
    exit(0);
}
void child2()
{
    printf("child process 2 is killed !by parent\n");
    exit(0);
}
int main()
{
    if(p1 = fork())
    {
        if(p2 = fork())
        {
            signal(SIGINT,killchilds);//获取中断信号并调用处理函数
            wait(0);//等待子进程1结束
            wait(0);//等待子进程2结束
            //实现同步
            printf("parent process is killed\n");
            exit(0);
        }
        else
        {
            signal(SIGINT,SIG_IGN);//SIG_IGN忽略^c信号
            signal(17,child2);
            pause();//使当前进程等待
        }
    }
    else
    {
        signal(SIGINT,SIG_IGN);
        signal(16,child1);
        pause();//使当前进程等待
    }
}
