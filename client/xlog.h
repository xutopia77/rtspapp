
#ifndef _XLOG_H
#define _XLOG_H

#include <stdlib.h>
#include <stdio.h>


#define LOG_WARN	printf
#define LOG_INFO	printf
#define LOG_ERR		printf
#define LOG_OFTEN	printf
#define LOG_DBG	printf
#define LOG_RUNLINE	LOG_DBG("run line %s#%d\n",__FILE__,__LINE__)

#define LOG_REALIZE	LOG_ERR("not realize\n")

#define user_assert assert
// #define fatal(s)	do{LOG_ERR("%s",s);exit(-1);}while(0)
#define fatal(...)        do{LOG_ERR(__VA_ARGS__);abort();exit(1);}while(0)
#endif

