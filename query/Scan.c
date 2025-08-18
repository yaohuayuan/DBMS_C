//
// Created by yaohuayuan on 2024/11/19.
//

#include "Scan.h"
#include "Scan.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ProductScan.h"


Scan *ScanInit(void *data,ScanCode code){
    Scan* scan = malloc(sizeof(Scan));
    scan->code = code;
    if(code == SCAN_TABLE_CODE){
        TableScan *tableScan = (TableScan*)data;
        scan->scanUnion.tableScan = tableScan;
        scan->next = TableScanNext;
        scan->hasField = TableScanHasField;
        scan->getVal = TableScanGetVal;
        scan->beforeFirst = TableScanBeforeFirst;
        scan->next = TableScanNext;
        scan->close = TableScanClose;
        scan->getInt = TableScanGetInt;
        scan->getString = TableScanGetString;
        scan->getInt = TableScanGetInt;
        scan->getVal = TableScanGetVal;
        scan->hasField = TableScanHasField;
        scan->delete = TableScanDelete;
        scan->insert = TableScanInsert;
        scan->getRid = TableScanGetRID;
        scan->moveToRID = TableScanMoveToRid;
        scan->setInt = TableScanSetInt;
        scan->setString = TableScanSetString;
        scan->setVal = TableScanSetVal;
    }else if(code == SCAN_PRODUCT_CODE){
        ProductScan *productScan = (ProductScan*)data;
        scan->code = code;
        scan->scanUnion.productScan = productScan;
        scan->beforeFirst = ProductScanBeforeFirst;
        scan->hasField = ProductScanHasField;
        scan->next = ProductScanNext;
        scan->getVal = ProductScanGetVal;
        scan->getInt = ProductScanGetInt;
        scan->getVal = ProductScanGetVal;
        scan->getString = ProductScanGetString;
        scan->close = ProductScanClose;
    }else if(code == SCAN_SELECT_CODE){
        SelectScan *selectScan = (SelectScan*)data;
        scan->scanUnion.selectScan = selectScan;
        scan->next = SelectScanNext;
        scan->hasField = SelectScanHasField;
        scan->getVal = SelectScanGetVal;
        scan->beforeFirst = SelectScanBeforeFirst;
        scan->next = SelectScanNext;
        scan->close = SelectScanClose;
        scan->getInt = SelectScanGetInt;
        scan->getString = SelectScanGetString;
        scan->getInt = SelectScanGetInt;
        scan->getVal = SelectScanGetVal;
        scan->hasField = SelectScanHasField;
        scan->delete = SelectScanDelete;
        scan->insert = SelectScanInsert;
        scan->getRid = SelectScanGetRID;
        scan->moveToRID = SelectScanMoveToRID;
        scan->setInt = SelectScanSetInt;
        scan->setString = SelectScanSetString;
        scan->setVal = SelectScanSetVal;
    }else if(code == SCAN_PROJECT_CODE){
        scan->code = SCAN_PROJECT_CODE;
        ProjectScan *projectScan = (ProjectScan*)data;
        scan->scanUnion.projectScan = projectScan;
        scan->getVal = ProjectScanGetVal;
        scan->hasField = ProjectScanHasField;
        scan->getString = ProjectScanGetString;
        scan->getInt = ProjectScanGetInt;
        scan->next = ProjectScanNext;
        scan->beforeFirst = ProjectScanBeforeFirst;
        scan->close = ProjectClose;
    }
    return scan;
}