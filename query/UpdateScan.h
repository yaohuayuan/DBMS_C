//
// Created by yaohuayuan on 2024/11/19.
//

#ifndef DBMS_C_UPDATESCAN_H
#define DBMS_C_UPDATESCAN_H
#ifndef UPDATE_SCAN_H
#define UPDATE_SCAN_H

#include "Scan.h"
#include "Constant.h"
typedef struct UpdateScanOPS{
    void (*setVal)(void *data,CString *fldname,Constant*constant);
    void (*setInt)(void *data,CString *fldname,int val);
    void (*setString)(void *data,CString *fldname,CString *val);
    void (*insert)(void*data);
    void (*delete)(void *data);
    RID *(*getRid)(void *data);
    void (*moveToRID)(void *data,RID *rid);
}UpdateScanOPS;
typedef struct UpdateScan {
    UpdateScanOPS *updateScanOps;

}UpdateScan;
#endif // UPDATE_SCAN_H
#endif //DBMS_C_UPDATESCAN_H
