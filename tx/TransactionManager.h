//
// Created by yaohuayuan on 2024/12/19.
//
#ifndef DBMS_C_TRANSACTIONMANAGER_H
#define DBMS_C_TRANSACTIONMANAGER_H

#include "Transaction.h"
#include "map.h"

typedef struct TransactionData {
    Transaction* transaction;
} TransactionData;

typedef map_t(TransactionData) map_TransactionData_t;

typedef struct TransactionManager {
    map_TransactionData_t* mapTransactionData;
} TransactionManager;


TransactionManager* TransactionManagerInit();  // 初始化事务管理器
void TransactionManagerAdd(TransactionManager* txnManager, Transaction* txn);  // 添加事务
void TransactionManagerDisplay(TransactionManager* txnManager);  // 展示事务信息
Transaction *TransactionManagerSwitch(TransactionManager* txnManager, char * txnId);  // 切换事务
void TransactionManagerCommit(TransactionManager* txnManager);
void TransactionManagerRollback(TransactionManager* txnManager);
#endif // DBMS_C_TRANSACTIONMANAGER_H
