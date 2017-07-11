#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


//========================MACRO====================//
#define BIT7 0x080
#define BIT6 0x040
#define BIT5 0x020
#define B5BIT 0x01f
#define B76BIT 0x0c0
#define B54BIT 0x030
#define B4BIT 0x0f
#define B12345BIT 0x3e
#define B0BIT 0x01
#define B16BIT ((~(~0))|0xffff)
#define B8BIT ((~(~0))|0xff)
#define BL4BIT ((~(~0))|0x0f)
#define BH4BIT ((~(~0))|0xf0)

#define MACRO (8)//16
#define WIDTH (176)
#define HEIGHT (144)
#define HORIZONTAL_COORDINATE (WIDTH/MACRO)//22
#define VERTICAL_COORDINATE (HEIGHT/MACRO)//18
#define SEED_NUM (6)

struct seed
{
	int x;
	int y;
	int value;
}seed;

struct filter
{
	int enable;
	int ARRAY_Difference[HEIGHT][WIDTH];
	int AverageFilter[HEIGHT][WIDTH];	
}filter;

struct averageValue
{
	int TotalAverage;
	int MaxTotalAverage;
	int MinTotalAverage;
}averageValue;

int ARRAY_Y[HEIGHT][WIDTH] = {0};
int ARRAY_COORDINATE[VERTICAL_COORDINATE][HORIZONTAL_COORDINATE] = {0};
int ARRAY_LUMA[VERTICAL_COORDINATE][HORIZONTAL_COORDINATE] = {0};


struct seed Seeds[SEED_NUM];
struct seed SeedsMacro[SEED_NUM];
struct filter Filters[SEED_NUM];
struct averageValue AverageValue;

//========================PutYToFile====================//
void Y2File(void)
{
	FILE *fp1;
	FILE *fp2;
	int i = 0;
	int k = 1;
	int temp = 0;
	int H4temp = 0;
	int L4temp = 0;

	if((fp1 = fopen("176X144_BALL.y","rb")) == NULL)
		printf("eror fp1 open\n");
	if((fp2 = fopen("y2file.txt","wb")) == NULL)
		printf("eror fp2 open\n");

	while(!feof(fp1))
	{
		temp = fgetc(fp1) & B8BIT;
		fputc(0x30, fp2);//0
		fputc(0x78, fp2);//x
		H4temp = (temp >> 4) & BL4BIT;
		L4temp = temp & BL4BIT;

		if((H4temp >= 0x00) && (H4temp <= 0x09))
		{
			fputc((H4temp + 0x30), fp2);
		}
		else if((H4temp >= 0x0a) && (H4temp <= 0x0f))
		{
			fputc((H4temp + 0x37), fp2);//0x57
		}
		if((L4temp >= 0x00) && (L4temp <= 0x09))
		{
			fputc((L4temp + 0x30), fp2);
		}
		else if((L4temp >= 0x0a) && (L4temp <= 0x0f))
		{
			fputc((L4temp + 0x37), fp2);//0x57
		}
		//fputc(0x2C, fp2);
		if(k % 176)
		{
			fputc(0x2C, fp2);//,
		}
		else
		{
			fputc(0x2C, fp2);//,		
			fputc(0x0D, fp2);
			fputc(0x0A, fp2);
		}
		k++;
	}
	printf("\nk=%d\n",k);
	fclose(fp1);
	fclose(fp2);
	return;
}
//========================LOG_ARRAY_COORDINATE====================//
void LOG_ARRAY_Y(void)
{
	int i=0;
	int j=0;	
	printf("\n==============LOG_ARRAY_Y===============\n");	
	for(j=0; j<HEIGHT; j++)
	{
		for(i=0; i<WIDTH; i++)
		{	
			printf("%02x ",ARRAY_Y[j][i]);	
		}
		printf("\n");	
	}
	return;
}
//========================Y2File====================//
void Y2Array(void)
{
	FILE *fp1;
	int i=0;
	int j=0;		
	int k = 0;
	int temp = 0;

	if((fp1 = fopen("176X144_BALL.y","rb")) == NULL)
		printf("eror fp1 open\n");
	memset(ARRAY_Y, WIDTH*HEIGHT*sizeof(int), 0x0);
	while(!feof(fp1))
	{
		temp = fgetc(fp1) & B8BIT;	
		ARRAY_Y[k/WIDTH][k%WIDTH]=temp;
		k++;
	}
	fclose(fp1);
	LOG_ARRAY_Y();
	return;
}
//========================LOG_ARRAY_COORDINATE====================//
void LOG_ARRAY_COORDINATE(void)
{
	int i=0;
	int j=0;	
	int x=0;
	int y=0;	
	int k=0;	
	
	printf("\n==============LOG_ARRAY_COORDINATE===============\n");	
	for(j=0; j<VERTICAL_COORDINATE; j++)
	{
		for(i=0; i<HORIZONTAL_COORDINATE; i++)
		{
			//x=ARRAY_COORDINATE[i][j] & B8BIT;
			//y=(ARRAY_COORDINATE[i][j] & B16BIT)>>8;		
			printf("0x%04x ",ARRAY_COORDINATE[j][i]);	
			//printf("(%d,%d) ",x,y);				
		}
		printf("\n");		
	}	
	return;
}
	
