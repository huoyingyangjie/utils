//
// Created by yangjiegogogo@gmail.com on 8/6/19.
//

#include "utils/tire.h"


struct tire_t * tire_create(){
    struct tire_t * tire=NULL;
    tire=(struct tire_t*)malloc(sizeof(struct tire_t));
    if(!tire)
        return NULL;
    memset(tire,0,sizeof(struct tire_t));
    return tire;
}

void tire_init(struct tire_t * tire){
    bzero(tire, sizeof(struct tire_t));
}

int tire_add(struct tire_t * root, char * str, void *val){
    struct tire_t * node=root;
    unsigned char character;
    again:
    character = *((unsigned char *)str);
    if(!character)
        return -1;
    if(!(node->nodes[character]))
    {
        node->nodes[character]=(struct tire_t *)malloc(sizeof(struct tire_t));
        if(!(node->nodes[character]))
            return -2;
        memset(node->nodes[character],0, sizeof(node->nodes));
    }
    node=node->nodes[character];
    if(!(*(++str)))
    {
        node->val=val;
        SFENCE;
        node->light=1;
        LFENCE;
        return 0;
    }
    goto again;
}

/*
 * return value:  < 0 failed,  0  success,  1 success but not use node
 * */

int tire_add_hp(struct tire_t * root, char * str, void *val, struct tire_t * new_node){
    struct tire_t * node=root;
    unsigned char character;
    int ret;
    ret=1;
    again:
    character = *((unsigned char *)str);
    if(!character)
        return -1;
    if(!(node->nodes[character]))
    {
        ret=0;
        node->nodes[character]=new_node;
        memset(node->nodes[character],0, sizeof(node->nodes));
    }
    node=node->nodes[character];
    if(!(*(++str)))
    {
        node->val=val;
        node->light=1;
        return ret;
    }
    goto again;
}