#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <stdio.h>
#define  SHMKEY  75
int shmid,i;
int *addr;

void  client()
{
    int i;
    shmid=shmget(SHMKEY,1024,0777);      /*打开共享存储区*/
    addr=shmat(shmid,0,0);           /*获得共享存储区首地址*/
    for (i=9;i>=0;i--)
    {
        while (*addr!=-1);
        printf("(client) sent\n");
        *addr=i;
    }
    exit(0);
}
void server()
{
    shmid=shmget(SHMKEY,1024,0777|IPC_CREAT); /*创建共享存储区*/
    addr=shmat(shmid,0,0);        /*获取首地址*/
    do
    {
        *addr=-1;
        while (*addr==-1);
        printf("(server) received\n");
    }
    while (*addr);
    shmctl(shmid,IPC_RMID,0);     /*撤消共享存储区，归还资源*/
    exit(0);
}

int main()
{
    while ((i=fork())==-1);
    if (!i)
        server();
    system("ipcs  -m");
    while ((i=fork())==-1);
    if (!i)
        client();
    wait(0);
    wait(0);
}
