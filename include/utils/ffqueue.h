// thanks to zhouyw
#ifndef UTILS_FFQUEUE_H
#define UTILS_FFQUEUE_H
#ifdef __cplusplus
extern "C" {
#endif

#include "utils/sys.h"
#include <stdint.h>
#include <stddef.h>
#include <stdalign.h>

struct ffqueue_t {
    uint64_t * buffer; 
    uint64_t capacity;
    uint64_t mask;
    uint64_t multiple;
    alignas(CACHE_LINE_SIZE) uint64_t head;
    alignas(CACHE_LINE_SIZE) uint64_t head_unit_offset;
    alignas(CACHE_LINE_SIZE) uint64_t tail;
    alignas(CACHE_LINE_SIZE) uint64_t tail_unit_offset;
};

struct ffqueue_t * ffqueue_create(uint64_t msg_size, uint64_t msg_capacity);
void ffqueue_destroy(struct ffqueue_t * ffqueue);

#define ffqueue_get_acquire_try(ffqueue) ({ffqueue->head_unit_offset = (ffqueue->head & ffqueue->mask)*ffqueue->multiple; ffqueue->buffer[ffqueue->head_unit_offset] == 0 ? NULL : ffqueue->buffer + ffqueue->head_unit_offset + 1;})
#define ffqueue_get_release(ffqueue) ({asm volatile("");ffqueue->buffer[ffqueue->head_unit_offset] = 0; ++ffqueue->head;})

#define ffqueue_put_acquire_try(ffqueue) ({ffqueue->tail_unit_offset = (ffqueue->tail & ffqueue->mask)*ffqueue->multiple; ffqueue->buffer[ffqueue->tail_unit_offset] == 0 ? ffqueue->buffer + ffqueue->tail_unit_offset + 1 : NULL;})
#define ffqueue_put_release(ffqueue) ({asm volatile("");ffqueue->buffer[ffqueue->tail_unit_offset] = 1; ++ffqueue->tail;})

#ifdef __cplusplus
};
#endif
#endif // UTILS_FFQUEUE_H

