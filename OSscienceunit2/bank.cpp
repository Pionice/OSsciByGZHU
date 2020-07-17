//  Created by Gama on 2020/5/20.
//  操作系统实验二银行家算法
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;
class bank
{
private:
    int m;//资源数
    int n;//进程数
    int* Available;//系统可利用资源
    int **Max;//进程最大需求矩阵
    int** Allocation;//进程已拥有资源矩阵
    int** Need;//进程需求矩阵
    int** request;//进程需求矩阵
    int count = 0;//计数器
    int index;//当前申请资源的进程下标
    bool initON=false;
public:
    void creatData();//初始化数组各种数据
    void RequestRes();//请求资源
    int Checksecurity();//安全性检测
    void print();//打印数据
    void deletedata();//释放动态内存
    int InitSecurity();//检测初始安全性
};
void bank::creatData()
{
    index = 0;
    srand((int)time(0));
    m = (rand() % 9) + 1;
    n = (rand() % 9) + 1;
    
    //m 资源个数 n 进程个数
    //申请可利用资源向量Available
    Available = new int[m];
    for (int i = 0; i < m; i++)
    {
        Available[i] = (rand() % 9) + 1;
    }
    //申请Max内存空间
    Max = new int* [n];
    for (int i = 0; i < n; i++)
    {
        Max[i] = new int[m];
    }
    
    //申请Allocation内存空间
    Allocation = new int* [n];
    for (int i = 0; i < n; i++)
    {
        Allocation[i] = new int[m];
    }
    
    //申请Need内存空间
    Need = new int* [n];
    for (int i = 0; i < n; i++)
    {
        Need[i] = new int[m];
    }
    
    
    
    //为数组赋值
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            Max[i][j] = 0;
            Allocation[i][j] = 0;
            Need[i][j] = 0;
        }
    }
    
    
    
    //最大需求矩阵Max
    
    
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            Max[i][j] = (rand() % 9) + 1;
        }
    }
    //分配矩阵Allocation
    
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            int temp = (rand() % 5) + 1;
            while (temp > Max[i][j])
                temp = (rand() % 5) + 1;
            Allocation[i][j] = temp;
        }
    }
    //需求矩阵Need
    
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            Need[i][j] = Max[i][j] - Allocation[i][j];
        }
    }
    
    request = new int* [n];
    for (int i = 0; i < n; i++)
    {
        request[i] = new int[m];
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            request[i][j] = 0;
        }
    }
    
}
void bank::print()
{
    cout << "当前系统拥有资源：";
    for (int i = 0; i < m; i++)
    {
        cout << Available[i] << "  ";
    }
    cout << endl << "当前系统进程资源分配情况：" << endl;
    int p = 2*m;
    cout << "PID";
    for (int i = 0; i < m+2; i++)
        cout << " ";
    cout << "Max";
    for (int i = 0; i < p; i++)
        cout << " ";
    cout << "Allocation";
    for (int i = 0; i < p; i++)
        cout << " ";
    cout << "Need"<<endl;
    
    
    for (int i = 0; i < n; i++)
    {
        cout << "P" << i << "      ";
        for (int j = 0; j < m; j++)
        {
            cout << Max[i][j] << " ";
        }
        cout << "      ";
        for (int j = 0; j < m; j++)
        {
            cout << Allocation[i][j] << " ";
        }
        cout << "      ";
        for (int j = 0; j < m; j++)
        {
            cout << Need[i][j] << " ";
        }
        cout << endl<<endl;
        
    }
}

void bank::deletedata()
{
    //删除动态申请的内存空间
    for (int i = 0; i < n; i++)
    {
        delete[]Max[i];
    }
    delete[]Max;
    for (int i = 0; i < n; i++)
    {
        delete[]Allocation[i];
    }
    delete[]Allocation;
    for (int i = 0; i < n; i++)
    {
        delete[]Need[i];
    }
    delete[]Need;
    for (int i = 0; i < n; i++)
    {
        delete[]request[i];
    }
    delete[]request;
}

int bank::InitSecurity()
{
    initON=true;
    int a = Checksecurity();
    initON=false;
    return a;
}

