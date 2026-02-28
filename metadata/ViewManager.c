//
// Created by yaohuayuan on 2024/11/19.
//

#include "ViewManager.h"

ViewManager* ViewManagerInit(bool isNew, TableManager *tableManager, Transaction *transaction) {
    ViewManager *viewManager = malloc(sizeof(ViewManager));
    viewManager->tableManager = tableManager;

    if (isNew) {
        Schema *schema = SchemaInit();
        CString *viewnameParam = CStringCreateFromCStr("viewname");
        CString *viewdefParam = CStringCreateFromCStr("viewdef");
        SchemaAddStringField(schema, viewnameParam, TABLE_MANAGER_MAX_NAME);
        SchemaAddStringField(schema, viewdefParam, VIEW_MANAGER_MAX_VIEWDEF);
        CString *viewcatTbl = CStringCreateFromCStr("viewcat");
        TableManagerCreateTable(tableManager, viewcatTbl, schema, transaction);
        CStringDestroy(viewnameParam);
        CStringDestroy(viewdefParam);
        CStringDestroy(viewcatTbl);
    }
    return viewManager;
}

// 创建视图
void ViewManagerCreateView(ViewManager *viewManager, CString *vname, CString *vdef, Transaction *transaction) {
    CString *viewcatTbl = CStringCreateFromCStr("viewcat");
    CString *viewnameParam = CStringCreateFromCStr("viewname");
    CString *viewdefParam = CStringCreateFromCStr("viewdef");
    
    Layout *layout = TableManagerGetLayout(viewManager->tableManager, viewcatTbl, transaction);
    TableScan *scan = TableScanInit(transaction, viewcatTbl, layout);
    Scan *tableScan = ScanInit(scan,SCAN_TABLE_CODE);
    TableScanInsert(tableScan);
    TableScanSetString(tableScan, viewnameParam, vname);
    TableScanSetString(tableScan, viewdefParam, vdef);

    TableScanClose(tableScan);
    
    CStringDestroy(viewcatTbl);
    CStringDestroy(viewnameParam);
    CStringDestroy(viewdefParam);
}

// 获取视图定义
CString* ViewManagerGetViewDef(ViewManager *viewManager, CString *vname, Transaction *transaction) {
    CString *result = NULL;
    CString *viewcatTbl = CStringCreateFromCStr("viewcat");
    CString *viewnameParam = CStringCreateFromCStr("viewname");
    CString *viewdefParam = CStringCreateFromCStr("viewdef");

    Layout *layout = TableManagerGetLayout(viewManager->tableManager, viewcatTbl, transaction);
    TableScan *tableScan = TableScanInit(transaction, viewcatTbl, layout);
    Scan *scan = ScanInit(tableScan,SCAN_TABLE_CODE);
    while (TableScanNext(scan)) {
        CString *currentName = CStringCreateFromCStr(TableScanGetString(scan, viewnameParam));
        if (CStringEqual(currentName, vname)) {
            result = CStringCreateFromCStr(TableScanGetString(scan, viewdefParam)); // 拷贝字符串以返回
            CStringDestroy(currentName);
            break;
        }
        CStringDestroy(currentName);
    }
    
    CStringDestroy(viewcatTbl);
    CStringDestroy(viewnameParam);
    CStringDestroy(viewdefParam);

    TableScanClose(scan);
    return result;
}
