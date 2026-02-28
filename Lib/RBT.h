//
// Created by Lenovo on 2025/6/24.
//

#ifndef NEWDBMS_RBT_H
#define NEWDBMS_RBT_H
typedef enum {
    RED,
    BLACK
}RBColor;
typedef struct RBNode{
    void *key;
    void *data;
    RBColor color;
    struct RBNode*left;
    struct RBNode*right;
    struct RBNode*parent;
}RBNode;
typedef struct {
    RBNode *root;
    RBNode *nil; // 哨兵节点
    int (*compare)(const void*, const void*);
    void (*KeyFree)(void*);
    void (*DataFree)(void*);
} RBTree;
/**
 * @brief 初始化一个红黑树。
 *
 * @param tree 指向要初始化的RBTree结构体。
 * @param compare 用于比较两个键的函数指针。
 * @param keyFree 用于释放键内存的函数指针（可为NULL）。
 * @param valueFree 用于释放值内存的函数指针（可为NULL）。
 * @return 成功返回1，失败返回0。
 */
int RBTreeInit(RBTree *tree, // 修改为驼峰命名
               int (*compare)(const void*, const void*),
               void (*keyFree)(void*),
               void (*valueFree)(void*));

/**
 * @brief 向红黑树中插入一个键值对。
 *
 * @param tree 指向RBTree的指针。
 * @param key 要插入的键。
 * @param value 与键关联的值。
 * @return 成功返回1，失败返回0。
 */
int RBTreeInsert(RBTree *tree, void *key, void *value); // 修改为驼峰命名

/**
 * @brief 在红黑树中查找给定键的节点。
 *
 * @param tree 指向RBTree的指针。
 * @param x 当前开始搜索的节点（通常是tree->root）。
 * @param key 要查找的键。
 * @return 如果找到，返回指向RBNode的指针；否则，返回指向树的nil节点的指针。
 */
RBNode* RBTreeSearch(RBTree *tree, RBNode *x, void *key); // 修改为驼峰命名

/**
 * @brief 从红黑树中删除给定键的节点。
 *
 * @param tree 指向RBTree的指针。
 * @param key 要删除的节点的键。
 * @return 成功返回1，如果未找到键则返回0。
 */
int RBTreeDelete(RBTree *tree, void *key); // 修改为驼峰命名

/**
 * @brief 释放红黑树关联的所有内存。
 *
 * @param tree 指向RBTree的指针。
 */
void RBTreeFree(RBTree *tree); // 修改为驼峰命名

// 内部辅助函数（为了统一性也改为驼峰命名法）
RBNode* RBTreeMinimum(RBTree *tree, RBNode *x); // 修改为驼峰命名

/**
 * @brief 查找并返回给定节点的后继节点。
 *
 * @param tree 指向红黑树的指针。
 * @param x 当前节点。
 * @return 后继节点，如果没有则返回nil节点。
 */
RBNode* RBTreeSuccessor(RBTree *tree, RBNode *x);

#endif //NEWDBMS_RBT_H
