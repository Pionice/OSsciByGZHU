//
//  main.c
//  sci_4
//  Created by Gama on 2020/6/15.
//
//  Copyright © 2020 admin. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#define SIZE 1024
int main(int argc, const char * argv[]) {
    // insert code here...
    char buf[SIZE];
    long readed=0;//已读取的字节数
    FILE *source,*backup;
    source=fopen("/Users/admin/Desktop/source.dat","r");
    backup=fopen("/Users/admin/Desktop/backup.dat", "w");
    if(source==NULL)
    {
        printf("源文件打开失败");
        exit(0);
    }
    if(backup==NULL)
    {
        printf("目标文件打开失败");
        exit(0);
    }
    while( (readed=(fread(buf, sizeof(char),SIZE, source))) >0)
    {
        fwrite(buf, sizeof(char),readed,backup);//写入数据到文件
        if(ferror(backup))
        {
            //若写入错误则返回
            printf("写入目的文件失败");
            exit(0);
        }
    }
    if(ferror(source))
    {
        printf("源文件读取错误");
        exit(0);
    }
    if(fclose(backup))
    {
        printf("目的文件流关闭失败");
        exit(0);
    }
    if(fclose(source))
    {
        printf("源文件流关闭失败");
        exit(0);
    }
    return 0;
}
