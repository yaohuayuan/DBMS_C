//
// Created by yaohuayuan on 2024/11/29.
//

#ifndef DBMS_C_CREATEINDEXDATA_H
#define DBMS_C_CREATEINDEXDATA_H
typedef struct CreateIndexData{
    char *idxname,*tblname,*fldname;
}CreateIndexData;
CreateIndexData *CreateIndexDataInit(char *idxname,char *tblname,char *fldname);

#endif //DBMS_C_CREATEINDEXDATA_H
