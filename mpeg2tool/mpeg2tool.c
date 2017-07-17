/************************************************************/
/* Author : sisi.wu                                         */
/* Date   : 2017/7/12                                       */
/* Email  : sisi.wu@mstarsemi.com                           */
/************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "include/version.h"
#include "include/common.h"
#include "include/mpeg2tool.h"
#include "include/abend.h"
#include "include/parseTS.h"

void show_version()
{
    INFO("%s\n",version);
}

void show_help()
{
    INFO("\nOptions :\n\n");

    INFO("-i : The path of input TS file\n");
    INFO("-o : The path of output TS file\n");
    INFO("-t : The location you want to start abend, style:00:00:00\n");
    INFO("-a : The duration of abend you want to make, unit:ms\n");
    INFO("-c : concat two TS into one TS; Ex:mpeg2tool -c input1.ts input2.ts output.ts\n");
    INFO("-d : The duration of input TS file, unit:s; Ex:mpeg2tool -i input.ts -d\n");
    INFO("-p : Show all PIDs; Ex:mpeg2tool -i input.ts -p\n");	
    INFO("-T : Run auto test\n");
    INFO("-h : show help\n");	
    INFO("\n\n");
    INFO("If you meet a BUG or have some good ideas , send Email to sisi.wu@mstarsemi.com\n");
    INFO("\n\n");
}

void test_all()
{
    INFO("\n=[test_abend]====mpeg2tool -i input.ts -o input_abend.ts -t 00:00:03 -a 400=============\n");
    MPEG2ToolSetInputPath("input.ts");	
    MPEG2ToolSetOutputPath("input_abend.ts");
    MPEG2ToolSetStartTime("00:00:03");
    MPEG2AbendStream("400");
    INFO("\n=[test_duration]==================mpeg2tool -i input.ts -d========================\n");
    MPEG2ToolSetInputPath("input.ts");
    MPEG2ToolGetDuration();
    INFO("\n=[test_duration]==================mpeg2tool -i input.ts -p========================\n");
    MPEG2ToolSetInputPath("input.ts");
    MPEG2ToolGetPIDs();	
    INFO("\n=[test_concat]==================./mpeg2tool -c input_abend.ts input2.ts concat.ts========================\n");
    MPEG2ToolConcat("input_abend.ts", "input2.ts", "concat.ts");
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

            case 'c' :
                CHECK_ARGS(i, 3, argc);
                MPEG2ToolConcat(argv[2], argv[3], argv[4]);
                break;

            case 'd' :
                //CHECK_ARGS(i, 1, argc);
                MPEG2ToolGetDuration();
                break;
				
            case 'p' :
                //CHECK_ARGS(i, 1, argc);
                MPEG2ToolGetPIDs();
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
    MPEG2ToolDeInit();
    return 0;
}


