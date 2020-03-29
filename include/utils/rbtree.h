//
// Created by yangjiegogogo@gmail.com on 5/15/19.
//

#ifndef UTILS_RBTREE_H
#define UTILS_RBTREE_H

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "utils/sys.h"
#include "utils/debug.h"


#define RB_RED   (0UL)
#define RB_BLACK (1UL)


struct rbnode_t {
    uint64_t color;
    struct rbnode_t *  left;
    struct rbnode_t *  right;
    struct rbnode_t *  parent;
};


struct rbroot_t {
    struct rbnode_t * rbnode;
};

ABS_INLINE void rbtree_init_node(struct rbnode_t * node){
    node->color=RB_RED;
    node->parent=NULL;
    node->right=NULL;
    node->left=NULL;
}

/*
 * initialize red black tree root node , make it as empty tree
 *
 * */

ABS_INLINE void rbtree_init_root(struct rbroot_t * root){
    root->rbnode=NULL;
}

#define INSERT_RIGHT_SECOND()   \
            pp=parent->parent;\
            ppp=pp->parent;\
            if(parent==pp->left){\
            uncle=pp->right;\
            if(uncle==NULL || uncle->color==RB_BLACK){\
            parent->right=node->left;\
            node->left->parent=parent;\
            pp->left=node->right;\
            node->right->parent=pp;\
            node->left=parent;\
            node->right=pp;\
            node->color=RB_BLACK;\
            node->parent=ppp;\
            parent->parent=node;\
            pp->parent=node;\
            pp->color=RB_RED;\
            if(ppp==NULL){\
            root->rbnode=node;\
            return;\
            }\
            if(ppp->right==pp)\
            ppp->right=node;\
            else\
            ppp->left=node;\
            return;\
            } else{\
            node->parent=parent;\
            parent->right=node;\
            parent->color=RB_BLACK;\
            uncle->color=RB_BLACK;\
            if(ppp==NULL)\
            {\
            root->rbnode=pp;\
            return;\
            }\
            pp->color=RB_RED;\
            if(ppp->color==RB_BLACK)\
            return;\
            node=pp;\
            parent=ppp;\
            if(parent->right==node)\
            {goto right;}\
            else\
            {goto left;}\
            }\
            } else{\
            uncle=pp->left;\
            if(uncle==NULL || uncle->color==RB_BLACK){\
            node->parent=parent;\
            pp->right=parent->left;\
            if(parent->left!=NULL)\
            parent->left->parent=pp;\
            parent->left=pp;\
            parent->right=node;\
            parent->parent=ppp;\
            parent->color=RB_BLACK;\
            pp->parent=parent;\
            pp->color=RB_RED;\
            if(ppp==NULL){\
            root->rbnode=parent;\
            return;\
            }\
            if(ppp->right==pp)\
            ppp->right=parent;\
            else\
            ppp->left=parent;\
            return;\
            } else{\
            node->parent=parent;\
            parent->right=node;\
            parent->color=RB_BLACK;\
            uncle->color=RB_BLACK;\
            if(ppp==NULL)\
            {\
            root->rbnode=pp;\
            return;\
            }\
            pp->color=RB_RED;\
            if(ppp->color==RB_BLACK)\
            return;\
            node=pp;\
            parent=ppp;\
            if(parent->right==node)\
            {goto right;}\
            else\
            {goto left;}\
            }\
            }\

