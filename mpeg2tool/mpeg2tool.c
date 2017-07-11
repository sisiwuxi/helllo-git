/************************************************************/
/* Author : sisi.wu                                         */
/* Date   : 2017/7/12                                       */
/* Email  : sisi.wu@mstarsemi.com                           */
/************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "version.h"
#include "common.h"
#include "mpeg2tool.h"
#include "abend.h"

void show_version()
{
    printf("%s\n",version);
}

void show_help()
{
    INFO("\nOptions :\n\n");

    INFO("-i : The path of input TS file\n");
    INFO("-o : The path of output TS file\n");
    INFO("-t : The location you want to start abend, style:00:00:00\n");
    INFO("-a : The duration of abend you want to make, unit:ms\n");
    INFO("-h : Ex:mpeg2tool -i input.ts -o output.ts -t 00:00:03 -a 400\n");
//mpeg2tool -i 1163_2101_2108_590654828_615647547.ts -o 1163_2101_2108_590654828_615647547_abend.ts -t 00:00:03 -a 400
    INFO("\n\n");
    INFO("If you meet a BUG or have some good ideas , send Email to sisi.wu@mstarsemi.com\n");
    INFO("\n\n");
}

int main(int argc , char* argv[])
{
    int i = 0;

    if(1 == argc)
    {
        show_help();
        return 0;
    }
    MPEG2ToolInit();
    for(i = 1; i < argc; i++)
    {
        if(argv[i][0] == '-')
        {
            switch(argv[i][1])
            {
            case 'i' :
                CHECK_ARGS(i, 1, argc);
                MPEG2ToolSetInputPath(argv[++i]);
                break;
		
            case 'o' :
                CHECK_ARGS(i, 1, argc);
                MPEG2ToolSetOutputPath(argv[++i]);
                break;

            case 't' :
                CHECK_ARGS(i, 1, argc);
                MPEG2ToolSetStartTime(argv[++i]);
                break;

            case 'a' :
                CHECK_ARGS(i, 1, argc);
                MPEG2AbendStream(argv[++i]);
                break;

            case 'v' :
                show_version();
                break;

            case 'h' :
            default :
                show_help();
                break;
            }
        }
    }
    MPEG2ToolDeInit();
    return 0;
}


