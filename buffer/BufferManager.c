//
// Created by Lenovo on 2025/7/16.
//
#include "BufferManager.h"
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "LRU/LRUPolicy.h"

Buffer *BufferManagerFindExistingBuffer(BufferManager *bm, BlockID *blockId){
    for(int i = 0; i < bm->bufferSize; i++){
        Buffer *buf = *(Buffer **)CVectorAt(bm->bufferPool, i);
        if(buf->blockId && BlockIDEqual(blockId, buf->blockId)){
            return buf;
        }
    }
    return NULL;
}

 bool BufferManagerWaitTooLong(long start){
    return time(NULL) - start > MAX_TIME;
}

Buffer *BufferManagerTryToPin(BufferManager *bm, BlockID *blockId) {
    // 1. 已存在？
    Buffer *buffer = BufferManagerFindExistingBuffer(bm, blockId);
    if (buffer != NULL) {
        if (buffer->pins == 0)
            bm->numAvailable--;
        bm->policy->record_access(bm->policy->impl, buffer->frame_id);
        BufferPin(buffer);
        return buffer;
    }

    // 2. 找空闲 buffer
    buffer = BufferManagerChooseUnPinnedBuffer(bm);

    // 3. 如果没有空闲，再 evict
    if (buffer == NULL) {
        int fid = bm->policy->evict(bm->policy->impl);
        if (fid < 0)
            return NULL;

        buffer = *(Buffer**)CVectorAt(bm->bufferPool, fid);
        if (BufferIsPinned(buffer))
            return NULL;
    }

    // 4. 使用这个 buffer
    BufferAssignToBlock(buffer, blockId);
    bm->numAvailable--;
    bm->policy->record_access(bm->policy->impl, buffer->frame_id);
    BufferPin(buffer);
    return buffer;
}


BufferManager *BufferManagerInit(
    FileManager *fileManager,
    LogManager *logManager,
    int numBuffs,
    ReplacementPolicy *policy
){
    BufferManager *bm = malloc(sizeof(BufferManager));
    bm->bufferSize = numBuffs;
    bm->numAvailable = numBuffs;
    if (policy)
        bm->policy = policy;
    else{
        bm->policy = LRUPolicyCreate(numBuffs);
    }
    bm->bufferPool = CVectorInit(sizeof(Buffer *), NULL, NULL, NULL);
    for(int i = 0; i < numBuffs; i++){
        Buffer *buf = BufferInit(fileManager, logManager);
        buf->frame_id = i;
        CVectorPushBack(bm->bufferPool, &buf);
    }
    return bm;
}

void
BufferManagerFlushAll(BufferManager *bm, int tx){
    for(int i = 0; i < bm->bufferSize; i++){
        Buffer *buf = *(Buffer **)CVectorAt(bm->bufferPool, i);
        if(buf->blockId && buf->txNum == tx)
            BufferFlush(buf);
    }
}

void
BufferManagerUnpin(BufferManager *bm, Buffer *buffer){
    int wasPinned = BufferIsPinned(buffer);
    BufferUnPin(buffer);

    if(wasPinned && buffer->pins == 0){
        bm->numAvailable++;
        bm->policy->remove(bm->policy->impl, buffer->frame_id);
    }
}

Buffer *BufferManagerPin(BufferManager *bm, BlockID *blockId){
    long start = time(NULL);
    Buffer *buf;

    while((buf = BufferManagerTryToPin(bm, blockId)) == NULL){
        if(BufferManagerWaitTooLong(start))
            return NULL;
        sleep(1);
    }
    return buf;
}
Buffer* BufferManagerChooseUnPinnedBuffer(BufferManager* bm) {
    for (int i = 0; i < bm->bufferSize; i++) {
        Buffer* buf = *(Buffer**)CVectorAt(bm->bufferPool, i);
        if (!BufferIsPinned(buf)) {
            return buf;
        }
    }
    return NULL;
}
