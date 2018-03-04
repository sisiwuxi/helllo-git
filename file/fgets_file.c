#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_LINE 1024
#define MAX_FILE_LEN 10
#define ERROR printf
#define DEBUG //printf
typedef enum 
{
    false, true
} bool;

typedef enum
{ 
    ACTION_NULL = 0,
    ACTION_START_FROM_LOCATION,
    ACTION_END_OF_LOCATION,
    ACTION_NUM,
}ActionAfterFind;

typedef struct
{ 
    char *file_in_name;
    char *file_out_name;
    char *str_first;
    ActionAfterFind AAF;
}GrepStrInFile;



int find_first_location_in_string(char *str, char *find_str)
{
    int len_buf=0, len_find_str=0;
    int i,j,location=0;

    len_buf=strlen(str);
    len_find_str=strlen(find_str);
    if(len_buf<len_find_str)
    {
        ERROR("len_find_str is longer than len_buf, input again please\n");
        return 0;
    }
    else
    {
        for(i=0; i<len_buf; i++)
        {
            if(find_str[0] == str[i])
            {
                location = i;
                for(j=0; j<len_find_str; j++)
                {
                    if(find_str[j] == str[location+j])
                    {}
                    else
                    {
                        break;
                    }
                }
                if(j == len_find_str)
                {
                    DEBUG("match in %d\n",location);
                    break;
                }
            }
        }
        //location += 1;
        if(i < len_buf)
        {
            DEBUG("first match in %d\n",location);
            DEBUG("%s\n",(char *)(str+location));
            //strcpy(buf_first, (const char *)(buf+location));
            return location;
        }
        else
        {
	    DEBUG("can't match,try again?\n");
            return 0;
        }
    }
}
int grep_str_in_file(GrepStrInFile *GSIF)
{
    char str[MAX_LINE],str1[MAX_LINE]={0};

    FILE *fp, *fp_out;
    int len, location=0;
    if((fp=fopen(GSIF->file_in_name,"r"))==NULL)
    {
        ERROR("fail to read %s", GSIF->file_in_name);
        exit(1);
    }   
    if((fp_out=fopen(GSIF->file_out_name,"wr"))==NULL)
    {
        ERROR("fail to write %s", GSIF->file_out_name);
        exit(1);
    }   
    while(fgets(str,MAX_LINE,fp)!=NULL)
    {
        len=strlen(str);
        str[len-1]='\0';
        DEBUG("%s %d\n",str,(len-1));
        DEBUG("%d ",(len-1));
        location = find_first_location_in_string(str, GSIF->str_first);
        DEBUG("%d ",location);
        if(location > 0)
        {
            if(ACTION_START_FROM_LOCATION == GSIF->AAF)
            {
                //strcpy(str1,(const char *)(str+location));
                //DEBUG("%s\n",(char *)str1);
                fputs((const char *)(str+location),fp_out);
                fputc(0x0D,fp_out);fputc(0x0A,fp_out);
            }
            else if(ACTION_END_OF_LOCATION == GSIF->AAF)
            {
                location += strlen(GSIF->str_first);
                strncpy(str1,(const char *)str,location);
                str1[location]='\0';
                DEBUG("%s\n",(char *)str1);
                fputs((const char *)(str1),fp_out);
                fputc(0x0D,fp_out);fputc(0x0A,fp_out);
            }
        }
        location = 0;
    }
    if(fp)
    {
        fclose(fp);
    }
    if(fp_out)
    {
        fclose(fp_out);
    }
    return 0;
}

void delete_repeating_line(char *input_file_name, char *output_file_name)
{
//not finish, next time use Linked list
#if 0
typedef struct node
{
    char filename[20];
    struct node *link;
};
    FILE *fp_in=NULL, *fp_out=NULL;
    char str_in[MAX_LINE]={0}, str_out[MAX_LINE]={0};
    bool findout = false;
  
    if((fp_in=fopen(input_file_name,"r"))==NULL)
    {
        ERROR("fail to read %s", input_file_name);
        exit(1);
    }    
    if((fp_out=fopen(output_file_name,"wr"))==NULL)
    {
        ERROR("fail to write %s", output_file_name);
        exit(1);
    }
 
    while(fgets(str_in,MAX_LINE,fp_in)!=NULL)
    {

#if 0
        findout=true;
        //printf("[%s-%s,%d]\n",str_in, str_out, findout);
        fseek(fp_out, 0, SEEK_SET);
        while(fgets(str_out,MAX_LINE,fp_out)!=NULL)
        {
            printf("[%s-%s,%d]\n",str_in, str_out, findout);
	    if(!strcmp(str_in, str_out))
            {
                findout=true;
                break;
            }
            else
            {
                findout=false;
            }
        }  
        if(false==findout)  
        {
            fputs((const char *)(str_in),fp_out);
            fputc(0x0D,fp_out);fputc(0x0A,fp_out);
】        }

#endif
    }
    if(fp_in)
    {
        fclose(fp_in);
    }
    if(fp_out)
    {
        fclose(fp_out);
        fp_out=NULL;
    }
#endif
}

int main(void)
{
    GrepStrInFile GSIF;
    char file_in_name[] = "1436.txt";
    char file_out1_name[] = "1436_[gst.txt";
    char file_out2_name[] = "1436_[gst_c.txt";
    char file_out3_name[] = "1436_[gst_c_del.txt";
    char str_first[]="[gst";
    char str_second[]=".c";
    /*==============start_from_[gst==============*/
    GSIF.file_in_name = file_in_name;
    GSIF.file_out_name = file_out1_name;
    GSIF.str_first = str_first;
    GSIF.AAF = ACTION_START_FROM_LOCATION;
    grep_str_in_file(&GSIF);
    /*==============end_of_.c==============*/
    GSIF.file_in_name = file_out1_name;
    GSIF.file_out_name = file_out2_name;
    GSIF.str_first = str_second;
    GSIF.AAF = ACTION_END_OF_LOCATION;
    grep_str_in_file(&GSIF);
    /*==============end_of_.c==============*/
    delete_repeating_line(file_out2_name, file_out3_name);
}
