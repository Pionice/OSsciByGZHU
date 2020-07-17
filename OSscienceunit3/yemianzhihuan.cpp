//
//  os.cpp
//  OS
//
//  Created by Gama on 2020/6/5.
//
//
#include <stdio.h>
#include <iostream>
#include <ctime>
#include <algorithm>
using namespace std;
#define TotalInstruction 320 //总指令数
class PageObject//页对象
{
public:
    int NO;//所在页数
    int stayTime;//已在内存停留时间
};
class Paper
{
private:
    float InterruptCount;//记录中断次数
    int MemoryNum;//内存块数量
    int PageNum;//页面数
    int address[TotalInstruction];//指令地址
    PageObject* InstrucPage;//指令页号，每条指令对应的页号
    PageObject* Truepage;//不包含重复页号的真正页地址流
    PageObject* phyMemory;//物理内存块
public:
    Paper()
    {
        InterruptCount = 0;
        MemoryNum = 0;
        PageNum = 0;
    }
    //创建指令序列
    void creatInstruction()
    {
        srand((unsigned)time(NULL));
        for(int i = 0;i<TotalInstruction;i+=4)
        {
            int temp = 0;
            temp = rand()%320;//在[0，319]的指令地址之间随机选取一起点m
            address[i] = temp;//顺序执行一条指令，即执行地址为m+1的指令
            temp = rand()%(temp+1);//在前地址[0,m+1]中随机选取一条指令并执行，该指令的地址为m’
            address[i+1]=temp;
            address[i+2]=temp+1;//顺序执行一条指令，其地址为m’+1
            temp = rand()%(320-(temp+2))+(temp+2);//在后地址[m’+2，319]中随机选取一条指令并执行
            address[i+3]=temp;
        }
    }
    //将指令序列变换为页地址流
    void Instruc2page()
    {
        InstrucPage = new PageObject[TotalInstruction];
        int n = 10;//每一页的指令数
        for(int i = 0;i<TotalInstruction;i++)
        {
            int temp1 = address[i]/n;
            int temp2 = address[i]%n;
            if(temp2==0)
            {
                InstrucPage[i].NO = temp1;
            }
            else
                InstrucPage[i].NO = temp1+1;
            InstrucPage[i].stayTime = 0;
        }
        //这里由于产生指令的规则(在顺序执行一条指令那里)会产生连续的重复页号，故将其合并
        Truepage = new PageObject[TotalInstruction];
        Truepage[0] = InstrucPage[0];
        for (int i = 1; i < TotalInstruction; i++)
        {
            if (Truepage[PageNum].NO != InstrucPage[i].NO)
            {
                PageNum++;
                Truepage[PageNum] = InstrucPage[i];
            }
        }
        delete InstrucPage;//使用完毕，即刻释放
    }
    //初始化内存块
    void InitMemory(int n)
    {
        //n为传入的内存块数量
        MemoryNum = n;
        phyMemory = new PageObject[MemoryNum];
        for(int i = 0;i<MemoryNum;i++)
        {
            phyMemory[i].NO = -1;
            phyMemory[i].stayTime = 0;
        }
    }
    //寻找空闲内存块
    int findMemory()
    {
        for(int i=0;i<MemoryNum;i++)
        {
            if(phyMemory[i].NO==-1)
                return i;
        }
        return -1;
    }
    //检测需要页面是否已在内存中
    int findPage(int target)
    {
        //target需求页号
        for(int i=0;i<MemoryNum;i++)
        {
            if(phyMemory[i].NO==target)
                return i;
        }
        return -1;
    }
    //寻找停留时间最长的页面
    int findMaxstayTime()
    {
        int flag = 0;
        for(int i =0;i<MemoryNum;i++)
        {
            if(phyMemory[i].stayTime>=phyMemory[flag].stayTime)
                flag = i;
        }
        return flag;
    }
    void showPageNo()
    {
        cout<<"当前所在内存的页号：";
        for(int i = 0;i<MemoryNum;i++)
        {
            if(phyMemory[i].NO!=-1)
                cout<<phyMemory[i].NO<<"    ";
        }
        cout<<endl<<endl;
    }
    void DeleteData()
    {
        delete Truepage;
        delete phyMemory;
    }
    //先进先出算法
    //直接查询处于内存中时长最久，即visit最大的页，将其换出
    void FIFO()
    {
        InterruptCount=0;//重置中断计数器
        int swap = 0;//需要交换的内存块下标
        int exist = -1;//需求页号是否存在内存的标志
        int freeMemory;//空闲内存块下标
        for(int i = 0;i<PageNum;i++)
        {
            //将页调入内存前应先检测内存中是否有该页
            exist = findPage(Truepage[i].NO);
            if(exist!=-1)
            {
                cout<<"当前请求页号"<<Truepage[i].NO<<"已在内存中"<<endl<<endl;
            }
            else
            //产生缺页中断
            {
                InterruptCount++;
                //检查内存中有无空闲块
                freeMemory = findMemory();
                if(freeMemory!=-1)//有空闲块
                {
                    phyMemory[freeMemory] = Truepage[i];
                    showPageNo();
                }
                else//无空闲块，需要换出调度
                {
                    swap = findMaxstayTime();
                    cout<<"当前请求页号"<<Truepage[i].NO<<"换出页号："<<phyMemory[swap].NO<<endl<<endl;
                    phyMemory[swap] = Truepage[i];
                    showPageNo();
                }
            }
            //经过一次请求调度，内存块中所有时长+1
            for(int i = 0;i<MemoryNum;i++)
            {
                phyMemory[i].stayTime++;
            }
        }
        InterruptCount-=10;//前十次引起中断是要往内存调入初始存在的页面，不算真正意义上的由于使用时缺页引发中断申请
        double miss = (InterruptCount/PageNum)*100;
        cout<<"FIFO算法缺页率:"<<miss<<"%"<<endl;
    }
    //最近最久未使用算法LRU
    //将刚访问过的页面stayTime置0，表示刚使用过
    void LRU()
    {
        InterruptCount=0;//重置中断计数器
        int swap = 0;//需要交换的内存块下标
        int exist = -1;//需求页号是否存在内存的标志
        int freeMemory;//空闲内存块下标
        for(int i = 0;i<PageNum;i++)
        {
            //将页调入内存前应先检测内存中是否有该页
            exist = findPage(Truepage[i].NO);
            if(exist!=-1)
            {
                phyMemory[exist].stayTime = 0;//该页刚刚访问，将stayTime置0
                cout<<"当前请求页号"<<Truepage[i].NO<<"已在内存中"<<endl<<endl;
            }
            else
                //产生缺页中断
            {
                InterruptCount++;
                //检查内存中有无空闲块
                freeMemory = findMemory();
                if(freeMemory!=-1)//有空闲块
                {
                    phyMemory[freeMemory] = Truepage[i];
                    showPageNo();
                }
                else//无空闲块，需要换出调度
                {
                    swap = findMaxstayTime();
                    cout<<"当前请求页号"<<Truepage[i].NO<<"换出页号："<<phyMemory[swap].NO<<endl<<endl;
                    phyMemory[swap] = Truepage[i];
                    showPageNo();
                }
            }
            //经过一次请求调度，内存块中所有时长+1
            for(int i = 0;i<MemoryNum;i++)
            {
                phyMemory[i].stayTime++;
            }
        }
        InterruptCount-=10;
        double miss = (InterruptCount/PageNum)*100;
        cout<<"LRU算法缺页率:"<<miss<<"%"<<endl;
    }
    //最佳淘汰算法OPT
    //选择永不使用或在未来最长时间内不再被访问的页面予以替换
    void OPT()
    {
        InterruptCount=0;//重置中断计数器
        int swap = 0;//需要交换的内存块下标
        int exist = -1;//需求页号是否存在内存的标志
        int freeMemory;//空闲内存块下标
        int Distance[MemoryNum];//已在内存的页面对下次需要自己时的距离
        for(int i = 0;i<MemoryNum;i++)
        {
            Distance[i]=0;//重置
        }
        for(int i = 0;i<PageNum;i++)
        {
            //将页调入内存前应先检测内存中是否有该页
            exist = findPage(Truepage[i].NO);
            if(exist!=-1)
            {
                cout<<"当前请求页号"<<Truepage[i].NO<<"已在内存中"<<endl<<endl;
            }
            else
                //产生缺页中断
            {
                //每次中断重置Distance数组
                for(int i = 0;i<MemoryNum;i++)
                {
                    Distance[i]=0;
                }
                InterruptCount++;
                //检查内存中有无空闲块
                freeMemory = findMemory();
                if(freeMemory!=-1)//有空闲块
                {
                    phyMemory[freeMemory] = Truepage[i];
                    showPageNo();
                }
                else//无空闲块，需要换出调度
                {
//根据OPT算法特性，对每一个存在内存的页号，在余下页地址流中寻找其距离，对比各个页号到下次调度到的距离，距离最长的stayTime置最大值
                    for(int index_i = 0;index_i<MemoryNum;index_i++)
                    {
                        for(int index_j = i;index_j<PageNum;index_j++)
                        {
                            if(phyMemory[index_i].NO!=Truepage[index_j].NO)
                            {
                                Distance[index_i]++;
                            }
                            else
                                break;
                        }
                    }
                    //找到距离最大的内存块下标，将其stayTime修改为inf
                    int tempIndex = 0;
                    for(int i = 0;i<MemoryNum;i++)
                    {
                        if(Distance[i]>=Distance[tempIndex])
                            tempIndex = i;
                    }
                    phyMemory[tempIndex].stayTime=INT_MAX;
                    swap = findMaxstayTime();
                    cout<<"当前请求页号"<<Truepage[i].NO<<"换出页号："<<phyMemory[swap].NO<<endl<<endl;
                    phyMemory[swap] = Truepage[i];
                    showPageNo();
                }
            }
            //经过一次请求调度，内存块中所有时长+1
            for(int i = 0;i<MemoryNum;i++)
            {
                phyMemory[i].stayTime++;
            }
        }
        InterruptCount-=10;
        double miss = (InterruptCount/PageNum)*100;
        cout<<"OPT算法缺页率:"<<miss<<"%"<<endl;
    }
    
};

int main()
{

    Paper p;
    p.creatInstruction();
    p.Instruc2page();
    p.InitMemory(4);
    cout<<"1FIFO算法    2LRU算法    3OPT算法"<<endl;
    int ch=0;
    cin>>ch;
    switch (ch)
    {
        case 1:
        {p.FIFO();break;}
        case 2:
        { p.LRU();break;}
        case 3:
        { p.OPT();break;}
        default:
        {
            cout<<"输入错误，请重新输入"<<endl;
            break;
        }
    }
    p.DeleteData();
    cout<<"执行完毕"<<endl;
    return 0;
}

