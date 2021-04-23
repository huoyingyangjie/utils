#include "utils/ffqueue.h"
#include "utils/mess.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>

struct ffqueue_t * ffqueue_create(uint64_t msg_size, uint64_t msg_capacity)
{
    struct ffqueue_t * ffqueue = NULL;
    uint64_t aligned_ffqueue_size = next_pow(sizeof(struct ffqueue_t));
    uint64_t aligned_msg_size = next_pow(sizeof(uint64_t) + msg_size);
    uint64_t aligned_msg_capacity = next_pow(msg_capacity);
    uint64_t mem_size = aligned_ffqueue_size + aligned_msg_size * aligned_msg_capacity;
    int ret = 0;

    ret = posix_memalign((void**)&ffqueue, CACHE_LINE_SIZE, mem_size);
    if(ret)
    {
        errno = ret;
        return NULL;
    }
    memset(ffqueue, 0, mem_size);
    ffqueue->capacity = aligned_msg_capacity;
    ffqueue->buffer = (uint64_t*)((char *)(ffqueue) + aligned_ffqueue_size);
    ffqueue->multiple = aligned_msg_size / sizeof(uint64_t);
    ffqueue->head = 0;
    ffqueue->head_unit_offset = 0;
    ffqueue->tail = 0;
    ffqueue->tail_unit_offset = 0;
    ffqueue->mask = ffqueue->capacity - 1;
    return ffqueue;
}

void ffqueue_destroy(struct ffqueue_t * ffqueue)
{
    free(ffqueue);
}

