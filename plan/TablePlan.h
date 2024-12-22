//
// Created by yaohuayuan on 2024/11/30.
//

#ifndef DBMS_C_TABLEPLAN_H
#define DBMS_C_TABLEPLAN_H
#include "Plan.h"
#include "Transaction.h"
#include "Layout.h"
#include "StatInfo.h"
#include "MetadataManager.h"
typedef struct TablePlan{
    char *tblname;
    Transaction *tx;
    Layout *layout;
    StatInfo *si;
}TablePlan;
Scan *TablePlanOpen(void*data);
TablePlan *TablePlanInit(Transaction*tx,char *tblname,MetadataMgr*metadataMgr);
int TablePlanBlocksAccessed(void *data);
int TablePlanRecordsOutput(void *data);
int TablePlanDistinctValues(void *data,char *fldname);
Schema *TablePlanSchema(void *data);
#endif //DBMS_C_TABLEPLAN_H
