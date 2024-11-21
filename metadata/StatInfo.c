//
// Created by yaohuayuan on 2024/11/19.
//

#include "StatInfo.h"
#include <stdlib.h>

StatInfo* StatInfoInit(int numBlocks, int numRecs) {
    StatInfo* si = (StatInfo*)malloc(sizeof(StatInfo));
    si->numBlocks = numBlocks;
    si->numRecs = numRecs;
    return si;
}

int StatInfoBlocksAccessed(StatInfo* si) {
    return si->numBlocks;
}

int StatInfoRecordsOutput(StatInfo* si) {
    return si->numRecs;
}

int StatInfoDistinctValues(StatInfo* si, const char* fldname) {
    return 1 + (si->numRecs / 3);
}