//========================GetCoordinate====================//
void GetCoordinate(void)
{
	int i=0;
	int j=0;	
	int x=0;
	int y=0;
	int k = 0;	
	int temp=0;

	memset(ARRAY_COORDINATE, HORIZONTAL_COORDINATE*VERTICAL_COORDINATE*sizeof(int), 0x0);	
	for(j=0; j<VERTICAL_COORDINATE; j++)
	{
		for(i=0; i<HORIZONTAL_COORDINATE; i++)
		{
			x=0+i*MACRO;
			y=0+j*MACRO;
			temp=((y & B8BIT)<<8)|(x & B8BIT);
			ARRAY_COORDINATE[j][i]=temp;
			k++;
		}
		printf("\n");		
	}
	printf("\nk=%d\n",k);		
	LOG_ARRAY_COORDINATE();
	return;
}
//========================LOG_ARRAY_LUMA====================//
void LOG_ARRAY_LUMA(void)
{
	int i=0;
	int j=0;	

	printf("\n==============LOG_ARRAY_LUMA===============\n");	
	for(j=0; j<VERTICAL_COORDINATE; j++)
	{
		for(i=0; i<HORIZONTAL_COORDINATE; i++)
		{	
			printf("%02x ",ARRAY_LUMA[j][i]);	
		}
		printf("\n");		
	}	
	return;
}
//========================AverageLuminance====================//
int AverageLuminance(int x_start, int x_end, int y_start, int y_end)
{
	int i=0;
	int j=0;	
	int sum=0;	
	int ave=0;		
	
	for(j=y_start; j<y_end; j++)
	{
		for(i=x_start; i<x_end; i++)
		{	
			sum+=ARRAY_Y[j][i];
		}
	}
	ave=sum/(MACRO*MACRO);
#if 0
	for(j=0; j<7; j++)
	{
		for(i=0; i<7; i++)
		{	
			sum+=ARRAY_Y[j][i];
		}
	}
#endif	
	printf("\n(%d~%d;%d~%d)=(%d,%d)",x_start,x_end,y_start,y_end, sum, ave);	
	return ave;
}
//========================SetLuminance====================//
void SetLuminance(void)
{
	int i=0;
	int j=0;	
	int x_start=0;
	int y_start=0;	
	int x_end=0;
	int y_end=0;		
	int k=0;	
	
	printf("\n==============SetLuminance===============\n");	
	for(j=0; j<VERTICAL_COORDINATE; j++)
	{
		for(i=0; i<HORIZONTAL_COORDINATE; i++)
		{	
			x_start=ARRAY_COORDINATE[j][i] & B8BIT;
			y_start=(ARRAY_COORDINATE[j][i] & B16BIT)>>8;	
			x_end=x_start+MACRO;
			y_end=y_start+MACRO;
			//printf("\n(%d~%d;%d~%d)",x_start,x_end,y_start,y_end);				
			ARRAY_LUMA[k/HORIZONTAL_COORDINATE][k%HORIZONTAL_COORDINATE]=AverageLuminance(x_start, x_end, y_start, y_end);
			k++;
		}
		printf("\n");		
	}	
	LOG_ARRAY_LUMA();
	return;
}
//========================GetTotalAverage====================//
void GetTotalAverage(void)
{
	int i=0;
	int j=0;	
	int k=0;		
	int sum=0;	
	int ave=0;	

	printf("\n==============GetTotalAverage===============\n");	
	for(j=0; j<VERTICAL_COORDINATE; j++)
	{
		for(i=0; i<HORIZONTAL_COORDINATE; i++)
		{	
			sum+=ARRAY_LUMA[j][i];
		}
		printf("\n");		
	}	
	AverageValue.TotalAverage=sum/(VERTICAL_COORDINATE*HORIZONTAL_COORDINATE);
	sum=0;	
	k=0;
	for(j=0; j<VERTICAL_COORDINATE; j++)
	{
		for(i=0; i<HORIZONTAL_COORDINATE; i++)
		{	
			if(ARRAY_LUMA[j][i]>=AverageValue.TotalAverage)
			{
				sum+=ARRAY_LUMA[j][i];
				k++;
			}
		}
		printf("\n");		
	}	
	AverageValue.MaxTotalAverage=sum/k;
	sum=0;
	k=0;
	for(j=0; j<VERTICAL_COORDINATE; j++)
	{
		for(i=0; i<HORIZONTAL_COORDINATE; i++)
		{	
			if(ARRAY_LUMA[j][i]<AverageValue.TotalAverage)
			{
				sum+=ARRAY_LUMA[j][i];
				k++;
			}
		}
		printf("\n");		
	}	
	AverageValue.MinTotalAverage=sum/k;	
	printf("\n AverageValue=%d,%d,%d\n", AverageValue.TotalAverage, AverageValue.MaxTotalAverage, AverageValue.MinTotalAverage);	
	return;
}
//========================LOG_SEEDS====================//
void LOG_SEEDS(void)
{
	int i=0;

	printf("\n==============LOG_SEEDS===============\n");	
	for(i=0; i<SEED_NUM; i++)
	{
		printf("\n(%d %d 0x%02x)", Seeds[i].x, Seeds[i].y, Seeds[i].value);		
	}	
	return;
}
//========================LOG_SEEDSMACRO====================//
void LOG_SEEDSMACRO(void)
{
	int i=0;

	printf("\n==============LOG_SEEDS===============\n");	
	for(i=0; i<SEED_NUM; i++)
	{
		printf("\n(%d %d 0x%02x)", SeedsMacro[i].x, SeedsMacro[i].y, SeedsMacro[i].value);		
	}	
	return;
}

