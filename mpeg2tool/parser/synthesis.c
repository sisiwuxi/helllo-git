#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define BIT7 0x080
#define BIT6 0x040
#define BIT5 0x020
#define B5BIT 0x01f
#define B76BIT 0x0c0
#define B54BIT 0x030
#define B4BIT 0x0f
#define B12345BIT 0x3e
#define B0BIT 0x01
#define B8BIT ((~(~0))|0xff)
#define BL4BIT ((~(~0))|0x0f)
#define BH4BIT ((~(~0))|0xf0)
#define MAX_INFO_LEN 50

typedef struct tagCONFIGINFO
{
    char Header[256];
    char Item[256];
    char Size[256];
    int loop;
    long total_size;
} CONFIGINFO;


typedef struct tagFILEINFO
{
    char Header[256];
    char sPos[256];
    char sSize[256];
    char path[256];
    long pos;
    long size;
} FILEINFO;

void main()
{
    FILE *fp1;
    FILE *fp2;
    FILE *fp3;
    int i = 0;
    int k = 0;
    int temp;
    int H4temp;
    int L4temp;
    CONFIGINFO ConfigInf;
    FILEINFO FileInfo;

    if((fp1 = fopen("Config.ini","rb")) == NULL)
        printf("eror fp1 open\n");
    if((fp2 = fopen("images.bin","wb")) == NULL)
        printf("eror fp2 open\n");

//================read item number=============
    memset(&ConfigInf, 0, sizeof(ConfigInf));
    fseek(fp1, 0, SEEK_SET);
    fgets(ConfigInf.Header, MAX_INFO_LEN, fp1);//从流中读取n-1个字符，除非读完一行
    printf("\n%s",ConfigInf.Header);
    temp = ftell(fp1);

    fseek(fp1, temp, SEEK_SET);
    fgets(ConfigInf.Item, MAX_INFO_LEN, fp1);
    printf("%s",ConfigInf.Item);
    temp = ftell(fp1);

    fseek(fp1, temp, SEEK_SET);
    fgets(ConfigInf.Size, MAX_INFO_LEN, fp1);
    printf("%s",ConfigInf.Size);
    temp = ftell(fp1);

    ConfigInf.loop = ConfigInf.Item[strlen(ConfigInf.Item) - 2] - 48;//last character
    //printf("%d",ConfigInf.loop);

    char temp_size[256] = {0};
    strcpy(temp_size, ConfigInf.Size + 7);//size=0x
    //printf("%s",temp_size);
    int len = strlen(temp_size) - 1;
    for(i = 0; i < len; i++)
    {
        if(temp_size[i]  >= 48 && temp_size[i] <= 58)
        {
            temp_size[i] -= 48;
        }
        else if(temp_size[i]  >= 65 && temp_size[i] <= 88)
        {
            temp_size[i] -= 55;
        }
        ConfigInf.total_size += (long)((long)temp_size[i] * (long)pow(16, (len - 1 - i)));
        //printf("\n%d %d %ld %ld",temp_size[i], (len - 1 - i), (long)pow(16, (len - 1 - i)), (long)ConfigInf.total_size);
    }
    printf("%ld",(long)ConfigInf.total_size);
    for(long j = 0; j < ConfigInf.total_size; j++)
    {
        fputc(0xFF, fp2);
    }
    printf("\n========================================================\n");
//=============obtain each info===============
    for(i = 0; i < ConfigInf.loop; i++)
    {
        printf("\n***************************%d*****************************\n", i);
        memset(&FileInfo, 0, sizeof(FileInfo));
        fseek(fp1, temp, SEEK_SET);//for 0D 0A
        fgets(FileInfo.Header, MAX_INFO_LEN, fp1);
        temp = ftell(fp1);

        fseek(fp1, temp, SEEK_SET);
        fgets(FileInfo.Header, MAX_INFO_LEN, fp1);
        printf("%s",FileInfo.Header);
        temp = ftell(fp1);

        fseek(fp1, temp, SEEK_SET);
        fgets(FileInfo.sPos, MAX_INFO_LEN, fp1);
        printf("%s",FileInfo.sPos);
        temp = ftell(fp1);

        fseek(fp1, temp, SEEK_SET);
        fgets(FileInfo.sSize, MAX_INFO_LEN, fp1);
        printf("%s",FileInfo.sSize);
        temp = ftell(fp1);

        fseek(fp1, temp, SEEK_SET);
        fgets(FileInfo.path, MAX_INFO_LEN, fp1);
        printf("%s",FileInfo.path);
        temp = ftell(fp1);

        char temp_size[256] = {0};
        strcpy(temp_size, FileInfo.sPos + 6);//pos=0x
        //printf("%s",temp_size);
        int len = strlen(temp_size) - 1;
        for(k = 0; k < len; k++)
        {
            if(temp_size[k]  >= 48 && temp_size[k] <= 58)
            {
                temp_size[k] -= 48;
            }
            else if(temp_size[k]  >= 65 && temp_size[k] <= 88)
            {
                temp_size[k] -= 55;
            }
            FileInfo.pos += (long)((long)temp_size[k] * (long)pow(16, (len - 1 - k)));
            //printf("\n%d %d %ld %ld",temp_size[k], (len - 1 - i), (long)pow(16, (len - 1 - k)), (long)ConfigInf.total_size);
        }
        printf("%ld\n",(long)FileInfo.pos);

        temp_size[256] = 0;
        strcpy(temp_size, FileInfo.sSize + 7);//size=0x
        //printf("%s",temp_size);
        len = strlen(temp_size) - 1;
        for(k = 0; k < len; k++)
        {
            if(temp_size[k]  >= 48 && temp_size[k] <= 58)
            {
                temp_size[k] -= 48;
            }
            else if(temp_size[k]  >= 65 && temp_size[k] <= 88)
            {
                temp_size[k] -= 55;
            }
            FileInfo.size += (long)((long)temp_size[k] * (long)pow(16, (len - 1 - k)));
            //printf("\n%d %d %ld %ld",temp_size[i], (len - 1 - i), (long)pow(16, (len - 1 - k)), (long)ConfigInf.total_size);
        }
        printf("%ld",(long)FileInfo.size);
        //char temp_path[256] = "images/uImage.lzo";
        char temp_path[256] = {0};
        if(i == (ConfigInf.loop - 1))
        {
            strncpy(temp_path, FileInfo.path + 5, (strlen(FileInfo.path) - 5));
        }
        else
        {
            strncpy(temp_path, FileInfo.path + 5, (strlen(FileInfo.path) - 6));
        }
        printf("\ntemp_path=%s\n", temp_path);
        if((fp3 = fopen(temp_path,"rb")) == NULL)
        {
            printf("eror fp3 open\n");
        }
        else
        {
            fseek(fp2, FileInfo.pos, SEEK_SET);
            while(!feof(fp3))
            {
                fputc(fgetc(fp3), fp2);
            }
            fclose(fp3);
        }

    }
    /*
    while(!feof(fp1))
    {
    	temp = fgetc(fp1) & B8BIT;
    	fputc(0x30, fp2);
    	fputc(0x78, fp2);
    	H4temp = (temp >> 4) & BL4BIT;
    	L4temp = temp & BL4BIT;

    	if((H4temp >= 0x00) && (H4temp <= 0x09))
    	{
    		fputc((H4temp + 0x30), fp2);
    	}
    	else if((H4temp >= 0x0a) && (H4temp <= 0x0f))
    	{
    		fputc((H4temp + 0x57), fp2);
    	}
    	if((L4temp >= 0x00) && (L4temp <= 0x09))
    	{
    		fputc((L4temp + 0x30), fp2);
    	}
    	else if((L4temp >= 0x0a) && (L4temp <= 0x0f))
    	{
    		fputc((L4temp + 0x57), fp2);
    	}
    	fputc(0x2C, fp2);
    	if(k % 16)
    	{
    		fputc(0x20, fp2);
    	}
    	else
    	{
    		fputc(0x0D, fp2);
    		fputc(0x0A, fp2);
    	}
    	k++;
    }
    */
    fclose(fp1);
    fclose(fp2);
}
