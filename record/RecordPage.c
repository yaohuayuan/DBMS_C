//
// Created by yaohuayuan on 2024/11/14.
//

#include "RecordPage.h"
RecordPage * RecordPageInit(Transaction*transaction,BlockID blockId,Layout*layout){
    RecordPage *recordPage = malloc(sizeof (RecordPage));
    recordPage->layout = layout;
    BlockID_Init(&recordPage->blockId,blockId.fileName,blockId.blockId);
    recordPage->transaction = transaction;
    TransactionPin(transaction,blockId);
    return recordPage;
}
int RecordPageOffset(RecordPage *recordPage,int slot){
    return slot*recordPage->layout->SlotSize;
}
int RecordPageGetInt(RecordPage *recordPage,int slot,char *fldName){
    int fldPos = RecordPageOffset(recordPage,slot)+ LayoutOffset(recordPage->layout,fldName);
    return TransactionGetInt(recordPage->transaction,recordPage->blockId,fldPos);
}
char *RecordPageGetString(RecordPage *recordPage,int slot,char*fldName){
    int fldPos = RecordPageOffset(recordPage,slot)+ LayoutOffset(recordPage->layout,fldName);
    return TransactionGetString(recordPage->transaction,recordPage->blockId,fldPos);
}
void RecordSetInt(RecordPage*recordPage,int slot,char *fldName,int val){
    int fldPos = RecordPageOffset(recordPage,slot)+ LayoutOffset(recordPage->layout,fldName);
    TransactionSetInt(recordPage->transaction,recordPage->blockId,fldPos,val,true);
}
void RecordSetString(RecordPage*recordPage,int slot,char *fldName,char* val){
    int fldPos = RecordPageOffset(recordPage,slot)+ LayoutOffset(recordPage->layout,fldName);
    TransactionSetString(recordPage->transaction,recordPage->blockId,fldPos,val,true);
}
void RecordPageSetFlag(RecordPage*recordPage,int slot,int flag){
    TransactionSetInt(recordPage->transaction,recordPage->blockId, RecordPageOffset(recordPage,slot),flag,true);
}
void RecordPageDelete(RecordPage*recordPage,int slot){
    RecordPageSetFlag(recordPage,slot,RECORD_PAGE_EMPTY);
}
bool RecordPageIsValidSlot(RecordPage*recordPage,int slot){
    return RecordPageOffset(recordPage,slot+1)<=recordPage->transaction->fileManager->blockSize;
}
int RecordPageSearchAfter(RecordPage*recordPage,int slot,int flag){
    slot++;
    while (RecordPageIsValidSlot(recordPage,slot)){
        if(TransactionGetInt(recordPage->transaction,recordPage->blockId, RecordPageOffset(recordPage,slot))==flag){
            return slot;
        }
        slot++;
    }
    return -1;
}
void RecordPageFormat(RecordPage *recordPage){
    int slot = 0;
    while (RecordPageIsValidSlot(recordPage,slot)){
        TransactionSetInt(recordPage->transaction,recordPage->blockId, RecordPageOffset(recordPage,slot),RECORD_PAGE_EMPTY,false);
        Schema *schema = recordPage->layout->schema;
        FieldNode*fieldNode = schema->fields->next;
        while (fieldNode!=NULL){
            char *fldName = fieldNode->fileName;
            int fldPos = RecordPageOffset(recordPage,slot)+ LayoutOffset(recordPage->layout,fldName);
            if(SchemaType(schema,fldName)==FILE_INFO_CODE_INTEGER){
                TransactionSetInt(recordPage->transaction,recordPage->blockId,fldPos,0,false);
            }else if(SchemaType(schema,fldName)==FILE_INFO_CODE_VARCHAR){
                TransactionSetString(recordPage->transaction,recordPage->blockId,fldPos,"",false);
            }
            fieldNode = fieldNode->next;
        }
        slot++;
    }
}
int RecordPageInsertAfter(RecordPage*recordPage,int slot){
    int newSlot = RecordPageSearchAfter(recordPage,slot,RECORD_PAGE_EMPTY);
    if(newSlot>=0){
        RecordPageSetFlag(recordPage,newSlot,RECORD_PAGE_USED);
    }
    return newSlot;
}
int RecordPageNextAfter(RecordPage*recordPage,int slot){
    return RecordPageSearchAfter(recordPage,slot,RECORD_PAGE_USED);
}