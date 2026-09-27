#include "CMap.h"
#include <stdlib.h>
#include "string.h"
#include "stdio.h"
int CMapInit(CMap *map,
            size_t keySize,
            size_t valueSize,
            int (*compare)(const void*, const void*),
            void (*keyFree)(void*),
            void (*valueFree)(void*),
            void* (*deepCopyKey)(void*),
            void* (*deepCopyData)(void*)
            ) {
    if (!map || !compare || keySize == 0 || valueSize == 0) {
        return 0;
    }
    map->key_size = keySize;
    map->value_size = valueSize;
    map->DeepCopyKey = deepCopyKey;
    map->DeepCopyData = deepCopyData;
    return RBTreeInit(&map->tree, compare, keyFree, valueFree);
}
int CMapInsert(CMap *map, const void *key, const void *value) {
    if (!map || !key || !value || !map->tree.nil) {
        return 0;
    }
    // 1. 先查是否存在
    RBNode *exist = RBTreeSearch(&map->tree, map->tree.root, (void*)key);
    if (exist != map->tree.nil) {
        return 0; // key 已存在，插入失败
    }

    // 2. 再做深拷贝
    void *key_copy = NULL, *value_copy = NULL;

    if (map->DeepCopyKey) {
        key_copy = map->DeepCopyKey((void*)key);
    } else {
        key_copy = malloc(map->key_size);
        if (key_copy) {
            memcpy(key_copy, key, map->key_size);
        }
    }
    if (!key_copy) {
        return 0;
    }

    if (map->DeepCopyData) {
        value_copy = map->DeepCopyData((void*)value);
    } else {
        value_copy = malloc(map->value_size);
        if (value_copy) {
            memcpy(value_copy, value, map->value_size);
        }
    }
    if (!value_copy) {
        if (map->tree.KeyFree) {
            map->tree.KeyFree(key_copy);
        } else {
            free(key_copy);
        }
        return 0;
    }

    if (!RBTreeInsert(&map->tree, key_copy, value_copy)) {
        if (map->tree.KeyFree) {
            map->tree.KeyFree(key_copy);
        } else {
            free(key_copy);
        }
        if (map->tree.DataFree) {
            map->tree.DataFree(value_copy);
        } else {
            free(value_copy);
        }
        return 0;
    }

    return 1;
}

// Map查找
void* CMapFind(CMap *map, const void *key) {
    if (!map || !key || !map->tree.nil) {
        return NULL;
    }
    RBNode *node = RBTreeSearch(&map->tree, map->tree.root, (void*)key); // 调用红黑树的查找函数

    if (node == map->tree.nil){
        return NULL;
    }

    return node->data;
}

// Map删除
int CMapErase(CMap *map, const void *key) {
    if (!map || !key || !map->tree.nil) {
        return 0;
    }
    return RBTreeDelete(&map->tree, (void*)key); // 调用红黑树的删除函数
}

// 释放Map
void CMapDestroy(CMap *map) {
    if (!map || !map->tree.nil) {
        return;
    }
    RBTreeFree(&map->tree); // 调用红黑树的释放函数
}
CMapIterator CMapIteratorBegin(CMap *map) {
    CMapIterator it;
    it.tree = &map->tree;
    it.node = RBTreeMinimum(&map->tree,map->tree.root); // 获取树中的第一个节点
    return it;
}

CMapIterator CMapIteratorEnd(CMap *map) {
    CMapIterator it;
    it.tree = &map->tree;
    it.node = map->tree.nil; // 结束时，指向nil
    return it;
}

void* CMapIteratorKey(CMapIterator *it) {
    return it->node->key;
}

void* CMapIteratorValue(CMapIterator *it) {
    return it->node->data;
}

void CMapIteratorNext(CMapIterator *it) {
    it->node = RBTreeSuccessor(it->tree, it->node);
}

int CMapIteratorEqual(CMapIterator *it1, CMapIterator *it2) {
    return it1->node == it2->node;
}
int CMapUpdate(CMap *map, const void *key, const void *new_value) {
    if (!map || !key || !new_value || !map->tree.nil) {
        return 0;
    }
    RBNode *node = RBTreeSearch(&map->tree, map->tree.root, (void*)key);
    if (node == map->tree.nil) {
        return 0; // key 不存在
    }

    // 拷贝新值
    void *new_value_copy = NULL;
    if (map->DeepCopyData) {
        new_value_copy = map->DeepCopyData((void*)new_value);
    } else {
        new_value_copy = malloc(map->value_size);
        if (new_value_copy) {
            memcpy(new_value_copy, new_value, map->value_size);
        }
    }
    if (!new_value_copy) {
        return 0;
    }

    // 释放旧值
    if (map->tree.DataFree) {
        map->tree.DataFree(node->data);
    }

    node->data = new_value_copy; // 🔥 关键修复
    return 1;
}
int CMapPut(CMap *map, const void *key, const void *value) {
    if (!map || !key || !value || !map->tree.nil) {
        return 0;
    }
    RBNode *node = RBTreeSearch(&map->tree, map->tree.root, (void*)key);
    if (node != map->tree.nil) {
        // 已存在，更新
        return CMapUpdate(map, key, value);
    } else {
        // 不存在，插入
        return CMapInsert(map, key, value);
    }
}
