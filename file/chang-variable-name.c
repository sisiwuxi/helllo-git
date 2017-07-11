#include <stdio.h>
#include <string.h>
#define LENGTH 0x5E2FB0//0x1260690//0x2b050

#if 0
#define NAME_LENGTH 21
void main()
{
    FILE *fp1;
    FILE *fp2;

    if((fp1 = fopen("html_8bpp.pkg","rb")) == NULL)
        printf("eror fp1 open\n");
    if((fp2 = fopen("html_8bpp_new.pkg","wb")) == NULL)
        printf("eror fp2 open\n");

    int i = 0;
    int offset = 0;
    int times = 0;
    char temp_name[30] = {0};
    while(!feof(fp1))
    {
        if(offset > LENGTH)
        {
            break;
        }
        fseek(fp1, offset, SEEK_SET);
        for(i=0; i<NAME_LENGTH; i++)
        {
            temp_name[i] = fgetc(fp1);
        }
        if(!strcmp(temp_name, "g_sem_interrupt_start"))
        {
            times++;
            temp_name[1] = 0x30 + (times / 100);
            temp_name[2] = 0x30 + ((times % 100) / 10);
            temp_name[3] = 0x30 + (times % 10);
            for(i=0; i<NAME_LENGTH; i++)
            {
                fputc(temp_name[i], fp2);
            }
            offset += NAME_LENGTH;
        }
        else
        {
            fputc(temp_name[0], fp2);
            offset++;
        }
    }
    fseek(fp1, offset, SEEK_SET);
    while(!feof(fp1))
    {
        fputc(fgetc(fp1), fp2);
    }
    fclose(fp1);
    fclose(fp2);
}
#else
#define NAME_LENGTH 19
void main()
{
    FILE *fp1;
    FILE *fp2;

    if((fp1 = fopen("html_8bpp_new.pkg","rb")) == NULL)
        printf("eror fp1 open\n");
    if((fp2 = fopen("html_8bpp_new_2.pkg","wb")) == NULL)
        printf("eror fp2 open\n");

    int i = 0;
    int offset = 0;
    int times = 0;
    char temp_name[30] = {0};
    while(!feof(fp1))
    {
        if(offset > LENGTH)
        {
            break;
        }
        fseek(fp1, offset, SEEK_SET);
        for(i=0; i<NAME_LENGTH; i++)
        {
            temp_name[i] = fgetc(fp1);
        }
        if(!strcmp(temp_name, "g_sem_interrupt_end"))
        {
            times++;
            temp_name[1] = 0x30 + (times / 100);
            temp_name[2] = 0x30 + ((times % 100) / 10);
            temp_name[3] = 0x30 + (times % 10);
            for(i=0; i<NAME_LENGTH; i++)
            {
                fputc(temp_name[i], fp2);
            }
            offset += NAME_LENGTH;
        }
        else
        {
            fputc(temp_name[0], fp2);
            offset++;
        }
    }
    fseek(fp1, offset, SEEK_SET);
    while(!feof(fp1))
    {
        fputc(fgetc(fp1), fp2);
    }
    fclose(fp1);
    fclose(fp2);
}
#endif