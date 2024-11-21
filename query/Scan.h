//
// Created by yaohuayuan on 2024/11/19.
//

#ifndef DBMS_C_SCAN_H
#define DBMS_C_SCAN_H

#include "Constant.h"
#include "Schema.h"
#include <stdbool.h>

typedef struct Scan Scan;
typedef struct UpdateScan UpdateScan;
typedef struct RID RID;

typedef struct ScanOps {
    void (*beforeFirst)(Scan *scan);
    bool (*next)(Scan *scan);
    int (*getInt)(Scan *scan, const char *fldname);
    const char* (*getString)(Scan *scan, const char *fldname);
    Constant* (*getVal)(Scan *scan, const char *fldname);
    bool (*hasField)(Scan *scan, const char *fldname);
    void (*close)(Scan *scan);
} ScanOps;

typedef struct Scan {
    ScanOps *ops;
} Scan;

typedef struct UpdateScanOps {
    ScanOps scanOps;
    void (*setVal)(UpdateScan *scan, const char *fldname, Constant *val);
    void (*setInt)(UpdateScan *scan, const char *fldname, int val);
    void (*setString)(UpdateScan *scan, const char *fldname, const char *val);
    void (*insert)(UpdateScan *scan);
    void (*delete)(UpdateScan *scan);
    RID* (*getRid)(UpdateScan *scan);
    void (*moveToRid)(UpdateScan *scan, const RID *rid);
} UpdateScanOps;

typedef struct UpdateScan {
    Scan scan;
    UpdateScanOps *ops;
} UpdateScan;

typedef struct RID {
    int pageId;
    int slotId;
} RID;


#endif //DBMS_C_SCAN_H