//========================ThirdSeed====================//
void ThirdAllSeed(void)
{
	int i=0;
	int j=0;	
	int Max=ARRAY_Y[0][0];	
	int Min=ARRAY_Y[0][0];	
	
	printf("\n==============ThirdAllSeed===============\n");	
	memset(Seeds, SEED_NUM*sizeof(seed), 0x0);	
	Seeds[0].value=ARRAY_Y[0][0];//max
	Seeds[3].value=ARRAY_Y[0][0];//min
	
	for(j=0; j<HEIGHT; j++)
	{
		for(i=0; i<WIDTH; i++)
		{	
			if(ARRAY_Y[j][i]>Max)
			{
				Seeds[0].x=j;
				Seeds[0].y=i;
				Max=ARRAY_Y[j][i];
				Seeds[0].value=ARRAY_Y[j][i];
			}
			if(ARRAY_Y[j][i]<Min)
			{
				Seeds[3].x=j;
				Seeds[3].y=i;
				Min=ARRAY_Y[j][i];
				Seeds[3].value=ARRAY_Y[j][i];
			}			
		}
	}		
	LOG_SEEDS();
	return;
}

//========================ThirdSeed====================//
void ThirdSeed(void)
{
	int i=0;
	int j=0;	
	int Max=ARRAY_LUMA[0][0];	
	int Min=ARRAY_LUMA[0][0];	
	
	printf("\n==============ThirdSeed===============\n");	
	memset(SeedsMacro, SEED_NUM*sizeof(seed), 0x0);	
	SeedsMacro[0].value=ARRAY_LUMA[0][0];//max
	SeedsMacro[3].value=ARRAY_LUMA[0][0];//min
	
	for(j=0; j<VERTICAL_COORDINATE; j++)
	{
		for(i=0; i<HORIZONTAL_COORDINATE; i++)
		{	
			if(ARRAY_LUMA[j][i]>Max)
			{
				SeedsMacro[0].x=j;
				SeedsMacro[0].y=i;
				Max=ARRAY_LUMA[j][i];
				SeedsMacro[0].value=ARRAY_LUMA[j][i];
			}
			if(ARRAY_LUMA[j][i]<Min)
			{
				SeedsMacro[3].x=j;
				SeedsMacro[3].y=i;
				Min=ARRAY_LUMA[j][i];
				SeedsMacro[3].value=ARRAY_LUMA[j][i];
			}			
		}
	}	
	Max=ARRAY_LUMA[0][0];	
	Min=ARRAY_LUMA[0][0];		
	SeedsMacro[1].value=ARRAY_LUMA[0][0];//max
	SeedsMacro[4].value=ARRAY_LUMA[0][0];//min	
	for(j=0; j<VERTICAL_COORDINATE; j++)
	{
		for(i=0; i<HORIZONTAL_COORDINATE; i++)
		{	
			if((ARRAY_LUMA[j][i]>Max) && !(SeedsMacro[0].x==j && SeedsMacro[0].y==i))
			{
				SeedsMacro[1].x=j;
				SeedsMacro[1].y=i;
				Max=ARRAY_LUMA[j][i];
				SeedsMacro[1].value=ARRAY_LUMA[j][i];
			}
			if((ARRAY_LUMA[j][i]<Min) && !(SeedsMacro[3].x==j && SeedsMacro[3].y==i))
			{
				SeedsMacro[4].x=j;
				SeedsMacro[4].y=i;
				Min=ARRAY_LUMA[j][i];
				SeedsMacro[4].value=ARRAY_LUMA[j][i];
			}			
		}
	}	
	Max=ARRAY_LUMA[0][0];	
	Min=ARRAY_LUMA[0][0];		
	SeedsMacro[2].value=ARRAY_LUMA[0][0];//max
	SeedsMacro[5].value=ARRAY_LUMA[0][0];//min	
	for(j=0; j<VERTICAL_COORDINATE; j++)
	{
		for(i=0; i<HORIZONTAL_COORDINATE; i++)
		{	
			if((ARRAY_LUMA[j][i]>Max) && !(SeedsMacro[0].x==j && SeedsMacro[0].y==i) && !(SeedsMacro[1].x==j && SeedsMacro[1].y==i))
			{
				SeedsMacro[2].x=j;
				SeedsMacro[2].y=i;
				Max=ARRAY_LUMA[j][i];
				SeedsMacro[2].value=ARRAY_LUMA[j][i];
			}
			if((ARRAY_LUMA[j][i]<Min) && !(SeedsMacro[3].x==j && SeedsMacro[3].y==i) && !(SeedsMacro[4].x==j && SeedsMacro[4].y==i))
			{
				SeedsMacro[5].x=j;
				SeedsMacro[5].y=i;
				Min=ARRAY_LUMA[j][i];
				SeedsMacro[5].value=ARRAY_LUMA[j][i];
			}			
		}
	}	
	LOG_SEEDSMACRO();
	return;
}
//========================LOG_SEEDS_ARRAY====================//
void LOG_FILTERS_ARRAY(void)
{
	int i=0;
	int j=0;		
	int k=0;
	printf("\n==============LOG_FILTERS_ARRAY===============\n");	
	for(k=0; k<SEED_NUM; k++)
	{
		printf("\n==============ARRAY_Difference[%d]===============\n",k);			
		for(j=0; j<VERTICAL_COORDINATE; j++)
		{
			for(i=0; i<HORIZONTAL_COORDINATE; i++)
			{	
				printf("%04d ",Filters[k].ARRAY_Difference[j][i]);	
			}
			printf("\n");		
		}	
	}	
	return;
}
//========================LOG_FILTERS_AVE_ARRAY====================//
void LOG_FILTERS_AVE_ARRAY(void)
{
	int i=0;
	int j=0;		
	int k=0;
	printf("\n==============LOG_FILTERS_AVE_ARRAY===============\n");	
	for(k=0; k<SEED_NUM; k++)
	{
		printf("\n==============AverageFilter[%d]===============\n",k);			
		for(j=0; j<HEIGHT; j++)
		{
			for(i=0; i<WIDTH; i++)
			{	
				printf("%d ",Filters[k].AverageFilter[j][i]);	
			}
			printf("\n");		
		}	
	}	
	return;
}
//========================DifferenceCalculation====================//
void DifferenceCalculation(void)
{
	int i=0;
	int j=0;		
	int k=0;
	memset(Filters, SEED_NUM*sizeof(filter), 0x0);	
	for(k=0;k<SEED_NUM;k++)
	{
		for(j=0; j<HEIGHT; j++)
		{
			for(i=0; i<WIDTH; i++)
			{			
				Filters[k].ARRAY_Difference[j][i]=Seeds[k].value-ARRAY_LUMA[j][i];
			}
		}
	}
	LOG_FILTERS_ARRAY();	
	return;
}
//========================TotalAveageFilter====================//
void TotalAveageFilter(void)
{
	int i=0;
	int j=0;		
	int k=0;
	for(k=0; k<SEED_NUM/2; k++)
	{
		for(j=0; j<VERTICAL_COORDINATE; j++)
		{
			for(i=0; i<HORIZONTAL_COORDINATE; i++)
			{	
				if(Filters[k].ARRAY_Difference[j][i] >= AverageValue.MaxTotalAverage)
					Filters[k].AverageFilter[j][i]=0;
				else
					Filters[k].AverageFilter[j][i]=1;
			}
		}	
		Filters[k].AverageFilter[SeedsMacro[k].x][SeedsMacro[k].y]=2;
	}	
	
	
	for(k=SEED_NUM/2; k<SEED_NUM; k++)
	{
		for(j=0; j<VERTICAL_COORDINATE; j++)
		{
			for(i=0; i<HORIZONTAL_COORDINATE; i++)
			{	
				if(Filters[k].ARRAY_Difference[j][i] <= AverageValue.MinTotalAverage)
					Filters[k].AverageFilter[j][i]=0;
				else
					Filters[k].AverageFilter[j][i]=1;
			}
		}
		Filters[k].AverageFilter[SeedsMacro[k].x][SeedsMacro[k].y]=2;
	}		
	LOG_FILTERS_AVE_ARRAY();		
	return;
}
//========================DistanceFilter====================//
void DistanceFilter(void)//not complete
{
	int i=0;	
	int Radius=0;
	Radius=(HORIZONTAL_COORDINATE+VERTICAL_COORDINATE)/2;
	for(i=0;i<SEED_NUM;i++)
	{
		Filters[i].enable=1;
	}	
	Filters[0].enable=1;
	Filters[1].enable=0;
	Filters[2].enable=1;
	Filters[3].enable=1;
	Filters[4].enable=0;
	Filters[5].enable=0;	
	return;
}
//========================SyntheticEvaluation====================//
void SyntheticEvaluation(void)
{
	TotalAveageFilter();
	DistanceFilter();//not complete
	return;
}
//========================DynamicRangeLuma====================//
int GetModificationCoefficient(int num, int location)
{
	int x = 0;
	int y = 0;	
	int sum = 0.0;	
	double distance = 0.0;
	x=location/WIDTH;
	y=location%WIDTH;
	sum = (float)(abs(x - Seeds[num].x)*abs(x - Seeds[num].x)) + (float)(abs(y - Seeds[num].y)*abs(y - Seeds[num].y));
	distance = sqrt(sum);
	//printf("(%d,%d,%d,%f) ",x,y,sum,distance);
	if(1==Filters[num].AverageFilter[x][y])
	{
		return (int)distance;
	}
	else if(0==Filters[num].AverageFilter[x][y])
	{
		return (int)(0-distance);
	}	
}
//========================DynamicRangeLuma====================//
void DropStone(int num)
{
	FILE *fp1;
	FILE *fp2;
	int k = 0;
	int temp = 0;
	int Coefficient=0;
	int CoefficientTest=0;	
	char file_name[30]={0};
	sprintf(file_name, "176X144_BALL_%d.y\n", num); 

	if((fp1 = fopen("176X144_BALL.y","rb")) == NULL)
		printf("eror fp1 open\n");
	if((fp2 = fopen(file_name,"wb")) == NULL)
		printf("eror fp2 open\n");

	printf("\nfile_name=%s\n", file_name);

	while(!feof(fp1))
	{
		temp = fgetc(fp1) & B8BIT;
		Coefficient=GetModificationCoefficient(num, k);
		if(Coefficient!=0)
		{
			CoefficientTest=WIDTH/Coefficient;
			printf("%d->",temp);
			if(Coefficient>0)
			{
				if((temp+CoefficientTest)<255)
				{
					temp+=CoefficientTest;
				}
				else
				{
					temp=255;
				}
			}
			if(Coefficient<0)
			{
				if((temp+CoefficientTest)>0)
				{
					temp+=CoefficientTest;
				}
				else
				{
					temp=0;
				}
			}
		}
#if 0		
		if(temp>255)
			temp=255;
		else if(temp<0)
			temp=0;	
#endif	
		printf("%d ",temp);
		fputc(temp, fp2);
		k++;
	}
	printf("\nk=%d\n",k);
	fclose(fp1);
	fclose(fp2);	
}
//========================DynamicRangeLuma====================//
void DynamicRangeLuma(void)
{
	int i=0;
	//for(i=0;i<SEED_NUM;i++)
	{
		if(Filters[i].enable)
		{
			DropStone(i);
		}
	}
	return;
}
//========================CombinationNewFile====================//
void CombinationNewFile(void)
{
	FILE *fp1;
	FILE *fp2;
	FILE *fp3;
	
	if((fp1 = fopen("0.y","rb")) == NULL)
		printf("eror fp1 open\n");	

	if((fp2 = fopen("176X144_BALL.uv","rb")) == NULL)
		printf("eror fp2 open\n");
		
	if((fp3 = fopen("0.yuv","wb")) == NULL)
		printf("eror fp3 open\n");	

		
	while(!feof(fp1))
	{
		fputc(fgetc(fp1), fp3);
	}
	while(!feof(fp2))
	{
#if 1
		fputc(fgetc(fp2), fp3);
#else
		fgetc(fp2);
		fputc(128, fp3);
#endif		
	}	
	
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
	return;
}
//========================Main====================//
void main()
{
#if 1
	Y2File();
	Y2Array();
//===================1===================//		
	ThirdAllSeed();
#if 0
	GetCoordinate();
	SetLuminance();	
//===================2===================//	
	GetTotalAverage();		
	ThirdSeed();	
	DifferenceCalculation();
	SyntheticEvaluation();
#endif	
	GetTotalAverage();		
	DifferenceCalculation();
	SyntheticEvaluation();
//===================3===================//	
	DynamicRangeLuma();
#else
	CombinationNewFile();
#endif
	return;
}
