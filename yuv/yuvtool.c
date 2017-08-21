/************************************************************/
/* Author : sisi.wu                                         */
/* Date   : 2017/7/12                                       */
/************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "include/version.h"
#include "include/common.h"

void show_version()
{
    INFO("%s\n",version);
}

void show_help()
{
    INFO("\nOptions :\n\n");
    INFO("-i : The path of input TS file\n");
    INFO("-o : The path of output TS file\n");
    INFO("\n\n");
    INFO("If you meet a BUG or have some good ideas , send Email to sisiwuxi@hotmail.com\n");
    INFO("\n\n");
}

void test_all()
{
    INFO("\n=[new]====yuvtool  -n YUV420 1920 1080=============\n");
}

int main(int argc , char* argv[])
{
    int i = 0;
    if(1 == argc)
    {
        show_help();
        return 0;
    }
    YUVToolInit();	
    for(i = 1; i < argc; i++)
    {
        if(argv[i][0] == '-')
        {
            switch(argv[i][1])
            {
            case 'i' :
                YUVToolSetInputPath(argv[++i]);
                break;

            case 'o' :
                YUVToolSetOutputPath(argv[++i]);
                break;
				
            case 'n' :
                YUVToolNewFile(argv[2], argv[3], argv[4], argv[5], argv[6], argv[7]);
                break;

            case 'v' :
                show_version();
                break;

            case 'T' :
                test_all();
                break;

            case 'h' :
            default :
                show_help();
                break;
            }
        }
    }
    YUVToolDeInit();	
    return 0;
}


