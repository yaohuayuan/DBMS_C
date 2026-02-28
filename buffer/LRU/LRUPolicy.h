//
// Created by Lenovo on 2026/1/13.
//

#ifndef NEWDBMS_LRUPOLICY_H
#define NEWDBMS_LRUPOLICY_H
#include"ReplacementPolicy.h"
/**
 * 创建一个新的 LRU 替换策略实例。
 *
 * @param capacity 缓存容量。
 * @return 返回初始化后的 ReplacementPolicy 指针。
 */
ReplacementPolicy* LRUPolicyCreate(int capacity) ;
#endif //NEWDBMS_LRUPOLICY_H