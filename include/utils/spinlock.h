//
// Created by yangjiegogogo@gmail.com on 12/13/19.
//

#ifndef UTILS_SPINLOCK_H
#define UTILS_SPINLOCK_H

#include "utils/sys.h"
#include <emmintrin.h>

// Thanks to Cory Nelson

ABS_INLINE int spin_lock_try(int * p){
    return __sync_bool_compare_and_swap(p,0,1);
}

ABS_INLINE void spin_lock(int volatile * p){
    while (!__sync_bool_compare_and_swap(p,0,1))
    {
        do{
            _mm_pause();
        } while (*p);
    }
}

ABS_INLINE void spin_unlock(int volatile * p){
    asm volatile ("":::"memory");
    *p=0;
}


#endif //UTILS_SPINLOCK_H

