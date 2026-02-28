//
// Created by Lenovo on 2025/6/24.
//

#include "RBT.h"
#include "malloc.h"

static RBNode* CreateNilNode(){
    RBNode *node = (RBNode*)malloc(sizeof(RBNode));
    if(node){
        node->color = BLACK;
        node->key = NULL;
        node->data = NULL;
        node->left = node;
        node->right = node;
        node->parent = node;
    }
    return node;
}

int RBTreeInit(RBTree *tree, // 修改为驼峰命名
               int (*compare)(const void*, const void*),
               void (*keyFree)(void*),
               void (*valueFree)(void*)){
    tree->nil = CreateNilNode();
    if(!tree->nil){
        return 0;
    }
    tree->root = tree->nil;
    tree->compare = compare;
    tree->KeyFree= keyFree;
    tree->DataFree = valueFree;
    return 1;
}
static RBNode* CreateRBNode(RBTree *tree, void *key, void *data) {
    RBNode *node = malloc(sizeof(RBNode));
    if (node) {
        node->key = key;
        node->data = data;
        node->color = RED;
        node->left = tree->nil;
        node->right = tree->nil;
        node->parent = tree->nil;
    }
    return node;
}

static void RBTreeLeftRotate(RBTree *tree, RBNode *x) { // 修改为驼峰命名
    RBNode *y = x->right;
    x->right = y->left;

    if (y->left != tree->nil)
        y->left->parent = x;

    y->parent = x->parent;

    if (x->parent == tree->nil)
        tree->root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->left = x;
    x->parent = y;
}
static void RBTreeRightRotate(RBTree *tree, RBNode *x) { // 修改为驼峰命名
    RBNode *y = x->left;
    x->left = y->right;

    if (y->right != tree->nil)
        y->right->parent = x;

    y->parent = x->parent;

    if (x->parent == tree->nil)
        tree->root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->right = x;
    x->parent = y;
}
static void RBTreeInsertFixup(RBTree *tree, RBNode *z) { // 修改为驼峰命名
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            RBNode *y = z->parent->parent->right; // 叔叔节点
            if (y->color == RED) {
                // 情况 1: 叔叔是红色
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                // 情况 2: 叔叔是黑色，z 是右孩子
                if (z == z->parent->right) {
                    z = z->parent;
                    RBTreeLeftRotate(tree, z); // 修改为驼峰命名
                }
                // 情况 3: 叔叔是黑色，z 是左孩子
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                RBTreeRightRotate(tree, z->parent->parent); // 修改为驼峰命名
            }
        } else { // z->parent == z->parent->parent->right
            RBNode *y = z->parent->parent->left; // 叔叔节点
            if (y->color == RED) {
                // 情况 1: 叔叔是红色
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                // 情况 2: 叔叔是黑色，z 是左孩子
                if (z == z->parent->left) {
                    z = z->parent;
                    RBTreeRightRotate(tree, z); // 修改为驼峰命名
                }
                // 情况 3: 叔叔是黑色，z 是右孩子
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                RBTreeLeftRotate(tree, z->parent->parent); // 修改为驼峰命名
            }
        }
    }
    tree->root->color = BLACK;
}
int RBTreeInsert(RBTree *tree, void *key, void *value) { // 修改为驼峰命名
    RBNode *z = CreateRBNode(tree,key,value); // 修改为驼峰命名
    if (!z)
        return 0;
    RBNode *y = tree->nil;
    RBNode *x = tree->root;
    while (x != tree->nil) {
        y = x;
        int cmp = tree->compare(z->key, x->key);
        if (cmp < 0)
            x = x->left;
        else if (cmp > 0) // 处理重复键：插入到右子树
            x = x->right;
        else { // key 已存在：更新 value
            if (tree->DataFree)
                tree->DataFree(x->data);

            x->data = z->data;

            // 新的 key 不需要
            if (tree->KeyFree)
                tree->KeyFree(z->key);

            free(z);
            return 1;
        }

    }
    z->parent = y;
    if (y == tree->nil)
        tree->root = z;
    else if (tree->compare(z->key, y->key) < 0)
        y->left = z;
    else
        y->right = z;

    z->left = tree->nil;
    z->right = tree->nil;
    z->color = RED;
    RBTreeInsertFixup(tree, z);
    return 1;
}
RBNode* RBTreeSearch(RBTree *tree, RBNode *x, void *key) {
    while (x != tree->nil) {
        int cmp = tree->compare(key, x->key);
        if (cmp == 0){
            return x;
        }

        else if (cmp < 0)
            x = x->left;
        else
            x = x->right;
    }
    return tree->nil;
}
RBNode* RBTreeMinimum(RBTree *tree, RBNode *x) {
    while (x->left != tree->nil)
        x = x->left;
    return x;
}
static void RBTreeTransplant(RBTree *tree, RBNode *u, RBNode *v) {
    if (u->parent == tree->nil)
        tree->root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    v->parent = u->parent;
}
static void RBTreeDeleteFixup(RBTree *tree, RBNode *x) {
    while (x != tree->root && x->color == BLACK) {
        if (x == x->parent->left) {
            RBNode *w = x->parent->right; // 兄弟节点
            if (w->color == RED) {
                // 情况 1: 兄弟是红色
                w->color = BLACK;
                x->parent->color = RED;
                RBTreeLeftRotate(tree, x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                // 情况 2: 兄弟是黑色，两个孩子都是黑色
                w->color = RED;
                x = x->parent;
            } else {
                // 情况 3: 兄弟是黑色，左孩子是红色，右孩子是黑色
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    RBTreeRightRotate(tree, w); // 修改为驼峰命名
                    w = x->parent->right;
                }
                // 情况 4: 兄弟是黑色，右孩子是红色
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                RBTreeLeftRotate(tree, x->parent); // 修改为驼峰命名
                x = tree->root; // 循环终止
            }
        } else { // x == x->parent->right
            RBNode *w = x->parent->left; // 兄弟节点
            if (w->color == RED) {
                // 情况 1: 兄弟是红色
                w->color = BLACK;
                x->parent->color = RED;
                RBTreeRightRotate(tree, x->parent); // 修改为驼峰命名
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK) {
                // 情况 2: 兄弟是黑色，两个孩子都是黑色
                w->color = RED;
                x = x->parent;
            } else {
                // 情况 3: 兄弟是黑色，右孩子是红色，左孩子是黑色
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    RBTreeLeftRotate(tree, w); // 修改为驼峰命名
                    w = x->parent->left;
                }
                // 情况 4: 兄弟是黑色，左孩子是红色
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                RBTreeRightRotate(tree, x->parent); // 修改为驼峰命名
                x = tree->root; // 循环终止
            }
        }
    }
    x->color = BLACK;
}
int RBTreeDelete(RBTree *tree, void *key) {
    RBNode *z = RBTreeSearch(tree, tree->root, key);
    if (z == tree->nil)
        return 0;
    RBNode *y = z;
    RBNode *x;
    RBColor yOriginalColor = y->color;
    if (z->left == tree->nil) {
        x = z->right;
        RBTreeTransplant(tree, z, z->right);
    } else if (z->right == tree->nil) {
        x = z->left;
        RBTreeTransplant(tree, z, z->left);
    } else {
        y = RBTreeMinimum(tree, z->right);
        yOriginalColor = y->color;
        x = y->right;
        if (y->parent == z) {
            x->parent = y;
        } else {
            RBTreeTransplant(tree, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        RBTreeTransplant(tree, z, y); // 修改为驼峰命名
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    if (yOriginalColor == BLACK) // 修改为驼峰命名
        RBTreeDeleteFixup(tree, x); // 修改为驼峰命名

    if (tree->KeyFree)
        tree->KeyFree(z->key); // 修改为驼峰命名
    if (tree->DataFree)
        tree->DataFree(z->data); // 修改为驼峰命名
    free(z);
    return 1;
}

// 释放节点
static void RBTreeFreeNodeRecursive(RBTree *tree, RBNode *node) { // 修改为驼峰命名
    if (node == tree->nil) return;

    RBTreeFreeNodeRecursive(tree, node->left); // 修改为驼峰命名
    RBTreeFreeNodeRecursive(tree, node->right); // 修改为驼峰命名

    if (tree->KeyFree)
        tree->KeyFree(node->key); // 修改为驼峰命名
    if (tree->DataFree) tree->DataFree(node->data); // 修改为驼峰命名
    free(node);
}

// 释放整棵树
void RBTreeFree(RBTree *tree) { // 修改为驼峰命名
    RBTreeFreeNodeRecursive(tree, tree->root); // 修改为驼峰命名
    free(tree->nil);
    tree->root = NULL;
    tree->nil = NULL;
}
RBNode* RBTreeSuccessor(RBTree *tree, RBNode *x) {
    if (x->right != tree->nil) {
        // 如果节点x有右子树，则后继是右子树中的最小节点
        x = x->right;
        while (x->left != tree->nil) {
            x = x->left;
        }
        return x;
    }

    // 如果没有右子树，后继是其最近的祖先节点
    RBNode *y = x->parent;
    while (y != tree->nil && x == y->right) {
        x = y;
        y = y->parent;
    }
    return y;
}
