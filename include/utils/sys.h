//
// Created by yangjiegogogo@gmail.com on 5/15/19.
//

#ifndef UTILS_SYS_H
#define UTILS_SYS_H

#ifdef __cplusplus
extern "C" {
#endif

#define likely(x)   __builtin_expect(!!(x),1)
#define unlikely(x) __builtin_expect(!!(x),0)

#define ABS_INLINE static __inline__ __attribute__((always_inline))

#define PAGE_SIZE (4096U)
#define CACHE_LINE_SIZE (64U)

#define cpu_pause() __asm__ __volatile__ (".byte 0xf3, 0x90")

#define MFENCE __asm__ __volatile__ ("mfence":::"memory")
#define LFENCE __asm__ __volatile__ ("lfence":::"memory")
#define SFENCE __asm__ __volatile__ ("sfence":::"memory")


long get_nr_open();
void print_args(int argc, char ** argv);

#ifdef __cplusplus
};
#endif

#endif //UTILS_SYS_H
