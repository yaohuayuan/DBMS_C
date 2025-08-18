// DeadlockDetector.c  (覆盖原文件即可)
#include "DeadlockDetector.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* ------------- 内部工具 ------------- */
static char *itoa_buf(int n, char buf[16]) {
    sprintf(buf, "%d", n);
    return buf;
}

/* ------------- 初始化 / 销毁 ------------- */
DeadlockDetector *DeadlockDetectorInit(void) {
    DeadlockDetector *d = calloc(1, sizeof(*d));
    d->lockHolders = malloc(sizeof(map_int_t));
    map_init(d->lockHolders);
    d->waitForList = NULL;
    return d;
}

void DeadlockDetectorDestroy(DeadlockDetector *d) {
    if (!d) return;
    map_deinit(d->lockHolders);
    free(d->lockHolders);

    WaitForEdge *cur = d->waitForList;
    while (cur) {
        WaitForEdge *next = cur->next;
        free(cur);
        cur = next;
    }
    free(d);
}

/* ------------- 死锁检测：三色 DFS ------------- */
static bool dfsColor(int tx,
                     char *color,
                     const DeadlockDetector *d) {
    if (color[tx] == 1) return true;   // 发现回边
    if (color[tx] == 2) return false;

    color[tx] = 1;                     // 标记为“访问中”
    for (WaitForEdge *e = d->waitForList; e; e = e->next) {
        if (e->fromTx == tx && dfsColor(e->toTx, color, d))
            return true;
    }
    color[tx] = 2;                     // 标记为“已结束”
    return false;
}

bool DetectDeadlock(DeadlockDetector *d) {
    /* 事务号范围 < 1024，若后续扩容请把 1024 改为动态值 */
    char color[1024] = {0};
    bool found = false;
    for (WaitForEdge *e = d->waitForList; e; e = e->next) {
        if (dfsColor(e->fromTx, color, d)) {
            found = true;
            break;
        }
    }
    return found;
}

/* ------------- 边增删 ------------- */
void AddWaitForEdge(DeadlockDetector *d,
                    int fromTx, int toTx, BlockID *blockId) {
    WaitForEdge *e = malloc(sizeof(*e));
    *e = (WaitForEdge){.fromTx = fromTx,
            .toTx   = toTx,
            .blockId = blockId,
            .next   = d->waitForList};
    d->waitForList = e;
}

void RemoveWaitForEdges(DeadlockDetector *d,
                        int tx, BlockID *blockId) {
    WaitForEdge **pp = &d->waitForList;
    while (*pp) {
        WaitForEdge *cur = *pp;
        if (cur->fromTx == tx &&
            (blockId == NULL || BlockIDEqual(cur->blockId, blockId))) {
            *pp = cur->next;
            free(cur);
        } else {
            pp = &cur->next;
        }
    }
}