#define INSERT_LEFT_SECOND()   \
            pp=parent->parent;\
            ppp=pp->parent;\
            if(parent==pp->left){\
            uncle=pp->right;\
            if(uncle==NULL || uncle->color==RB_BLACK){\
            DBG("parent is left,uncle black");\
            node->parent=parent;\
            parent->color=RB_BLACK;\
            parent->left=node;\
            pp->left=parent->right;\
            if(parent->right!=NULL)\
            parent->right->parent=pp;\
            parent->right=pp;\
            parent->parent=ppp;\
            pp->parent=parent;\
            pp->color=RB_RED;\
            if(ppp==NULL){\
            root->rbnode=parent;\
            return;\
            }\
            if(ppp->right==pp)\
            ppp->right=parent;\
            else\
            ppp->left=parent;\
            return;\
            } else{\
            DBG("parent is left,uncle red");\
            node->parent=parent;\
            parent->color=RB_BLACK;\
            parent->left=node;\
            uncle->color=RB_BLACK;\
            if(ppp==NULL)\
            {\
            root->rbnode=pp;\
            return;\
            }\
            pp->color=RB_RED;\
            if(ppp->color==RB_BLACK)\
            return;\
            node=pp;\
            parent=ppp;\
            if(parent->right==node)\
            {goto right;}\
            else\
            {goto left;}\
            }\
            } else{\
            uncle=pp->left;\
            if(uncle==NULL || uncle->color==RB_BLACK){\
            DBG("parent is right,uncle black");\
            parent->left=node->right;\
            node->right->parent=parent;\
            pp->right=node->left;\
            node->left->parent=pp;\
            node->left=pp;\
            node->right=parent;\
            node->color=RB_BLACK;\
            node->parent=ppp;\
            parent->parent=node;\
            pp->parent=node;\
            pp->color=RB_RED;\
            if(ppp==NULL){\
            root->rbnode=node;\
            return;\
            }\
            if(ppp->right==pp)\
            ppp->right=node;\
            else\
            ppp->left=node;\
            return;\
            } else{\
            node->parent=parent;\
            parent->left=node;\
            parent->color=RB_BLACK;\
            uncle->color=RB_BLACK;\
            if(ppp==NULL)\
            {\
            root->rbnode=pp;\
            return;\
            }\
            pp->color=RB_RED;\
            if(ppp->color==RB_BLACK)\
            return;\
            node=pp;\
            parent=ppp;\
            if(parent->right==node)\
            {goto right;}\
            else\
            {goto left;}\
            }\
            }\


#define INSERT_RIGHT_FIRST()   \
        pp=parent->parent;\
        ppp=pp->parent;\
        if(parent==pp->left){\
        uncle=pp->right;\
        if(uncle==NULL || uncle->color==RB_BLACK){\
        parent->right=NULL;\
        pp->left=NULL;\
        node->left=parent;\
        node->right=pp;\
        node->color=RB_BLACK;\
        node->parent=ppp;\
        parent->parent=node;\
        pp->parent=node;\
        pp->color=RB_RED;\
        if(ppp==NULL){\
        root->rbnode=node;\
        return;\
        }\
        if(ppp->right==pp)\
        ppp->right=node;\
        else\
        ppp->left=node;\
        return;\
        } else{\
        node->parent=parent;\
        parent->right=node;\
        parent->color=RB_BLACK;\
        uncle->color=RB_BLACK;\
        if(ppp==NULL)\
        {\
        root->rbnode=pp;\
        return;\
        }\
        pp->color=RB_RED;\
        if(ppp->color==RB_BLACK)\
        return;\
        node=pp;\
        parent=ppp;\
        if(parent->right==node)\
        {right:\
        INSERT_RIGHT_SECOND()\
        }\
        else\
        {left:\
            INSERT_LEFT_SECOND()}\
        }\
        } else{\
        uncle=pp->left;\
        if(uncle==NULL || uncle->color==RB_BLACK){\
        node->parent=parent;\
        pp->right=parent->left;\
        if(parent->left!=NULL)\
        parent->left->parent=pp;\
        parent->left=pp;\
        parent->right=node;\
        parent->parent=ppp;\
        parent->color=RB_BLACK;\
        pp->parent=parent;\
        pp->color=RB_RED;\
        if(ppp==NULL){\
        root->rbnode=parent;\
        return;\
        }\
        if(ppp->right==pp)\
        ppp->right=parent;\
        else\
        ppp->left=parent;\
        return;\
        } else{\
        node->parent=parent;\
        parent->right=node;\
        parent->color=RB_BLACK;\
        uncle->color=RB_BLACK;\
        if(ppp==NULL)\
        {\
        root->rbnode=pp;\
        return;\
        }\
        pp->color=RB_RED;\
        if(ppp->color==RB_BLACK)\
        return;\
        node=pp;\
        parent=ppp;\
        if(parent->right==node)\
        {INSERT_RIGHT_SECOND()}\
        else\
        {INSERT_LEFT_SECOND()}\
        }\
        }\

