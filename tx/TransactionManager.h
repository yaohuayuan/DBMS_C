//
// Created by yaohuayuan on 2024/12/19.
//
#ifndef DBMS_C_TRANSACTIONMANAGER_H
#define DBMS_C_TRANSACTIONMANAGER_H

#include "Transaction.h"
#include "map.h"

// 事务状态枚举


// 事务数据结构
typedef struct TransactionData {
    Transaction* transaction;  // 事务指针
} TransactionData;

// 定义一个事务数据类型的map
typedef map_t(TransactionData) map_TransactionData_t;

// 事务管理器结构
typedef struct TransactionManager {
    map_TransactionData_t* mapTransactionData;  // 事务映射表
} TransactionManager;

// 函数声明
TransactionManager* TransactionManagerInit();  // 初始化事务管理器
void TransactionManagerAdd(TransactionManager* txnManager, Transaction* txn);  // 添加事务
void TransactionManagerDisplay(TransactionManager* txnManager);  // 展示事务信息
Transaction *TransactionManagerSwitch(TransactionManager* txnManager, char * txnId);  // 切换事务
void TransactionManagerCommit(TransactionManager* txnManager);
void TransactionManagerRollback(TransactionManager* txnManager);
#endif // DBMS_C_TRANSACTIONMANAGER_H
