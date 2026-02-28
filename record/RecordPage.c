//
// Created by yaohuayuan on 2024/11/14.
//

#include "RecordPage.h"
RecordPage * RecordPageInit(Transaction*transaction,BlockID *blockId,Layout*layout){
    RecordPage *recordPage = malloc(sizeof (RecordPage));
    recordPage->layout = layout;
    recordPage->blockId = blockId;
    recordPage->transaction = transaction;
    TransactionPin(transaction,blockId);
    return recordPage;
}
int RecordPageOffset(RecordPage *recordPage,int slot){
    int offset = slot * recordPage->layout->SlotSize;
//    printf("DEBUG: RecordPageOffset - slot %d, SlotSize %d, offset %d\n",
//           slot, recordPage->layout->SlotSize, offset);
    return slot*recordPage->layout->SlotSize;

}
int RecordPageGetInt(RecordPage *recordPage,int slot,CString *fldName){
    int fldPos = RecordPageOffset(recordPage,slot)+ LayoutOffset(recordPage->layout,fldName);
    return TransactionGetInt(recordPage->transaction,recordPage->blockId,fldPos);
}
 const char *RecordPageGetString(RecordPage *recordPage,int slot,CString*fldName){
    int fldPos = RecordPageOffset(recordPage,slot)+ LayoutOffset(recordPage->layout,fldName);
    return CStringGetPtr(TransactionGetString(recordPage->transaction,recordPage->blockId,fldPos)) ;
}
void RecordSetInt(RecordPage*recordPage,int slot,CString *fldName,int val){
    int fldPos = RecordPageOffset(recordPage,slot)+ LayoutOffset(recordPage->layout,fldName);
    TransactionSetInt(recordPage->transaction,recordPage->blockId,fldPos,val,true);
}
void RecordSetString(RecordPage*recordPage,int slot,CString *fldName,CString* val){
    int fldPos = RecordPageOffset(recordPage,slot)+ LayoutOffset(recordPage->layout,fldName);
    TransactionSetString(recordPage->transaction,recordPage->blockId,fldPos, val,true);
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
int RecordPageSearchAfter(RecordPage*recordPage, int slot, int flag) {
    slot++;
//    printf("DEBUG: RecordPageSearchAfter - ��ʼ������slot %d, ����flag %d\n", slot, flag);

    while (RecordPageIsValidSlot(recordPage, slot)) {
        int offset = RecordPageOffset(recordPage, slot);
        int currentFlag = TransactionGetInt(recordPage->transaction, recordPage->blockId, offset);

//        printf("DEBUG: slot %d, offset %d, ��ȡ����flag %d, ����flag %d\n",
//               slot, offset, currentFlag, flag);

        if(currentFlag == flag) {
//            printf("DEBUG: �ҵ�ƥ���slot %d\n", slot);
            return slot;
        }
        slot++;
    }
//    printf("DEBUG: û���ҵ�ƥ���slot\n");
    return -1;
}
void RecordPageFormat(RecordPage *recordPage){
    int slot = 0;
    while (RecordPageIsValidSlot(recordPage,slot)){
        TransactionSetInt(recordPage->transaction,recordPage->blockId, RecordPageOffset(recordPage,slot),RECORD_PAGE_EMPTY,false);
        Schema *schema = recordPage->layout->schema;
        FieldNode*fieldNode = schema->fields;
        while (fieldNode!=NULL){
            int offset = RecordPageOffset(recordPage, slot);
//            printf("DEBUG: ��ʽ��slot %d, offset %d, ����flagΪ %d\n",
//                   slot, offset, RECORD_PAGE_EMPTY);
            CString *fldName = fieldNode->fileName;
            int fldPos = RecordPageOffset(recordPage,slot)+ LayoutOffset(recordPage->layout,fldName);
            if(SchemaType(schema,fldName)==FILE_INFO_CODE_INTEGER){
                TransactionSetInt(recordPage->transaction,recordPage->blockId,fldPos,0,false);
            }else{
                TransactionSetString(recordPage->transaction,recordPage->blockId,fldPos, CStringCreateFromCStr(""),false);
            }
            fieldNode = fieldNode->next;
        }
        slot++;
    }
//    printf("DEBUG: RecordPageFormat - ��ʽ�����\n");
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