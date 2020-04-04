#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TOTAL_NUM (16*16*512)
#define RANDOM_SEED (2923)
#define LAPLACE_SEED_BIG (1048576)
#define LAPLACE_SEED_SMALL (2045)


/*******************************************************************
* gauss随机分布
* beta--拉普拉斯分布的参数
* seed--随机数种子
*******************************************************************/
char gauss(double mean, double sigma)
{
    int i;
    double x;
    double y;
    char ret = 0;
    for(x=0,i=0; i<12; i++) 
    {
        x += rand() % 255 + 1;
    }
    x = x - 255*6.0;
    y = mean + x*sigma;
    ret = (char)(y+0.5);
    return(y);
}



double uniform(double a,double b,signed int seed)
{
	double t;
	seed=LAPLACE_SEED_SMALL*seed+1;
	
	seed=seed-(seed/LAPLACE_SEED_BIG)*LAPLACE_SEED_BIG;
	t=seed/((float)LAPLACE_SEED_BIG);
	t=a+(b-a)*t;

	return(t);
}

/*******************************************************************
* laplace随机分布
* beta--拉普拉斯分布的参数
* seed--随机数种子
*******************************************************************/
double laplace(double beta,signed int seed)
{
	double u1,u2,x;

	u1=uniform(0.,1.,rand()*seed);
	do {
		u2=uniform(0.,1.,rand()*seed);
	} while(u2 < 0);
	if(u1<=0.5)
		x=-beta*log(1.-u2);
	else
		x=beta*log(u2);
    x = x*22;
	return(x);
}
/*******************************************************************
* Normal Distribution
* 0:gauss
* 1:laplace
*******************************************************************/

void normal_distribution(int type, double *temp)
{
	double mean = 0.0;
    double sigma = 0.0;
	switch(type)
	{
		case 0:
			mean = 0.0;
			sigma = 1.0/12.0;
			*temp = gauss(mean, sigma);
			break;
		case 1:
			mean = 0.5;
			sigma = RANDOM_SEED;
			*temp = laplace(mean, sigma);
			break;
		default:
			break;
	}
	return;
}

/*******************************************************************
* Normal Distribution
*******************************************************************/
void main()
{
    double temp = 0.0;
    int i;
    srandom(RANDOM_SEED);

	for(i=0; i< TOTAL_NUM; i++) 
    {
        if(0 == i%8) printf("\n");
        //normal_distribution(0, &temp);
        normal_distribution(1, &temp);
		printf("%d, ", (int)temp);
    }
    printf("\n");
}


