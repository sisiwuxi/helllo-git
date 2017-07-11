/************************************************************/
/* Author : sisi.wu                                         */
/* Date   : 2016/11/22                                       */
/* Email  : pace.pi@mstarsemi.com                           */
/************************************************************/
#include "stdio.h"
#include "edid.h"

void show_help()
{
    printf("\nOptions :\n\n");
    printf("-i input_path -o output_path\n");
    
    printf("\n\n");
    printf("If you meet a BUG or have some good ideas , send Email to sisi.wu@mstarsemi.com\n");
    printf("\n\n");

}

int main(int argc , char* argv[])
{
    int i = 0;
    if(argc == 1)
    {
        show_help();
        return 0;
    }

    for(i = 1; i < argc; i++)
    {
        if(argv[i][0] == '-')
        {
            switch(argv[i][1])
	    {
            case 'i' :
                set_input_path((const char *)argv[++i]);
                break;
            case 'o' :
                set_output_path((const char *)argv[++i]);
                break;
            case 'h' :
		show_help();
                break;
            default :
                show_help();
                break;
            }
        }
    }
printf("\ni=%d",i);
    if(5==i)
    {
printf("\n[%d,%s]",__LINE__,__FUNCTION__);
    	convert();
printf("\n[%d,%s]",__LINE__,__FUNCTION__);
    }
    return 0;
}

