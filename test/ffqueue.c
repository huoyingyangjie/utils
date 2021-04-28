#include "utils/ffqueue.h"
#include "utils/meaclk.h"
#include "utils/measure.h"
#define _GNU_SOURCE
#define __USE_GNU
#include <sched.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define LOOPSIZE (1000000000)
#define MSGNUM (4096)

struct msg_t {
    uint64_t a;
    uint64_t b;
    unsigned char c; 
};

void *fn_read(void * args)
{
    struct ffqueue_t * ffqueue = (struct ffqueue_t *)args;
    struct msg_t * msg = 0;
    uint64_t i = 0;
    unsigned char c = 0;
    while(i < LOOPSIZE)
    {
        if((msg = (struct msg_t*)ffqueue_get_acquire_try(ffqueue)) != NULL)
        { 
            c = i+3;
            if(msg->a != i || msg->b != (i+1) || msg->c != c)
            {
                printf("incorrect, a=%lu, b=%lu, msg->c=%u, c=%u, i=%lu.\n", msg->a, msg->b, msg->c, c, i);
                abort();
            }
            ffqueue_get_release(ffqueue);
            ++i;
        }
    } 
    return NULL;
}

void *fn_write(void * args)
{
    struct ffqueue_t * ffqueue = (struct ffqueue_t *)args;
    struct msg_t * msg = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t i = 0;    
 

    RDTSC(t1);
    //t1=get_cycles();
    while(i < LOOPSIZE)
    {
        if((msg = (struct msg_t*)ffqueue_put_acquire_try(ffqueue)) != NULL)
        {
            msg->a = i;
            msg->b = i+1;
            msg->c = (i+3);
            //printf("write msg=%p\n.", msg);
            ffqueue_put_release(ffqueue);
            ++i;
        }
    }
    
    RDTSC(t2);
    //t2=get_cycles();
    printf("cycles = %lu.\n", (t2-t1)/LOOPSIZE);
    return NULL;
}

int main(int argc, char * argv[])
{
    if (argc != 3) {
        printf("Usage: app cpu1 cpu2\n");
        return 0;
    }
    int cpu1 = strtol(argv[1], NULL, 10);
    int cpu2 = strtol(argv[2], NULL, 10);
    pthread_barrier_t bar;
    pthread_attr_t attr1, attr2;
    pthread_attr_init(&attr1);
    pthread_attr_init(&attr2);

    cpu_set_t cpus1, cpus2;
    CPU_ZERO(&cpus1);
    CPU_ZERO(&cpus2);
    CPU_SET(cpu1, &cpus1);
    CPU_SET(cpu2, &cpus2);
    pthread_attr_setaffinity_np(&attr1, sizeof(cpu_set_t), &cpus1);
    pthread_attr_setaffinity_np(&attr2, sizeof(cpu_set_t), &cpus2);
    pthread_barrier_init(&bar, NULL, 2);

    struct ffqueue_t * ffqueue = ffqueue_create(sizeof(struct msg_t), MSGNUM);
    if(ffqueue == NULL)
    {
        printf("Allocate memory failed.\n");
        return 0;
    }

    pthread_t thread1, thread2;
    pthread_create(&thread1, &attr1, fn_read, (void*)ffqueue);
    pthread_create(&thread2, &attr2, fn_write, (void*)ffqueue);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_barrier_destroy(&bar);
}
