//
// Created by yaohuayuan on 2024/11/14.
//

#ifndef DBMS_C_RECORDPAGE_H
#define DBMS_C_RECORDPAGE_H
#include "Transaction.h"
#include "Layout.h"
typedef enum RecordPageCode{
    RECORD_PAGE_EMPTY,
    RECORD_PAGE_USED
}RecordCode;
typedef struct RecordPage{
    Transaction *transaction;
    BlockID blockId;
    Layout *layout;
}RecordPage;
RecordPage * RecordPageInit(Transaction*transaction,BlockID blockId,Layout*layout);
int RecordPageOffset(RecordPage *recordPage,int slot);
int RecordPageGetInt(RecordPage *recordPage,int slot,char *fldName);
char *RecordPageGetString(RecordPage *recordPage,int slot,char*fldName);
void RecordSetInt(RecordPage*recordPage,int slot,char *fldName,int val);
void RecordSetString(RecordPage*recordPage,int slot,char *fldName,char* val);
void RecordPageSetFlag(RecordPage*recordPage,int slot,int flag);
void RecordPageDelete(RecordPage*recordPage,int slot);
bool RecordPageIsValidSlot(RecordPage*recordPage,int slot);
int RecordPageSearchAfter(RecordPage*recordPage,int slot,int flag);
void RecordPageFormat(RecordPage *recordPage);
int RecordPageInsertAfter(RecordPage*recordPage,int slot);
int RecordPageNextAfter(RecordPage*recordPage,int slot);
#endif //DBMS_C_RECORDPAGE_H
