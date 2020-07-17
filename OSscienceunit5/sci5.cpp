//
//  main.cpp
//  OSsci5
//
//  Created by Gama on 2020/6/22.
//  Copyright © 2020 admin. All rights reserved.
//

#include <iostream>
#include<vector>
#include <numeric>
#include<cstdlib>
#include<ctime>
#include<cmath>
#include<algorithm>
using namespace std;
#define MaxDisSum 1000
#define MaxDisCount 1000
class DiskWork
{
private:
    vector<int>Distance;//磁头每次移到下一个磁道的距离
    int DiskCount;//磁道个数
    vector<int>request;
    int Start;//磁头初始位置
    int *SSTF_TempDisk;
    int *SCAN_TempDisk;
    int SSTF_count;
    int SCAN_count;
    int SCAN_dir;//扫描方向
public:
    void InitData();//初始化数据
    void FCFS();//先来先服务算法
    void SSTF(int index);//最短寻道时间优先算法
    void SSTF_main();
    void SCAN(int index);//电梯算法
    void SCAN_main();
    void SCAN_New();
    void quickSort(int left,int right,vector<int>a);
};
void DiskWork::InitData()
{
    //随机产生一组请求访问磁道序列
    srand((int)time(0));
    DiskCount = rand() % 1000;//随机产生磁道数
    DiskCount = 10;
    for(int i = 0;i<DiskCount;i++)
    {
        int a =rand()%MaxDisSum;
        request.push_back(a);
        cout<<a<<"    ";
        if((i+1)%10==0)
            cout<<endl;
    }
    cout<<endl;
    Start = rand()%MaxDisSum;//磁头位置随机选定
    SSTF_TempDisk = new int[DiskCount];
    SCAN_TempDisk = new int[DiskCount];
    SSTF_count=0;
    SCAN_count=0;
    SCAN_dir=1;
}
void DiskWork::FCFS()
{
    //对磁道进行处理。
    //输出磁头移动轨迹和移动的总磁道数
    int indexDisk = Start;
    cout<<"磁头移动轨迹："<<Start<<"->";
    for(int i = 0;i<request.size();i++)
    {
        Distance.push_back(abs(request[i]-indexDisk));//移动到下个请求磁道所移动的磁道数
        indexDisk=request[i];//修改当前磁头
        if(i==request.size()-1)
            cout<<request[i];
        else
            cout<<request[i]<<"->";
    }
    cout<<endl;
    cout<< "移动的总磁道数：" <<accumulate(Distance.begin(), Distance.end(), 0);//输出移动的总磁道数
    Distance.clear();//清除数据
}
void DiskWork::SSTF(int index)
{
    if(SSTF_count==DiskCount-1)
        cout<<request[index];
    else
        cout<<request[index]<<"->";
    SSTF_TempDisk[index] = -1;
    SSTF_count++;
    if(SSTF_count==DiskCount)
    {return;}
    //先用这个数组标志，后续优化可以用每次在vector中删除已用磁道来减少遍历数进行优化
    //每次寻道之前知道在整个请求序列中哪个磁道最近
    int min =  1001;
    int x = 0;
    //在请求序列找到离当前磁头最近的磁道号
    //计算距离
    for(int i = 0;i<request.size();i++)
    {
        if(SSTF_TempDisk[i]!=-1)
        {
            SSTF_TempDisk[i]= abs(request[i]-request[index]);
        }
    }
    for(int i = 0;i<request.size();i++)
    {
        if(SSTF_TempDisk[i]!=-1)
        {
            if(SSTF_TempDisk[i]<min)
            {
                min =SSTF_TempDisk[i];
                x = i;
            }
        }
    }
    Distance.push_back(abs(request[index]-request[x]));
    SSTF(x);
}
void DiskWork::SSTF_main()
{
    int indexDisk = Start;
    int min = 1001;
    int x = 0;
    for(int i =0;i<request.size();i++)
    {
        int temp = abs(indexDisk-request[i]);
        if(temp<min)
        {
            x=i;
            min = temp;
        }
    }
    Distance.push_back(abs(request[x]-indexDisk));
    cout<<indexDisk<<"->";
    SSTF(x);
    cout<<endl;
    cout<< "移动的总磁道数：" <<accumulate(Distance.begin(), Distance.end(), 0)<<endl;//输出移动的总磁道数
    Distance.clear();
}
void DiskWork::SCAN(int index)
{
    //规定先往比当前位置大的方向扫描，方向为1，相反方向为-1
    if(SCAN_count==DiskCount-1)
        cout<<request[index];
    else
        cout<<request[index]<<"->";
    SCAN_TempDisk[index] = -1;
    SCAN_count++;
    if(SCAN_count==DiskCount)
    {return;}
    //计算当前磁道到每个请求磁道的距离
    for(int i = 0;i<request.size();i++)
    {
        if(SCAN_TempDisk[i]!=-1)
        {
            SCAN_TempDisk[i]= abs(request[i]-request[index]);
        }
    }
    if(SCAN_dir==1)
    {
        int min = 1001;
        int x=-1;
        for(int i = 0;i<request.size();i++)
        {
            if(SCAN_TempDisk[i]!=-1)
            {
                if(request[i]>=request[index])
                {
                    if(SCAN_TempDisk[i]<min)
                    {
                        min = SCAN_TempDisk[i];
                        x = i;
                    }
                }
            }
        }
        if(x==-1)//当前方向无请求磁道
        {
            SCAN_dir=-1;//转换方向
            int max =-1001;
            int x = 0;
            for(int i = 0;i<request.size();i++)
            {
                if(SCAN_TempDisk[i]!=-1)
                {
                    if(request[i]>max)
                    {
                        max = request[i];
                        x = i;
                    }
                }
            }
            Distance.push_back(abs(request[x]-request[index]));
            SCAN(x);
        }
        else//当前方向有请求磁道
        {
            Distance.push_back(abs(request[x]-request[index]));
            SCAN(x);
        }
    }
    else //往相反方向移动
    {
        int max =-1001;
        int x = 0;
        for(int i = 0;i<request.size();i++)
        {
            if(SCAN_TempDisk[i]!=-1)
            {
                if(request[i]>max)
                {
                    max = request[i];
                    x = i;
                }
            }
        }
        Distance.push_back(abs(request[x]-request[index]));
        SCAN(x);
    }
}
void DiskWork::SCAN_main()
{
    int indexDisk = Start;
    int min = 1001;
    int x = -1;
    //当前磁道到每个请求磁道的距离,找到当前方向上最近的磁道
    for(int i =0;i<request.size();i++)
    {
        if(request[i]>=indexDisk)
        {
            int temp = abs(indexDisk-request[i]);
            if(temp<min)
            {
                x=i;
                min = temp;
            }
        }
    }
    if(x==-1)
    {
        //此时往相反方向寻找
        int max = -1001;
        SCAN_dir = -1;
        for(int i = 0;i<request.size();i++)
        {
            if(request[i]<indexDisk)
            {
                int temp = abs(indexDisk-request[i]);
                if(temp>max)
                {
                    max = temp;
                    x = i;
                }
            }
        }
    }
    Distance.push_back(abs(request[x]-indexDisk));
    cout<<indexDisk<<"->";
    SCAN(x);
    cout<<endl;
    cout<< "移动的总磁道数：" <<accumulate(Distance.begin(), Distance.end(), 0)<<endl;//输出移动的总磁道数
    Distance.clear();
}


