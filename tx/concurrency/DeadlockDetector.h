//
// Created by Lenovo on 2025/8/13.
//

#ifndef NEWDBMS_DEADLOCKDETECTOR_H
#define NEWDBMS_DEADLOCKDETECTOR_H
#include "map.h"
#include "BlockID.h"
typedef struct WaitForEdge {
    int fromTx;             // 等待的事务ID
    int toTx;               // 持有锁的事务ID
    BlockID* blockId;       // 被等待的区块ID
    struct WaitForEdge* next;
} WaitForEdge;
typedef struct {
    map_int_t* lockHolders; // 每个锁的持有者（X锁）或持有者计数（S锁）
    WaitForEdge* waitForList; // 等待关系链表
} DeadlockDetector;
bool DetectDeadlock(DeadlockDetector* detector);
void AddWaitForEdge(DeadlockDetector* detector, int fromTx, int toTx, BlockID* blockId);
void RemoveWaitForEdges(DeadlockDetector* detector, int txNum, BlockID* blockId);
DeadlockDetector* DeadlockDetectorInit();
#endif //NEWDBMS_DEADLOCKDETECTOR_H