#define INSERT_LEFT_FIRST() \
        pp=parent->parent;\
        ppp=pp->parent;\
        if(parent==pp->left){\
        uncle=pp->right;\
        if(uncle==NULL || uncle->color==RB_BLACK){\
        DBG("parent is left,uncle black");\
        node->parent=parent;\
        parent->color=RB_BLACK;\
        parent->left=node;\
        pp->left=parent->right;\
        if(parent->right!=NULL)\
        parent->right->parent=pp;\
        parent->right=pp;\
        parent->parent=ppp;\
        pp->parent=parent;\
        pp->color=RB_RED;\
        if(ppp==NULL){\
        root->rbnode=parent;\
        return;\
        }\
        if(ppp->right==pp)\
        ppp->right=parent;\
        else\
        ppp->left=parent;\
        return;\
        } else{\
        DBG("parent is left,uncle red");\
        node->parent=parent;\
        parent->color=RB_BLACK;\
        parent->left=node;\
        uncle->color=RB_BLACK;\
        if(ppp==NULL)\
        {\
        root->rbnode=pp;\
        return;\
        }\
        pp->color=RB_RED;\
        if(ppp->color==RB_BLACK)\
        return;\
        node=pp;\
        parent=ppp;\
        if(parent->right==node)\
        {   right:\
            INSERT_RIGHT_SECOND()}\
        else\
        {   left:\
            INSERT_LEFT_SECOND()}\
        }\
        } else{\
        uncle=pp->left;\
        if(uncle==NULL || uncle->color==RB_BLACK){\
        DBG("parent is right,uncle black");\
        parent->left=NULL;\
        pp->right=NULL;\
        node->left=pp;\
        node->right=parent;\
        node->color=RB_BLACK;\
        node->parent=ppp;\
        parent->parent=node;\
        pp->parent=node;\
        pp->color=RB_RED;\
        if(ppp==NULL){\
        root->rbnode=node;\
        return;\
        }\
        if(ppp->right==pp)\
        ppp->right=node;\
        else\
        ppp->left=node;\
        return;\
        } else{\
        node->parent=parent;\
        parent->left=node;\
        parent->color=RB_BLACK;\
        uncle->color=RB_BLACK;\
        if(ppp==NULL)\
        {\
        root->rbnode=pp;\
        return;\
        }\
        pp->color=RB_RED;\
        if(ppp->color==RB_BLACK)\
        return;\
        node=pp;\
        parent=ppp;\
        if(parent->right==node)\
        {INSERT_RIGHT_SECOND()}\
        else\
        {INSERT_LEFT_SECOND()}\
        }\
        }\



ABS_INLINE void rbtree_insert_left(struct rbroot_t * root, struct rbnode_t * parent, struct rbnode_t * node){
    struct rbnode_t * uncle;
    struct rbnode_t * pp;
    struct rbnode_t * ppp;


    //parent is black
    if(parent->color==RB_BLACK) {
        parent->left = node;
        node->parent = parent;
        return;
    }

    INSERT_LEFT_FIRST()



}

ABS_INLINE void rbtree_insert_right(struct rbroot_t * root, struct rbnode_t * parent, struct rbnode_t * node){
    struct rbnode_t * uncle;
    struct rbnode_t * pp;
    struct rbnode_t * ppp;



    //parent is black
    if(parent->color==RB_BLACK) {
        parent->right = node;
        node->parent = parent;
        return;
    }


    INSERT_RIGHT_FIRST()

}


ABS_INLINE void rbtree_change_child(struct rbnode_t * src, struct rbnode_t * dst, struct rbnode_t * parent, struct rbroot_t * root){
    if(parent){
        if(parent->left==src)
            parent->left=dst;
        else
            parent->right=dst;
    }
    else
        root->rbnode=dst;

}

ABS_INLINE struct rbnode_t * rbtree_delete_pre(struct rbnode_t * node, struct rbroot_t * root){
    struct rbnode_t * child=node->right;
    struct rbnode_t * tmp=node->left;
    struct rbnode_t * parent,*rebalance;

    if(!tmp){
        parent=node->parent;
        rbtree_change_child(node,child,parent,root);
        if(child)
        {
            child->parent=parent;
            child->color=node->color;
            rebalance=NULL;
        }
        else
            rebalance=node->color==RB_BLACK?parent:NULL;
        tmp=parent;
    }
    else if(!child){
        parent=node->parent;
        tmp->parent=parent;
        tmp->color=node->color;
        rbtree_change_child(node,tmp,parent,root);
        rebalance=NULL;
        tmp=parent;
    }
    else
    {
        struct rbnode_t * successor=child,*child2;
        tmp=child->left;
        if(!tmp){
            parent=successor;
            child2=successor->right;
        }
        else
        {
            do{
                parent=successor;
                successor=tmp;
                tmp=tmp->left;
            }while (tmp);
            child2=successor->right;
            parent->left=child2;
            successor->right=child;
            child->parent=successor;
        }
        tmp=node->left;
        successor->left=tmp;
        tmp->parent=successor;

        tmp=node->parent;
        rbtree_change_child(node,successor,tmp,root);

        if(child2){
            successor->parent=node->parent;
            successor->color=node->color;
            child2->parent=parent;
            child2->color=RB_BLACK;
            rebalance=NULL;
        }
        else
        {
            rebalance=successor->color==RB_BLACK?parent:NULL;
            successor->parent=node->parent;
            successor->color=node->color;
        }
        tmp=successor;//maybe useless
    }

    return rebalance;
}



