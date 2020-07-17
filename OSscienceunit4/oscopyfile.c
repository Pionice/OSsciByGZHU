//
//  oscopyfile.c
//  sci_4
//
//  Created by Gama on 2020/6/15.
//
//  Copyright © 2020 admin. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include <errno.h>
#define SIZE 1024
int main(int argc, const char * argv[]) {
    // insert code here...
    char buf[SIZE];
    long readed=0;//已读取的字节数
    int source,backup;
    source = open("/Users/admin/Desktop/source.dat",O_RDONLY|S_IRUSR);//以只读方式打开源文件
    if(source<0)
    {
        printf("源文件打开失败");
        exit(0);
    }
    backup = open("/Users/admin/Desktop/backup.dat",O_WRONLY|O_CREAT|S_IWUSR);//以只写方式打开写入文件，如不存在则新建
    if(backup<0)
    {
        printf("目标文件打开失败");
        exit(0);
    }
    //往目标文件写入数据
    
    while((readed=read(source,buf,sizeof(buf)))>0)
    {
        write(backup,buf,readed);
    }
    if(close(backup)==-1)
    {
        printf("目的文件流关闭失败");
        exit(0);
    }
    if(close(source)==-1)
    {
        printf("源文件流关闭失败");
        exit(0);
    }
    return 0;
}
