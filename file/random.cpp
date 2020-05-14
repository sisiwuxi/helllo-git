#include <stdio.h>
#include <random>
#include <string>
//#include <>
//#include

using namespace std;

 

#define pi 3.1415926

#define rd (rand()/(RAND_MAX+1.0))

 

//区间[min,max]上的均匀分布,min和max要求传入的参数类型一致

template <<span style="color:blue">class T>

T rand(T min, T max)

{

    return min+(max-min)*rand()/(RAND_MAX+1.0);

}

//求均值为miu，方差为sigma的正太分布函数在x处的函数值

double normal(double x, double miu,double sigma)

{

    return 1.0/sqrt(2*pi)/sigma*exp(-1*(x-miu)*(x-miu)/(2*sigma*sigma));

}

//按照矩形区域在函数值曲线上下位置分布情况得到正太函数x值

double randn(double miu,double sigma, double min ,double max)

{

    double x,y,dScope;

    do{

        x=rand(min,max);

        y=normal(x,miu,sigma);

        dScope=rand(0.0,normal(miu,miu,sigma));

    }while(dScope>y);

    return x;

}

double randn(int type)

{

    //按照12个均匀分布之和减去6得到正态分布函数的x值

    if (type==1)

        return rd+rd+rd+rd+rd+rd+rd+rd+rd+rd+rd+rd-6.0;

    //按照计算公式y=sqrt(-2*ln(U))*cos(2*PI*V)计算得到x

    else if(type==2)

        return sqrt(-2*log(rand()/(RAND_MAX+1.0)))*cos(2*pi*rand()/(RAND_MAX+1.0));

    else

        return randn(0.0,1.0,-10.0,10.0);

}

int main(int argc,char* argv[])

{

    srand((unsigned)time( NULL )); 

    ofstream outfile("321.txt");

    for (int i=0;i<100;i++)

    {

        //randn(1)、randn(2)和randn(3)效果差不多

        outfile << randn(3) << endl;

    }

    return 0;

}