void DiskWork::SCAN_New()
{
    //对请求序列进行排序
    //quickSort(0,(int)Distance.size()-1, Distance);
    sort(request.begin(),request.end());
    //找到初始磁头在请求磁道序列的位置
    int ing = 0;
    for(int i = 0;i<request.size();i++)
    {
        if(Start>=request[i])
            ing = i;
    }
    //从里向外移动
    for(int i = ing;i<request.size();i++)
    {
        cout<<request[i]<<"    ";
        if(i%10==0)
            cout<<endl;
    }
    //从外向里移动
    for(int i = ing-1;i>-1;i--)
    {
        cout<<request[i]<<"    ";
        if(i%10==0)
            cout<<endl;
    }
    
}


int main(int argc, const char * argv[]) {
    DiskWork d;
    d.InitData();
    cout<<"FCFS算法磁头移动轨迹："<<endl;
    d.FCFS();
    cout<<endl;
    cout<<"SSTF算法磁头移动轨迹："<<endl;
    d.SSTF_main();
    cout<<"SCAN算法磁头移动轨迹："<<endl;
    d.SCAN_main();
    cout<<endl<<endl;
    cout<<"SCAN算法使用快排版本的磁头移动轨迹："<<endl;
    d.SCAN_New();
    return 0;
}
