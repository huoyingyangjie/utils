//
// Created by yangjiegogogo@gmail.com on 8/5/19.
//
#include "utils/debug.h"
#include "utils/fifo.h"
#include <errno.h>
#include <string.h>

struct rb_t * create_shm_rb(key_t key, size_t entry_num, size_t entry_size){
    int shmid;
    void * ret;
    struct rb_t *rb;

    if(entry_num<1 || (entry_num&(entry_num-1))!=0)
        DIE("entry number must be pow of 2");

    shmid=shmget(key,0,0);
    if(shmid==-1){
        if(errno != ENOENT)
            DIE("get shared memory failed,errno=%d,strerror=%s",errno,strerror(errno));
    }else{
        if(shmctl(shmid, IPC_RMID, NULL) == -1)
            DIE("delete shared memory failed,errno=%d,strerror=%s",errno,strerror(errno));
    }

    shmid=shmget(key, sizeof(struct rb_t)+entry_num*entry_size+CACHE_LINE_SIZE,IPC_CREAT|0666);
    if(shmid==-1)
        DIE("create shared memory failed,errno=%d,strerror=%s",errno,strerror(errno));

    ret=shmat(shmid, NULL, 0);
    if(ret==(void*)-1)
        DIE("attach shared memory failed,errno=%d,strerror=%s",errno,strerror(errno));

    ret=(char*)ret+(CACHE_LINE_SIZE-(((uint64_t)ret)%CACHE_LINE_SIZE));
    rb=rb_init(ret,entry_num,entry_size);
    return rb;
}


struct rb_t * get_shm_rb(key_t key, size_t entry_num, size_t entry_size){
    int shmid;
    void * ret;
    if(entry_num<1 || (entry_num&(entry_num-1))!=0)
        DIE("entry number must be pow of 2");
    shmid=shmget(key,0,0);
    if(shmid==-1)
        DIE("get shared memory failed,errno=%d,strerror=%s",errno,strerror(errno));

    ret=shmat(shmid,NULL,0);
    if(ret==(void*)-1)
        DIE("attach shared memory failed,errno=%d,strerror=%s",errno,strerror(errno));

    ret=(char*)ret+(CACHE_LINE_SIZE-(((uint64_t)ret)%CACHE_LINE_SIZE));
    return (struct rb_t*)ret;
}

struct rb_t * create_rb(size_t entry_num, size_t entry_size){
    struct rb_t * rb=NULL;
    if(entry_num<1 || (entry_num&(entry_num-1))!=0)
        DIE("entry number must be pow of 2");
    rb=(struct rb_t*)malloc(sizeof(struct rb_t)+entry_num*entry_size+CACHE_LINE_SIZE);
    if(rb==NULL)
        DIE("malloc failed,errno=%d,strerror=%s",errno,strerror(errno));

    rb=(struct rb_t*)((char*)rb+(CACHE_LINE_SIZE-(((uint64_t)rb)%CACHE_LINE_SIZE)));
    rb->read=0;
    rb->write=0;
    rb->capacity=entry_num;
    rb->entry=entry_size;
    rb->publisher_buffer=(char*)rb+sizeof(struct rb_t);
    rb->processor_buffer=(char*)rb+sizeof(struct rb_t);
    return rb;
}

struct rb_t * rb_init(void * rb_ptr, uint64_t entry_num, uint64_t entry_size){
    struct rb_t * rb=(struct rb_t*)rb_ptr;
    rb->read=0;
    rb->write=0;
    rb->capacity=entry_num;
    rb->entry=entry_size;
    return rb;
}


struct rb_t * publisher_rb_init(void * rb_ptr){
    struct rb_t* rb=(struct rb_t*)rb_ptr;
    rb->publisher_buffer=(char*)rb_ptr+sizeof(struct rb_t);
    return rb;
}

struct rb_t * processor_rb_init(void * rb_ptr){
    struct rb_t * rb=(struct rb_t*)rb_ptr;
    rb->processor_buffer=(char*)rb_ptr+sizeof(struct rb_t);
    return rb;
}

uint64_t roundup_pow_of_two(uint64_t x)
{
    uint64_t ret = 1;
    if (x == 0){ return 1; }
    while (ret < x)
    {
        ret = ret << 1;
    }
    return ret;
}