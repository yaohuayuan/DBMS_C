//
// Created by yaohuayuan on 2024/11/4.
//

#include "ConcurrencyManager.h"
ConCurrencyManager * ConCurrencyManagerInit(){
    ConCurrencyManager *conCurrencyManager = malloc(sizeof (ConCurrencyManager));
    conCurrencyManager->mapStr = malloc(sizeof(map_str_t));
    map_init(conCurrencyManager->mapStr);
    conCurrencyManager->lockTable = LockTableInit();
    return conCurrencyManager;
}
void ConCurrencyManagerSLock(ConCurrencyManager *conCurrencyManager,BlockID blockId){
    char** value = map_get(conCurrencyManager->mapStr,BlockIDToString(blockId));
    Error *error = ErrorInit();
    if(value==NULL){
        LockTableSLock(conCurrencyManager->lockTable,blockId,error);
        map_set(conCurrencyManager->mapStr,BlockIDToString(blockId),"S");
    }
}
bool ConCurrencyManagerHasXLock(ConCurrencyManager *conCurrencyManager,BlockID blockId){
    char** LockType = map_get(conCurrencyManager->mapStr,BlockIDToString(blockId));
    return LockType!=NULL&&(strcmp(*LockType,"X")==0);
}
void ConCurrencyManagerXLock(ConCurrencyManager *conCurrencyManager,BlockID blockId){
    char** value = map_get(conCurrencyManager->mapStr,BlockIDToString(blockId));
    Error *error = ErrorInit();
    if(ConCurrencyManagerHasXLock(conCurrencyManager,blockId)){
        ConCurrencyManagerSLock(conCurrencyManager,blockId);
        LockTableXLock(conCurrencyManager->lockTable,blockId,error);
        map_set(conCurrencyManager->mapStr,BlockIDToString(blockId),"X");
    }
}
void ConCurrencyManagerRelease(ConCurrencyManager *conCurrencyManager){
    const char *key;
    map_iter_t iter = map_iter(conCurrencyManager->mapStr);
    const char *keys_to_remove[conCurrencyManager->mapStr->base.nnodes];  // 临时存储要删除的键
    int index = 0;

// 遍历所有键并记录
    while ((key = map_next(conCurrencyManager->mapStr, &iter))) {
        LockTableUnLock(conCurrencyManager->lockTable, BloCKIDString2BlockID(key));
        keys_to_remove[index++] = key;
    }

// 遍历结束后逐一删除记录的键
    for (int i = 0; i < index; i++) {
        map_remove(conCurrencyManager->mapStr, keys_to_remove[i]);
    }

}