//
// Created by yangjiegogogo@gmail.com on 5/21/19.
//

#ifndef UTILS_RBTREE64_H
#define UTILS_RBTREE64_H

#include "utils/rbtree.h"
#include "utils/debug.h"

struct rbnode64_t {
    struct rbnode_t node;
    uint64_t key;
    void * data;
};

ABS_INLINE struct rbnode64_t * rbtree64_insert(struct rbroot_t * root, struct rbnode64_t * node){

    struct rbnode_t * tmp;
    struct rbnode_t * last;

    tmp=root->rbnode;

    // root is null
    if(unlikely(tmp==NULL))
    {
        node->node.color=RB_BLACK;
        root->rbnode=&node->node;
        return NULL;
    }

    DBG("rbtree64: root node=%lu",(uint64_t)root->rbnode);

    DBG("rbtree64: searching last node");

    do{

        DBG("rbtree64: key=%lu",((struct rbnode64_t*)tmp)->key);

        if(((struct rbnode64_t*)tmp)->key == node->key ){
            return (struct rbnode64_t*)tmp;
        }
        last=tmp;
        tmp= ((struct rbnode64_t*)tmp)->key > node->key ? tmp->left : tmp->right;
        DBG("rbtree64: tmp=%lu",(uint64_t)tmp);

    }while(tmp!=NULL);

    DBG("rbtree64: inserting node");

    if(((struct rbnode64_t*)last)->key > node->key){
        rbtree_insert_left(root,last,&node->node);
        return NULL;
    }


    rbtree_insert_right(root,last,&node->node);
    return NULL;
}

ABS_INLINE struct rbnode64_t * rbtree64_search(struct rbroot_t * root, uint64_t key){

    struct rbnode_t * tmp;

    tmp=root->rbnode;
    // root is null
    if(unlikely(tmp==NULL))
    {
        return NULL;
    }

    do{
        if(((struct rbnode64_t*)tmp)->key == key ){
            return (struct rbnode64_t*)tmp;
        }

        tmp= ((struct rbnode64_t*)tmp)->key > key ? tmp->left : tmp->right;

    }while(tmp!=NULL);

    return NULL;
}



ABS_INLINE struct rbnode64_t * rbtree64_delete(struct rbroot_t * root, uint64_t key){
    struct rbnode_t * tmp;

    tmp=root->rbnode;
    // root is null
    if(unlikely(tmp==NULL))
    {
        return NULL;
    }

    do{
        if(((struct rbnode64_t*)tmp)->key == key ){
            rbtree_delete(tmp,root);
            return (struct rbnode64_t*)tmp;
        }

        tmp= ((struct rbnode64_t*)tmp)->key > key ? tmp->left : tmp->right;

    }while(tmp!=NULL);

    return NULL;
}

typedef void (*rbtree64_callback)(struct rbnode64_t *node) ;

ABS_INLINE void rbtree64_all(struct rbroot_t * root, rbtree64_callback callback){
    struct rbnode_t * node;
    for (node=rbtree_first(root);node;node=rbtree_next(node)) {
        callback((struct rbnode64_t*)node);
    }
}

#endif //UTILS_RBTREE64_H
