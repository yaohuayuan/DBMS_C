//
// Created by yaohuayuan on 2024/11/19.
//

#include "TableManager.h"
void TableManagerCreateTable(TableManager *tableManager,CString*tblname,Schema*sch,Transaction*transaction){
    Layout *layout = LayoutInit(sch,NULL,-1);

    CString *tblcatParam = CStringCreateFromCStr("tblcat");
    CString *tblnameParam = CStringCreateFromCStr("tblname");
    CString *slotsizeParam = CStringCreateFromCStr("slotsize");
    CString *fldcatParam = CStringCreateFromCStr("fldcat");
    CString *fldnameParam = CStringCreateFromCStr("fldname");
    CString *typeParam = CStringCreateFromCStr("type");
    CString *lengthParam = CStringCreateFromCStr("length");
    CString *offsetParam = CStringCreateFromCStr("offset");
    
    TableScan *table = TableScanInit(transaction,tblcatParam,tableManager->tableCatalogLayout);
    Scan*tableCatalog = ScanInit(table,SCAN_TABLE_CODE);
    TableScanInsert(tableCatalog);
    TableScanSetString(tableCatalog,tblnameParam,tblname);
    TableScanSetInt(tableCatalog,slotsizeParam,layout->SlotSize);
    TableScanClose(tableCatalog);

    TableScan *field = TableScanInit(transaction,fldcatParam,tableManager->fieldCatalogLayout);
    Scan*fieldCatalog = ScanInit(field,SCAN_TABLE_CODE);
    FieldNode *fieldNode = sch->fields;
    while(fieldNode){
        CString *fldname = fieldNode->fileName;
        TableScanInsert(fieldCatalog);
        TableScanSetString(fieldCatalog,tblnameParam, tblname);
        TableScanSetString(fieldCatalog,fldnameParam, fldname);
        TableScanSetInt(fieldCatalog,typeParam, SchemaType(sch,fldname));
        TableScanSetInt(fieldCatalog,lengthParam, SchemaLength(sch,fldname));
        TableScanSetInt(fieldCatalog,offsetParam, LayoutOffset(layout,fldname));
        fieldNode = fieldNode->next;
    }
    TableScanClose(fieldCatalog);
    
    CStringDestroy(tblcatParam);
    CStringDestroy(tblnameParam);
    CStringDestroy(slotsizeParam);
    CStringDestroy(fldcatParam);
    CStringDestroy(fldnameParam);
    CStringDestroy(typeParam);
    CStringDestroy(lengthParam);
    CStringDestroy(offsetParam);
}
TableManager *TableManagerInit(bool isNew,Transaction*transaction){
    Schema * tableCatalogSchema = SchemaInit();
    CString *tblnameParam = CStringCreateFromCStr("tblname");
    CString *slotsizeParam = CStringCreateFromCStr("slotsize");
    CString *fldnameParam = CStringCreateFromCStr("fldname");
    CString *typeParam = CStringCreateFromCStr("type");
    CString *lengthParam = CStringCreateFromCStr("length");
    CString *offsetParam = CStringCreateFromCStr("offset");
    CString *tblcatParam = CStringCreateFromCStr("tblcat");
    CString *fldcatParam = CStringCreateFromCStr("fldcat");
    
    SchemaAddStringField(tableCatalogSchema,tblnameParam,TABLE_MANAGER_MAX_NAME);
    SchemaAddIntField(tableCatalogSchema,slotsizeParam);
    TableManager *tableManager = malloc(sizeof (TableManager));
    tableManager->tableCatalogLayout = LayoutInit(tableCatalogSchema,NULL,-1);

    Schema *fieldCatalogSchema = SchemaInit();
    SchemaAddStringField(fieldCatalogSchema,tblnameParam,TABLE_MANAGER_MAX_NAME);
    SchemaAddStringField(fieldCatalogSchema,fldnameParam,TABLE_MANAGER_MAX_NAME);
    SchemaAddIntField(fieldCatalogSchema,typeParam);
    SchemaAddIntField(fieldCatalogSchema,lengthParam);
    SchemaAddIntField(fieldCatalogSchema,offsetParam);
    tableManager->fieldCatalogLayout = LayoutInit(fieldCatalogSchema,NULL,0);

    if(isNew){
        TableManagerCreateTable(tableManager,tblcatParam,tableCatalogSchema,transaction);
        TableManagerCreateTable(tableManager,fldcatParam,fieldCatalogSchema,transaction);
    }
    
    CStringDestroy(tblnameParam);
    CStringDestroy(slotsizeParam);
    CStringDestroy(fldnameParam);
    CStringDestroy(typeParam);
    CStringDestroy(lengthParam);
    CStringDestroy(offsetParam);
    // 注意：tblcatParam 和 fldcatParam 不能在这里释放，因为它们被传递给了 TableManagerCreateTable
    
    return tableManager;
}
Layout *TableManagerGetLayout(TableManager *tableManager,CString *tblname,Transaction *transaction){
    int size = -1;
    CString *tblcatParam = CStringCreateFromCStr("tblcat");
    CString *fldcatParam = CStringCreateFromCStr("fldcat");
    CString *tblnameParam = CStringCreateFromCStr("tblname");
    CString *slotsizeParam = CStringCreateFromCStr("slotsize");
    CString *fldnameParam = CStringCreateFromCStr("fldname");
    CString *typeParam = CStringCreateFromCStr("type");
    CString *lengthParam = CStringCreateFromCStr("length");
    CString *offsetParam = CStringCreateFromCStr("offset");
    
    TableScan *tableScan = TableScanInit(transaction,tblcatParam,tableManager->tableCatalogLayout);
    Scan*scan = ScanInit(tableScan,SCAN_TABLE_CODE);
    while (TableScanNext(scan)){
        char *currentTblnameStr = TableScanGetString(scan,tblnameParam);
        CString *currentTblname = CStringCreateFromCStr(currentTblnameStr);
        if(CStringEqual(currentTblname,tblname)){
            size = TableScanGetInt(scan,slotsizeParam);
            CStringDestroy(currentTblname);
            break;
        }
        CStringDestroy(currentTblname);
    }
    TableScanClose(scan);

    Schema *schema = SchemaInit();
    map_int_t *offsets = malloc(sizeof(map_int_t));
    map_init(offsets);
    TableScan *field = TableScanInit(transaction,fldcatParam,tableManager->fieldCatalogLayout);
    Scan *fieldCatalog = ScanInit(field,SCAN_TABLE_CODE);

    while(TableScanNext(fieldCatalog)){
        char *currentTblnameStr = TableScanGetString(fieldCatalog,tblnameParam);
        CString *currentTblname = CStringCreateFromCStr(currentTblnameStr);
        if(CStringEqual(currentTblname,tblname)){
            char *fldnameStr = TableScanGetString(fieldCatalog,fldnameParam);
            CString *fldname = CStringCreateFromCStr(fldnameStr);
            int fldtype = TableScanGetInt(fieldCatalog,typeParam);
            int fldlen = TableScanGetInt(fieldCatalog,lengthParam);
            int offset = TableScanGetInt(fieldCatalog,offsetParam);
            map_set(offsets,CStringGetPtr(fldname),offset);
            SchemaAddField(schema,fldname,fldtype,fldlen);
            CStringDestroy(fldname);
        }
        CStringDestroy(currentTblname);
    }
    TableScanClose(fieldCatalog);
    
    CStringDestroy(tblcatParam);
    CStringDestroy(fldcatParam);
    CStringDestroy(tblnameParam);
    CStringDestroy(slotsizeParam);
    CStringDestroy(fldnameParam);
    CStringDestroy(typeParam);
    CStringDestroy(lengthParam);
    CStringDestroy(offsetParam);
    
    return LayoutInit(schema,offsets,size);
}