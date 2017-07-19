/************************************************************/
/* Author : sisi.wu                                         */
/* Date   : 2017/7/12                                       */
/************************************************************/

#define CHECK_ARGS(cur,need,max) if(cur + need >= max) \
	{ \
		INFO("Bad arguments : Abend -h for help\n"); \
		return(-1); \
	}


