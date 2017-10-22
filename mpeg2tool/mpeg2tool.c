/************************************************************/
/* Author : sisi.wu                                         */
/* Date   : 2017/7/12                                       */
/************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "include/version.h"
#include "include/common.h"
#include "include/mpeg2tool.h"
#include "include/abend.h"
#include "include/parseTS.h"

#define DBG_LEVEL TOOL_DBG_ERR

void show_version()
{
    DBG_INFO("%s\n",version);
}

void show_help()
{
    DBG_INFO("\nOptions :\n\n");

    DBG_INFO("-i : The path of input TS file\n");
    DBG_INFO("-o : The path of output TS file\n");
    DBG_INFO("-t : The location you want to start abend, style:00:00:00\n");
    DBG_INFO("-a : The duration of abend you want to make, unit:ms\n");
    DBG_INFO("-c : concat two TS into one TS; Ex:mpeg2tool -c input1.ts input2.ts output.ts\n");
    DBG_INFO("-d : The duration of input TS file, unit:s; Ex:mpeg2tool -i input.ts -d\n");
    DBG_INFO("-p : Show all PIDs; Ex:mpeg2tool -i input.ts -p > 1.txt\n");
    DBG_INFO("-s : Add SEI for HLG / BT2020; Ex:mpeg2tool -i input.ts -o output.ts -s\n");
    DBG_INFO("-T : Run auto test\n");
    DBG_INFO("-h : show help\n");
    DBG_INFO("\n\n");
    DBG_INFO("If you meet a BUG or have some good ideas , send Email to sisiwuxi@hotmail.com\n");
    DBG_INFO("\n\n");
}

void test_all()
{
    DBG_INFO("\n=[test_abend]====mpeg2tool -i input.ts -o input_abend.ts -t 00:00:03 -a 400 0=============\n");
    MPEG2ToolSetInputPath("input.ts");
    MPEG2ToolSetOutputPath("input_abend.ts");
    MPEG2ToolSetStartTime("00:00:03");
    MPEG2AbendStream("400", "0");
    DBG_INFO("\n=[test_duration]==================mpeg2tool -i input.ts -d========================\n");
    MPEG2ToolSetInputPath("input.ts");
    MPEG2ToolGetDuration();
    DBG_INFO("\n=[test_parse]==================mpeg2tool -i input.ts -p 0========================\n");
    MPEG2ToolSetInputPath("input.ts");
    MPEG2ToolParser((char *)"0");
    DBG_INFO("\n=[test_concat]==================./mpeg2tool -c input_abend.ts input2.ts concat.ts========================\n");
    MPEG2ToolConcat("input_abend.ts", "input2.ts", "concat.ts");
    DBG_INFO("\n=[test_concat]==================./mpeg2tool -i input.ts -o output.ts -s========================\n");
    MPEG2ToolSetInputPath("input.ts");
    MPEG2ToolSetOutputPath("output.ts");
    MPEG2AddSEI();
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
                CHECK_ARGS(i, 2, argc);
                MPEG2AbendStream(argv[i+1], argv[i+2]);
                break;

            case 'c' :
                CHECK_ARGS(i, 3, argc);
                MPEG2ToolConcat(argv[i+1], argv[i+2], argv[i+3]);
                break;

            case 'd' :
                //CHECK_ARGS(i, 1, argc);
                MPEG2ToolGetDuration();
                break;

            case 'p' :
                CHECK_ARGS(i, 1, argc);
                MPEG2ToolParser(argv[++i]);
                break;

            case 'v' :
                show_version();
                break;

            case 's' ://Add SEI for HLG / BT.2020
                printf("\n[%d,%d]\n", i, argc);
                CHECK_ARGS(i, 0, argc);
                MPEG2AddSEI();
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