int bank::Checksecurity()
{
    int total_true = 0;//计算finish的true个数
    int total;//某个进程中满足资源需求的个数
    int* Temp = new int[m];//临时存放Available数据
    int* Finish = new int[n];//是否足够资源分配
    //初始化
    for (int i = 0; i < m; i++)
    {
        Temp[i] = Available[i];
        Finish[i] = 0;
    }
    if(initON==false)
    {
        if (count == m)
        {
            Finish[index] = 1;
            for (int i = 0; i < m; i++)
            {
                Temp[i] = Temp[i] + Allocation[index][i];
                //如果Need全0证明程序获得全部所需资源，视为执行完成，即刻释放该进程拥有资源
            }
            cout << "尝试分配资源成功，进程已获得所需全部资源，执行完毕并释放拥有资源" << endl;
        }
        else
        {
            cout << "尝试分配资源成功，分配资源还未满足该进程所需全部资源。" << endl;
        }
        cout << "正在进行安全性检测......";
        cout << endl << endl;
    }
    for (int k = 0; k < n; k++)
    {
        for (int i = 0; i < n; i++)
        {
            total = 0;
            for (int j = 0; j < m; j++)
            {
                if (Need[i][j] <= Temp[j])
                    total++;//计算满足资源的总类数
                //当达到资源总数时代表此时系统拥有资源满足该进程全部需求
                //Finish[i]=0表示该进程还未被尝试过
                if (Finish[i] == 0 && total == m)
                {
                    Finish[i] = 1;
                    for (int l = 0; l < m; l++)
                    {
                        //上面已判断现有资源全部满足需求，模拟全部分配并执行进程完成
                        //将该已完成的进程资源全部回收
                        Temp[l] = Temp[l] + Allocation[i][l];
                    }
                }
            }
        }
    }
    
    for (int i1 = 0; i1 < n; i1++)
    {
        if (Finish[i1] == 1)
            total_true++;
        //统计Finish[i]＝＝true的个数
    }
    if (total_true == n)
    {
        if(initON==false)
        {cout << "安全性检测通过！" <<endl<<endl;cout << endl;}
        
        total_true = 0;       //将计数器f重新初始化，为下一次提出新的进程申请做准备
        if(count==m)
        {
            for(int i = 0;i<m;i++)
            {
                Available[i]=Available[i]+Allocation[index][i];
                //视当前申请的进程已执行完毕，收回已分配资源。
            }
        }
        delete[]Temp;
        delete[]Finish;
        cout << endl;
        return 1;
    }
    else
    {
        if(initON==false)
            cout << "安全性检测不通过！分配失败！"<<endl;
        if(initON==false)
        {
            for (int i = 0; i < m; i++)
            {
                //取消分配，将数据恢复
                Available[i] = Available[i] + request[index][i];
                Allocation[index][i] = Allocation[index][i] - request[index][i];
                Need[index][i] = Need[index][i] + request[index][i];
            }
            cout << endl;
        }
        delete[]Temp;
        delete[]Finish;
        return 0;
    }
}
void bank::RequestRes()
{
    index = (rand() % n);
    cout << "当前随机选取进程" << index << "申请资源" << endl <<endl;
    //生成请求资源向量
    cout << "申请向量：";
    for (int i = 0; i < m; i++)
    {
        int temp;
        int R = Need[index][i];
        if (R == 0)
            temp = 0;
        else
        {
            //temp = Need[index][i] - 1;
            temp = (rand() % R) + 1;//保证申请资源数不超过需求
        }
        request[index][i] = temp;
        cout << request[index][i] << "  ";
    }
    cout << endl<<endl;
    
    //检查是否足够资源分配
    for (int i = 0; i < m; i++)
    {
        if (request[index][i] > Available[i])
        {
            cout << "当前进程无足够资源满足，请等待" << endl<<endl;
            return;
        }
    }
    //足够资源分配，尝试分配
    cout << "足够资源分配，尝试分配..." << endl<<endl;
    
    for (int i = 0; i < m; i++)
    {
        //以下是试探性分配
        Available[i] = Available[i] - request[index][i];
        Allocation[index][i] = Allocation[index][i] + request[index][i];
        Need[index][i] = Need[index][i] - request[index][i];
        if (Need[index][i] == 0)
            count++;
    }
    Checksecurity();
}
int main()
{
    bank B;
    B.creatData();
    //检测初始系统是否安全，不安全则重新生成数据直到安全为止。
    int ch = 0;
    ch  = B.InitSecurity();
    while(ch==0)
    {
        B.creatData();
        ch = B.InitSecurity();
    }
    B.print();//打印系统与进程关于资源的数据
    B.RequestRes();
    B.print();//再次打印
    B.deletedata();
    return 0;
}
