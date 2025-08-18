//
// Created by yaohuayuan on 2024/12/19.
//

#include "TransactionManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 初始化事务管理器
TransactionManager* TransactionManagerInit() {
    TransactionManager* txnManager = malloc(sizeof(TransactionManager));
    txnManager->mapTransactionData = malloc(sizeof(map_TransactionData_t));
    map_init(txnManager->mapTransactionData);  // 初始化map
    return txnManager;
}

// 创建并添加一个事务
char *int2str(int num){
    char *str;
    int length;
    length = snprintf(NULL, 0, "%d", num);

    str = malloc(length + 1);

    if (str == NULL) {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    sprintf(str, "%d", num);
    return str;
}
void TransactionManagerAdd(TransactionManager* txnManager, Transaction* txn) {
    if (txn == NULL) {
        printf("Invalid transaction.\n");
        return;
    }

    // 创建事务数据结构
    TransactionData txnData;
    txnData.transaction = txn;

    int id = txnData.transaction->txNum;
    // 将事务加入到事务管理器中
    map_set(txnManager->mapTransactionData, int2str(id), txnData);
    printf("Transaction %s added to the manager.\n", int2str(id));
}

// 展示事务信息
void TransactionManagerDisplay(TransactionManager* txnManager) {
    const char* txnId;
    map_iter_t iter = map_iter(txnManager->mapTransactionData);
    printf("Active Transactions:\n");

    // 遍历所有事务
    while ((txnId = map_next(txnManager->mapTransactionData, &iter))) {
        TransactionData* txnData = map_get(txnManager->mapTransactionData, txnId);
        Transaction* txn = txnData->transaction;  // 获取事务对象

        // 根据事务的状态输出不同的结果
        const char* status = NULL;
        switch (txn->code) {
            case TX_TRANSACTION_COMMIT:
                status = "Committed";
                break;
            case TX_TRANSACTION_ROLLBACK:
                status = "Rolled Back";
                break;
            case TX_TRANSACTION_RUN:
                status = "Running";
                break;
            case TX_TRANSACTION_RECOVERY:
                status = "Recovery";
                break;
            default:
                status = "Unknown";
        }

        // 打印事务ID和状态
        printf("Transaction ID: %s, Status: %s\n", txnId, status);
    }
}


// 切换当前事务
Transaction *TransactionManagerSwitch(TransactionManager* txnManager, char * txnId) {

    TransactionData* txnData = map_get(txnManager->mapTransactionData, txnId);
    if (txnData == NULL) {
        printf("Transaction %d not found.\n", txnId);
        return NULL;
    }
    printf("Switched to transaction %s.\n", txnId);
    return txnData->transaction;
}
void TransactionManagerCommit(TransactionManager* txnManager){
    const char* txnId;
    map_iter_t iter = map_iter(txnManager->mapTransactionData);
    while ((txnId = map_next(txnManager->mapTransactionData, &iter))) {
        TransactionData* txnData = map_get(txnManager->mapTransactionData, txnId);
        Transaction*transaction = txnData->transaction;
        TransactionCommit(transaction);
    }
}
void TransactionManagerRollback(TransactionManager* txnManager){
    const char* txnId;
    map_iter_t iter = map_iter(txnManager->mapTransactionData);
    while ((txnId = map_next(txnManager->mapTransactionData, &iter))) {
        TransactionData* txnData = map_get(txnManager->mapTransactionData, txnId);
        Transaction*transaction = txnData->transaction;
        TransactionRollback(transaction);
    }
}