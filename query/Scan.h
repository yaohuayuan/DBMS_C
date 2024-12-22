//
// Created by yaohuayuan on 2024/11/19.
//

#ifndef DBMS_C_SCAN_H
#define DBMS_C_SCAN_H

#include "Constant.h"
#include "Schema.h"
#include "TableScan.h"
#include "ProductScan.h"
#include <stdbool.h>
#include "SelectScan.h"
#include "ProjectScan.h"
typedef struct ProjectScan ProjectScan;
typedef struct TableScan TableScan;
typedef struct ProductScan ProductScan;
typedef struct SelectScan SelectScan;
typedef struct Scan Scan;
typedef struct RID RID;
typedef struct ScanOps {
    void (*beforeFirst)(void *scan);
    bool (*next)(void *scan);
    int (*getInt)(void *scan,  char *fldname);
    char* (*getString)(void *scan,  char *fldname);
    Constant* (*getVal)(void *scan,  char *fldname);
    bool (*hasField)(void *scan,  char *fldname);
    void (*close)(void *scan);
} ScanOps;
typedef enum ScanCode {
    SCAN_TABLE_CODE,
    SCAN_PRODUCT_CODE,
    SCAN_SELECT_CODE,
    SCAN_PROJECT_CODE,
} ScanCode;

typedef union ScanUnion {
    TableScan *tableScan;
    ProductScan*productScan;
    SelectScan *selectScan;
    ProjectScan *projectScan;
} ScanUnion;
typedef struct Scan {
    void (*beforeFirst)(void *scan);
    bool (*next)(void *scan);
    int (*getInt)(void *scan,  char *fldname);
    char* (*getString)(void *scan,  char *fldname);
    Constant* (*getVal)(void *scan,  char *fldname);
    bool (*hasField)(void *scan,  char *fldname);
    void (*close)(void *scan);
    void (*setVal)(void *data,char *fldname,Constant*constant);
    void (*setInt)(void *data,char *fldname,int val);
    void (*setString)(void *data,char *fldname,char *val);
    void (*insert)(void*data);
    void (*delete)(void *data);
    RID *(*getRid)(void *data);
    void (*moveToRID)(void *data,RID *rid);
    ScanUnion scanUnion;
    ScanCode code;
} Scan;

Scan *ScanInit(void *data,ScanCode code);
#endif //DBMS_C_SCAN_H