ABS_INLINE void rbtree_rotate_set_parents(struct rbnode_t * src, struct rbnode_t *dst, struct rbroot_t * root, uint64_t color) {
    struct rbnode_t * parent=src->parent;
    dst->parent=src->parent;
    dst->color=src->color;
    src->parent=dst;
    src->color=color;
    rbtree_change_child(src,dst,parent,root);
}


ABS_INLINE void rbtree_delete_balance(struct rbnode_t *parent, struct rbroot_t * root){
    struct rbnode_t *node=NULL,*sibling,*tmp1,*tmp2;
    while (1) {
        sibling = parent->right;
        if (node != sibling) {
            if (sibling->color == RB_RED) {
                tmp1 = sibling->left;
                parent->right = tmp1;
                sibling->left = parent;
                tmp1->parent = parent;
                tmp1->color = RB_BLACK;
                rbtree_rotate_set_parents(parent, sibling, root, RB_RED);
                sibling = tmp1;
            }
            tmp1 = sibling->right;
            if (!tmp1 || tmp1->color == RB_BLACK) {
                tmp2 = sibling->left;
                if (!tmp2 || tmp2->color == RB_BLACK) {
                    sibling->parent = parent;
                    sibling->color = RB_RED;
                    if (parent->color == RB_RED)
                        parent->color = RB_BLACK;
                    else {
                        node = parent;
                        parent = node->parent;
                        if (parent)
                            continue;
                    }
                    break;
                }
                tmp1 = tmp2->right;
                sibling->left = tmp1;
                tmp2->right = sibling;
                parent->right = tmp2;
                if (tmp1) {
                    tmp1->parent = sibling;
                    tmp1->color = RB_BLACK;
                }
                tmp1 = sibling;
                sibling = tmp2;
            }
            tmp2 = sibling->left;
            parent->right = tmp2;
            sibling->left = parent;
            tmp1->parent = sibling;
            tmp1->color = RB_BLACK;
            if (tmp2)
                tmp2->parent = parent;
            rbtree_rotate_set_parents(parent, sibling, root, RB_BLACK);
            break;
        } else {
            sibling = parent->left;
            if (sibling->color == RB_RED) {
                tmp1 = sibling->right;
                parent->left = tmp1;
                sibling->right = parent;
                tmp1->parent = parent;
                tmp1->color = RB_BLACK;
                rbtree_rotate_set_parents(parent, sibling, root, RB_RED);
                sibling = tmp1;
            }
            tmp1 = sibling->left;
            if (!tmp1 || tmp1->color == RB_BLACK) {
                tmp2 = sibling->right;
                if (!tmp2 || tmp2->color == RB_BLACK) {
                    sibling->parent = parent;
                    sibling->color = RB_RED;

                    if (parent->color == RB_RED)
                        parent->color = RB_BLACK;
                    else {
                        node = parent;
                        parent = node->parent;

                        if (parent)
                            continue;
                    }
                    break;
                }
                tmp1 = tmp2->left;
                sibling->right = tmp1;
                tmp2->left = sibling;
                parent->left = tmp2;
                if (tmp1) {
                    tmp1->parent = sibling;
                    tmp1->color = RB_BLACK;
                }
                tmp1 = sibling;
                sibling = tmp2;
            }
            tmp2 = sibling->right;
            parent->left = tmp2;
            sibling->right = parent;
            tmp1->parent = sibling;
            tmp1->color = RB_BLACK;
            if (tmp2)
                tmp2->parent = parent;
            rbtree_rotate_set_parents(parent, sibling, root, RB_BLACK);
            break;
        }
    }

}


ABS_INLINE void rbtree_delete(struct rbnode_t * node , struct rbroot_t * root){
            struct rbnode_t * rebalance=rbtree_delete_pre(node, root);
            if(rebalance)
                rbtree_delete_balance(rebalance,root);
}




struct rbnode_t * rbtree_first(struct rbroot_t * root)
{
    struct rbnode_t * n=root->rbnode;
    if(!n)
        return NULL;
    while (n->left)
        n=n->left;
    return n;
}

struct rbnode_t * rbtree_next(struct rbnode_t * node)
{
    struct rbnode_t * parent;

    if(node->right)
    {
        node=node->right;
        while (node->left)
            node=node->left;
        return node;
    }

    while ((parent=node->parent)&&node==parent->right)
        node=parent;

    return parent;
}



#endif //UTILS_RBTREE_H
