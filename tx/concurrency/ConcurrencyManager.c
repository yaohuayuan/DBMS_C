//
// Created by yaohuayuan on 2024/11/4.
//

#include "ConcurrencyManager.h"
#include "CString.h"
static LockTable *lockTable = NULL;
ConCurrencyManager * ConCurrencyManagerInit(){
    ConCurrencyManager *conCurrencyManager = malloc(sizeof (ConCurrencyManager));
    conCurrencyManager->mapStr = malloc(sizeof(map_str_t));
    map_init(conCurrencyManager->mapStr);
    if(lockTable == NULL){
        lockTable = LockTableInit();
    }
    return conCurrencyManager;
}
void ConCurrencyManagerSLock(ConCurrencyManager *conCurrencyManager,BlockID *blockId){
    BlockID *blockId1 = BlockIDInit(blockId->fileName,blockId->BlockID);
    CString  * key = BlockID2CString(blockId1);
    char** value = map_get(conCurrencyManager->mapStr, CStringGetPtr(key));
    Error *error = ErrorInit();
    if(value==NULL){
        LockTableSLock(lockTable,blockId,error);
        map_set(conCurrencyManager->mapStr,CStringGetPtr(BlockID2CString(blockId)),"S");
    }
}
bool ConCurrencyManagerHasXLock(ConCurrencyManager *conCurrencyManager,BlockID *blockId){
    char** LockType = map_get(conCurrencyManager->mapStr, CStringGetPtr(BlockID2CString(blockId)) );
    return LockType!=NULL&&(strcmp(*LockType,"X")==0);
}
void ConCurrencyManagerXLock(ConCurrencyManager *mgr, BlockID *blockId) {
    Error *error = ErrorInit();

    if (ConCurrencyManagerHasXLock(mgr, blockId))
        return; // 已经是 X 锁

    char **lockType = map_get(mgr->mapStr, CStringGetPtr(BlockID2CString(blockId)));
    if (lockType && strcmp(*lockType, "S") == 0) {
        // 升级
        LockTableUpgrade(lockTable, blockId, error);
        if (error->errorCode == 0) {
            map_set(mgr->mapStr, CStringGetPtr(BlockID2CString(blockId)), "X");
        }
    } else {
        // 直接获取 X 锁
        LockTableXLock(lockTable, blockId, error);
        if (error->errorCode == 0) {
            map_set(mgr->mapStr, CStringGetPtr(BlockID2CString(blockId)), "X");
        }
    }
}

void ConCurrencyManagerRelease(ConCurrencyManager *conCurrencyManager){
    const char *key;
    map_iter_t iter = map_iter(conCurrencyManager->mapStr);
    const char *keys_to_remove[conCurrencyManager->mapStr->base.nnodes];  // 临时存储要删除的键
    int index = 0;
    while ((key = map_next(conCurrencyManager->mapStr, &iter))) {
//        printf("[DEBUG] Releasing lock for key: %s\n", key);  // 调试打印
        LockTableUnLock(lockTable,  BlockIDCString2BlockID(CStringCreateFromCStr(key)));
        keys_to_remove[index++] = key;
    }

// 遍历结束后逐一删除记录的键
    for (int i = 0; i < index; i++) {
        map_remove(conCurrencyManager->mapStr, keys_to_remove[i]);
    }

}