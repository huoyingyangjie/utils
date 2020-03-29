//
// Created by yangjiegogogo@gmail.com on 9/3/19.
//

#ifndef COSMOS_MEACLK_H
#define COSMOS_MEACLK_H

#ifdef __cplusplus
extern "C" {
#endif
#include "utils/sys.h"

ABS_INLINE unsigned long get_cycles(){
    unsigned low,high;
    unsigned long val;
    __asm__ __volatile__ ("rdtscp" : "=a" (low), "=d" (high));
    val=high;
    val=(val<<32)| low;
    return val;
}

#ifdef __cplusplus
};
#endif


#endif //COSMOS_MEACLK_H
