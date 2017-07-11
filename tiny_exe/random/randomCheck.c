#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<math.h>

#define N 1000
#define K 20

double avenage(double *p, int num); 
double fangcha(double *p, double avg, int num);
void junyunxing(int *p, int k, int num);
void zijiance(double *p, double fangcha, double avg, int k, intnum);

int main(void)
{
	int i, c[N/K];
	double a[N], b[N], avg, fcha;
	for(i = 0; i < K; i++)
		c[i]= 0;

	srand((unsigned) time (NULL));

	for(i = 0; i < N; i++)
	{
		a[i]= rand() / (RAND_MAX + 1.0);
		c[(int) (a[i] * K)]++;
		if(a[i] < 0.25 && a[i] > 0)
			b[i]= 2 + sqrt (4 * a[i]);
		else
			b[i]= 6 - sqrt (12 - 12 * a[i]);
		//printf ("%.6f\n", b[i]);
	}
	avg= avenage(b, N);
	fcha= fangcha(b, avg, N);
	junyunxing(c, K, N);
	zijiance(b, fcha, avg, K, N);
	return 0;
}

double avenage(double *p, int num)
{
	double avg, sum = 0.0;
	int i;
	for(i = 0; i < num; i++)
	sum+= p[i];
	avg= sum / num;
	printf("\n avg=%lf\n",avg);
	return avg;
}


double fangcha(double *p, double avg, int num)
{
	double fangcha = 0.0;
	int i;
	for(i = 0; i < num; i++)
	fangcha+= (p[i] - avg) * (p[i] - avg);
	fangcha= fangcha / num;
	printf("\nfangcha=%lf\n",fangcha);
	return fangcha;
}


void junyunxing(int *p, int k, int num)
{
	int i;
	long p1 = 0;
	double p2;
	for(i = 0; i < k; i++)
		p1+= (p[i] - num / k) * (p[i] - num / k);
	p2= k * p1 / num;
	printf("\n p2=%lf\n",p2);
}


void zijiance(double *p, double fangcha, double avg, int k, int num)
{
	double r, p1, total = 0.0;
	int i;
	for(i = 0; i < num - k; i++)
		total+= p[i] * p[i + k];
	r= (total / (num - k) - avg * avg) / fangcha;
	printf("\n r=%lf\n",r);
	p1= r * sqrt (num - k);
	printf("\n p1=%lf\n",p1);
}


