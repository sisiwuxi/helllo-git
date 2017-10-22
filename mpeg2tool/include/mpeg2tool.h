/************************************************************/
/* Author : sisi.wu                                         */
/* Date   : 2017/7/12                                       */
/************************************************************/

#define CHECK_ARGS(cur,need,max) if(cur + need >= max) \
	{ \
		DBG_ERR("Bad arguments : Abend -h for help\n"); \
		return(-1); \
	}


