//
// Created by Lenovo on 2026/1/13.
//

#include "LRUPolicy.h"

#include <stdlib.h>
#include"LRUCore.h"
typedef struct LRUNode LRUNode;

typedef struct LRUImpl{
    LRUCore* core;
    LRUList *list;
} LRUImpl;
static void lru_record_access(void* impl, int frame_id) {
   LRUImpl* lru_impl = (LRUImpl*)impl;
    LRUCore* core = lru_impl->core;
    LRUCoreAccess(core, frame_id);
}
static int lru_evict(void* impl) {
    LRUImpl* lru = (LRUImpl*)impl;
    LRUCore* core = lru->core;
    int key=LRUCoreVictim(core);

    return key;
}
static void lru_remove(void* impl, int frame_id) {
    LRUImpl* lru = (LRUImpl*)impl;
    LRUCore* core = lru->core;
    LRUCoreRemove(core, frame_id);
}
static void lru_destroy(void* impl) {
    LRUImpl* lru = (LRUImpl*)impl;
    LRUCoreDestroy(lru->core);
    free(lru);
}
ReplacementPolicy* LRUPolicyCreate(int capacity) {
    ReplacementPolicy* policy =
        (ReplacementPolicy*)malloc(sizeof(ReplacementPolicy));
    LRUImpl* impl = (LRUImpl*)malloc(sizeof(LRUImpl));
    impl->core = LRUCoreCreate();
    policy->impl = impl;
    policy->record_access = lru_record_access;
    policy->evict = lru_evict;
    policy->remove = lru_remove;
    policy->destroy = lru_destroy;
    return policy;
}
