//
// Created by yangjiegogogo@gmail.com on 8/5/19.
//

#ifndef UTILS_FIFO_H
#define UTILS_FIFO_H

#include "utils/sys.h"
#include <stdint.h>
#include <sys/shm.h>
#include <stddef.h>


struct rb_t {
    uint8_t padding0[CACHE_LINE_SIZE];
    uint64_t write;
    uint8_t padding1[CACHE_LINE_SIZE];
    uint64_t read;
    uint8_t padding2[CACHE_LINE_SIZE];
    uint64_t capacity;
    uint64_t entry;
    char * publisher_buffer;
    char * processor_buffer;
    void * org_ptr;
}__attribute__((packed));

uint64_t roundup_pow_of_two(uint64_t x);
struct rb_t * create_shm_rb(key_t key, size_t entry_num, size_t entry_size);
struct rb_t * get_shm_rb(key_t key, size_t entry_num, size_t entry_size);
struct rb_t * create_rb(size_t entry_num, size_t entry_size);
struct rb_t * rb_init(void * rb_ptr, uint64_t entry_num, uint64_t entry_size);
struct rb_t * publisher_rb_init(void * rb_ptr);
struct rb_t * processor_rb_init(void * rb_ptr);

ABS_INLINE void * publisher_acquire_entry(struct rb_t * rb){
    uint_fast64_t sequence;
    again:
    sequence=rb->write;
    if(sequence < (__atomic_load_n(&rb->read, __ATOMIC_ACQUIRE) + rb->capacity))
        return (rb->publisher_buffer + (sequence & (rb->capacity - 1)) * rb->entry);
    else
    {cpu_pause();goto again;}
}

ABS_INLINE void * publisher_acquire_entry_try(struct rb_t * rb){
    uint_fast64_t sequence;
    sequence=rb->write;
    if(sequence < (__atomic_load_n(&rb->read, __ATOMIC_ACQUIRE) + rb->capacity))
        return (rb->publisher_buffer + (sequence & (rb->capacity - 1)) * rb->entry);
    else
        return NULL;
}

ABS_INLINE void publisher_release_entry(struct rb_t * rb){
    __atomic_store_n(&rb->write, rb->write + 1, __ATOMIC_RELEASE);
}


ABS_INLINE void * processor_acquire_entry(struct rb_t * rb){
    uint_fast64_t sequence;
    again:
    sequence = rb->read;
    if(sequence < __atomic_load_n(&rb->write, __ATOMIC_ACQUIRE))
        return ( rb->processor_buffer + (sequence & (rb->capacity - 1)) * rb->entry );
    else
    {cpu_pause();goto again;}
}


ABS_INLINE void * processor_acquire_entry_try(struct rb_t * rb){
    uint_fast64_t sequence;
    sequence = rb->read;
    if(sequence < __atomic_load_n(&rb->write, __ATOMIC_ACQUIRE))
        return ( rb->processor_buffer + (sequence & (rb->capacity - 1)) * rb->entry );
    else
        return NULL;
}


ABS_INLINE void processor_release_entry(struct rb_t * rb){
    __atomic_store_n(&rb->read, rb->read+1, __ATOMIC_RELEASE);
}


#endif //UTILS_FIFO_H
