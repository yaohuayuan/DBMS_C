//
// Created by yaohuayuan on 2024/11/19.
//

#ifndef DBMS_C_PRODUCTSCAN_H
#define DBMS_C_PRODUCTSCAN_H


#include "Scan.h"
typedef struct Scan Scan;
typedef struct ProductScan{
    Scan *s1,*s2;

}ProductScan;

ProductScan *ProductScanInit(Scan*s1,Scan*s2);
void ProductScanBeforeFirst(void*Scan);
bool ProductScanNext(void*Scan);
int ProductScanGetInt(void *scan,CString *fldname);
char * ProductScanGetString(void *scan,CString *fldname);
Constant * ProductScanGetVal(void *scan,CString *fldname);
bool ProductScanHasField(void *scan,CString *fldname);
void ProductScanClose(void *scan);


#endif //DBMS_C_PRODUCTSCAN_H
