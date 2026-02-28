//
// Created by yaohuayuan on 2024/11/10.
//

#ifndef DBMS_C_TRANSACTION_H
#define DBMS_C_TRANSACTION_H
#include "concurrency/ConcurrencyManager.h"
#include "BufferManager.h"
#include "BufferList.h"
# define Transaction_END_OF_FILE (-1)
typedef struct RecoveryManager RecoveryManager;
/**
 * @brief TransactionStatus 枚举，定义了事务的状态。
 */
typedef enum {
    TX_TRANSACTION_COMMIT,  ///< 事务已提交
    TX_TRANSACTION_ROLLBACK,  ///< 事务已回滚
    TX_TRANSACTION_RUN,  ///< 事务正在运行
    TX_TRANSACTION_RECOVERY,  ///< 事务正在恢复
} TransactionStatus;

/**
 * @brief Transaction 结构体，用于表示数据库事务。
 */
typedef struct Transaction{
    TransactionStatus code;  ///< 事务状态
    RecoveryManager *recoveryManager;  ///< 恢复管理器
    ConCurrencyManager *conCurrencyManager;  ///< 并发管理器
    BufferManager *bufferManager;  ///< 缓冲管理器
    FileManager *fileManager;  ///< 文件管理器
    int txNum;  ///< 事务编号
    BufferList *bufferList;  ///< 缓冲区列表
}Transaction;

/**
 * @brief 初始化一个新的 Transaction 实例。
 *
 * @param fileManager 文件管理器指针。
 * @param logManager 日志管理器指针。
 * @param bufferManager 缓冲管理器指针。
 * @return 返回初始化后的 Transaction 指针。
 */
Transaction* TransactionInit(FileManager*fileManager, LogManager*logManager, BufferManager*bufferManager);

/**
 * @brief 提交事务。
 *
 * @param transaction 指向 Transaction 实例的指针。
 */
void TransactionCommit(Transaction*transaction);

/**
 * @brief 回滚事务。
 *
 * @param transaction 指向 Transaction 实例的指针。
 */
void TransactionRollback(Transaction*transaction);

/**
 * @brief 恢复事务。
 *
 * @param transaction 指向 Transaction 实例的指针。
 */
void TransactionRecover(Transaction*transaction);

/**
 * @brief 固定指定块。
 *
 * @param transaction 指向 Transaction 实例的指针。
 * @param blockId 块 ID。
 */
void TransactionPin(Transaction *transaction,BlockID *blockId);

/**
 * @brief 取消固定指定块。
 *
 * @param transaction 指向 Transaction 实例的指针。
 * @param blockId 块 ID。
 */
void TransactionUnPin(Transaction *transaction,BlockID *blockId);

/**
 * @brief 从指定块中获取整数值。
 *
 * @param transaction 指向 Transaction 实例的指针。
 * @param blockId 块 ID。
 * @param offset 偏移量。
 * @return 返回读取的整数值。
 */
int TransactionGetInt(Transaction*transaction,BlockID *blockId,int offset);

/**
 * @brief 从指定块中获取字符串值。
 *
 * @param transaction 指向 Transaction 实例的指针。
 * @param blockId 块 ID。
 * @param offset 偏移量。
 * @return 返回读取的字符串值。
 */
CString *TransactionGetString(Transaction*transaction,BlockID *blockId,int offset);

/**
 * @brief 在指定块中设置整数值。
 *
 * @param transaction 指向 Transaction 实例的指针。
 * @param blockId 块 ID。
 * @param offset 偏移量。
 * @param val 要设置的整数值。
 * @param okToLog 是否记录日志。
 */
void TransactionSetInt(Transaction*transaction,BlockID *blockId,int offset,int val,bool okToLog);

/**
 * @brief 在指定块中设置字符串值。
 *
 * @param transaction 指向 Transaction 实例的指针。
 * @param blockId 块 ID。
 * @param offset 偏移量。
 * @param val 要设置的字符串值。
 * @param okToLog 是否记录日志。
 */
void TransactionSetString(Transaction*transaction,BlockID *blockId,int offset,CString * val,bool okToLog);

/**
 * @brief 获取指定文件的大小。
 *
 * @param transaction 指向 Transaction 实例的指针。
 * @param fileName 文件名。
 * @return 返回文件大小。
 */
int TransactionSize(Transaction *transaction, CString *fileName);

/**
 * @brief 向指定文件追加新块。
 *
 * @param transaction 指向 Transaction 实例的指针。
 * @param fileName 文件名。
 * @return 返回新块的 BlockID。
 */
BlockID* TransactionAppend(Transaction *transaction,CString*fileName);

/**
 * @brief 获取块大小。
 *
 * @param transaction 指向 Transaction 实例的指针。
 * @return 返回块大小。
 */
int TransactionBlockSize(Transaction*transaction);

/**
 * @brief 获取可用缓冲区数量。
 *
 * @param transaction 指向 Transaction 实例的指针。
 * @return 返回可用缓冲区数量。
 */
int TransactionAvailableBuffs(Transaction*transaction);

/**
 * @brief 将事务转换为字符串表示。
 *
 * @param transaction 指向 Transaction 实例的指针。
 * @return 返回事务的字符串表示。
 */
CString * TransactionToSting(Transaction*transaction);
#endif //DBMS_C_TRANSACTION_H
