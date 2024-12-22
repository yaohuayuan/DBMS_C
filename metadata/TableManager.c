//
// Created by yaohuayuan on 2024/11/19.
//

#include "TableManager.h"
void TableManagerCreateTable(TableManager *tableManager,char*tblname,Schema*sch,Transaction*transaction){
    Layout *layout = LayoutInit(sch,NULL,-1);

    TableScan *table = TableScanInit(transaction,"tblcat",tableManager->tableCatalogLayout);
    Scan*tableCatalog = ScanInit(table,SCAN_TABLE_CODE);
    TableScanInsert(tableCatalog);
    TableScanSetString(tableCatalog,"tblname",tblname);
    TableScanSetInt(tableCatalog,"slotsize",layout->SlotSize);
    TableScanClose(tableCatalog);

    TableScan *field = TableScanInit(transaction,"fldcat",tableManager->fieldCatalogLayout);
    Scan*fieldCatalog = ScanInit(field,SCAN_TABLE_CODE);
    FieldNode *fieldNode = sch->fields;
    while(fieldNode){
        char *fldname = fieldNode->fileName;
        TableScanInsert(fieldCatalog);
        TableScanSetString(fieldCatalog,"tblname", tblname);
        TableScanSetString(fieldCatalog,"fldname", fldname);
        TableScanSetInt(fieldCatalog,"type", SchemaType(sch,fldname));
        TableScanSetInt(fieldCatalog,"length", SchemaLength(sch,fldname));
        TableScanSetInt(fieldCatalog,"offset", LayoutOffset(layout,fldname));
        fieldNode = fieldNode->next;
    }
    TableScanClose(fieldCatalog);
}
TableManager *TableManagerInit(bool isNew,Transaction*transaction){
    Schema * tableCatalogSchema = SchemaInit();
    SchemaAddStringField(tableCatalogSchema,"tblname",TABLE_MANAGER_MAX_NAME);
    SchemaAddIntField(tableCatalogSchema,"slotsize");
    TableManager *tableManager = malloc(sizeof (TableManager));
    tableManager->tableCatalogLayout = LayoutInit(tableCatalogSchema,NULL,-1);

    Schema *fieldCatalogSchema = SchemaInit();
    SchemaAddStringField(fieldCatalogSchema,"tblname",TABLE_MANAGER_MAX_NAME);
    SchemaAddStringField(fieldCatalogSchema,"fldname",TABLE_MANAGER_MAX_NAME);
    SchemaAddIntField(fieldCatalogSchema,"type");
    SchemaAddIntField(fieldCatalogSchema,"length");
    SchemaAddIntField(fieldCatalogSchema,"offset");
    tableManager->fieldCatalogLayout = LayoutInit(fieldCatalogSchema,NULL,0);

    if(isNew){
        TableManagerCreateTable(tableManager,"tblcat",tableCatalogSchema,transaction);
        TableManagerCreateTable(tableManager,"fldcat",fieldCatalogSchema,transaction);
    }
    return tableManager;
}
Layout *TableManagerGetLayout(TableManager *tableManager,char *tblname,Transaction *transaction){
    int size = -1;
    TableScan *tableScan = TableScanInit(transaction,"tblcat",tableManager->tableCatalogLayout);
    Scan*scan = ScanInit(tableScan,SCAN_TABLE_CODE);
    while (TableScanNext(scan)){
        if(strcmp(TableScanGetString(scan,"tblname"),tblname)==0){
            size = TableScanGetInt(scan,"slotsize");
            break;
        }
    }
    TableScanClose(scan);

    Schema *schema = SchemaInit();
    map_int_t *offsets = malloc(sizeof(map_int_t));
    map_init(offsets);
    TableScan *field = TableScanInit(transaction,"fldcat",tableManager->fieldCatalogLayout);
    Scan *fieldCatalog = ScanInit(field,SCAN_TABLE_CODE);

    while(TableScanNext(fieldCatalog)){
        if(strcmp(TableScanGetString(fieldCatalog,"tblname"),tblname)==0){
            char *fldname = TableScanGetString(fieldCatalog,"fldname");
            int fldtype = TableScanGetInt(fieldCatalog,"type");
            int fldlen = TableScanGetInt(fieldCatalog,"length");
            int offset = TableScanGetInt(fieldCatalog,"offset");
            map_set(offsets,fldname,offset);
            SchemaAddField(schema,fldname,fldtype,fldlen);
        }
    }
    TableScanClose(fieldCatalog);
    return LayoutInit(schema,offsets,size);
}