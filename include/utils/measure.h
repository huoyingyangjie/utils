#ifndef UTILS_MEASURE_H
#define UTILS_MEASURE_H
#ifdef __cplusplus
extern "C" {
#endif

#define RDTSC(val)  ({unsigned low,high;asm volatile("CPUID\n\t" "RDTSC\n\t" "mov %%edx, %0\n\t" "mov %%eax, %1\n\t": "=r" (high), "=r" (low):: "%rax", "%rbx", "%rcx", "%rdx"); val=high; val=(val<<32)|low;})
#define RDTSCP(val) ({unsigned low,high;asm volatile("RDTSCP\n\t" "mov %%edx, %0\n\t" "mov %%eax, %1\n\t" "CPUID\n\t": "=r" (high), "=r" (low):: "%rax", "%rbx", "%rcx", "%rdx"); val=high; val=(val<<32)|low;})

#ifdef __cplusplus
};
#endif
#endif // UTILS_MEASURE_H
