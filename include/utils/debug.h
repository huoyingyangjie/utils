// Authors: YangJie, <yangjiegogogo@gmail.com> on 8/16/19.

#ifndef UTILS_DEBUG_H
#define UTILS_DEBUG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>


void segv_dump();

void debug_print_s(const char *level, const char * file, const int line, const char * fmt,...) __attribute__((format(printf,4,5)));
void debug_print_raw_s(const char *level,const char *file,const int line,const char *fmt,...) __attribute__((format(printf,4,5)));
void hexbuf(const void * buf, size_t len, const char *level, const char *file, const int line, const char *fmt, ...) __attribute__((format(printf,6,7)));

#ifndef NDEBUG
#define HEX(buf, len, fmt, args...) ({hexbuf(buf, len, "DBG",__FILE__,__LINE__,fmt,##args);})
#else
#define HEX(buf, len, fmt, args...) ({;})
#endif
#define HEXF(buf, len, fmt, args...) ({hexbuf(buf, len, "INF",__FILE__,__LINE__,fmt,##args);})


#define DIE(fmt,args...)  ({debug_print_s("DIE",__FILE__,__LINE__,fmt,##args);})

#ifndef NDEBUG

#define DBG(fmt,args...)  ({debug_print_s("DBG",__FILE__,__LINE__,fmt,##args);})

#else

#define DBG(fmt,args...)  ({;})

#endif

#define WRN(fmt,args...) ({debug_print_s("WRN",__FILE__,__LINE__,fmt,##args);})

#define INF(fmt,args...) ({debug_print_s("INF",__FILE__,__LINE__,fmt,##args);})

#define INFR(fmt,args...) ({debug_print_raw_s("INF",__FILE__,__LINE__,fmt,##args);})

#define ERR(fmt,args...) ({debug_print_s("ERR",__FILE__,__LINE__,fmt,##args);})

void print_hex(const void *buf,size_t len);

#ifndef NDEBUG
#define ASSERT(exp) {if(!(exp)) DIE("assert");}
#else
#define ASSERT(exp) {(exp)};
#endif

#ifdef __cplusplus
};
#endif
#endif //UTILS_DEBUG_H
