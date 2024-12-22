//
// Created by yaohuayuan on 2024/11/19.
//

#include "ViewManager.h"

ViewManager* ViewManagerInit(bool isNew, TableManager *tableManager, Transaction *transaction) {
    ViewManager *viewManager = malloc(sizeof(ViewManager));
    viewManager->tableManager = tableManager;

    if (isNew) {
        Schema *schema = SchemaInit();
        SchemaAddStringField(schema, "viewname", TABLE_MANAGER_MAX_NAME);
        SchemaAddStringField(schema, "viewdef", VIEW_MANAGER_MAX_VIEWDEF);
        TableManagerCreateTable(tableManager, "viewcat", schema, transaction);
    }
    return viewManager;
}

// 创建视图
void ViewManagerCreateView(ViewManager *viewManager, char *vname, char *vdef, Transaction *transaction) {
    Layout *layout = TableManagerGetLayout(viewManager->tableManager, "viewcat", transaction);
    TableScan *scan = TableScanInit(transaction, "viewcat", layout);
    Scan *tableScan = ScanInit(scan,SCAN_TABLE_CODE);
    TableScanInsert(tableScan);
    TableScanSetString(tableScan, "viewname", vname);
    TableScanSetString(tableScan, "viewdef", vdef);

    TableScanClose(tableScan);
}

// 获取视图定义
char* ViewManagerGetViewDef(ViewManager *viewManager, char *vname, Transaction *transaction) {
    char *result = NULL;

    Layout *layout = TableManagerGetLayout(viewManager->tableManager, "viewcat", transaction);
    TableScan *tableScan = TableScanInit(transaction, "viewcat", layout);
    Scan *scan = ScanInit(tableScan,SCAN_TABLE_CODE);
    while (TableScanNext(scan)) {
        if (strcmp(TableScanGetString(scan, "viewname"), vname) == 0) {
            result = strdup(TableScanGetString(scan, "viewdef")); // 拷贝字符串以返回
            break;
        }
    }

    TableScanClose(scan);
    return result;
}
