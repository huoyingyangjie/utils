//
// Created by yangjiegogogo@gmail.com on 8/6/19.
//

#ifndef UTILS_TIRE_H
#define UTILS_TIRE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "utils/sys.h"
#ifndef NDEBUG
#include "utils/debug.h"
#endif
#define MAX_UINT8 (256U)

struct tire_t{
    void *val;
    struct tire_t * nodes[MAX_UINT8];
    uint64_t light;
};


struct tire_t * tire_create();
void tire_init(struct tire_t * tire);

int tire_add(struct tire_t * root, char * str, void * val);
int tire_add_hp(struct tire_t * root, char * str, void *val, struct tire_t * new_node);

ABS_INLINE void tire_del(struct tire_t * root, char * str, void **val){
    struct tire_t * node=root;
    unsigned char character;

    again:
    character=*((unsigned char*)str);
    if(!character)
        return;
    if(!node)
        return ;
    if(*(++str))
        goto again;
    if(node->light)
    {
        node->light=0;
        if(val!=NULL)
            *val=node->val;
    }
}


ABS_INLINE int tire_find(struct tire_t * root, char * str, void **val){
    struct tire_t * node=root;
    unsigned char * tmp_str=(unsigned char *)str;
    unsigned char character;

    character=*tmp_str;
    if(!character)
        return -1;
    again:
    node=node->nodes[character];
    if(!node)
        return -1;
    ++tmp_str;
    character=*tmp_str;
    if(character)
        goto again;
    if(node->light!=0)
    {
        if(val!=NULL)
            *val=node->val;
        return 0;
    }
    return -1;
}

#ifdef __cplusplus
};
#endif

#endif //UTILS_TIRE_H
