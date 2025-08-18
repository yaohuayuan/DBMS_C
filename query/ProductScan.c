//
// Created by yaohuayuan on 2024/11/19.
//

#include "ProductScan.h"

#include <stdlib.h>
void ProductScanBeforeFirst(void*data){
    Scan *scan = (Scan*)data;
    ProductScan *productScan = scan->scanUnion.productScan;
    productScan->s1->beforeFirst(productScan->s1);
    productScan->s1->next(productScan->s1);
    productScan->s2->beforeFirst(productScan->s2);
}
bool ProductScanNext(void*data){
    Scan *scan = (Scan*)data;
    ProductScan *productScan = scan->scanUnion.productScan;
    if(productScan->s2->next(productScan->s2)){
        return true;
    }else{
        productScan->s2->beforeFirst(productScan->s2);
        return productScan->s2->next(productScan->s2)&&
            productScan->s1->next(productScan->s1);
    }
}
int ProductScanGetInt(void *data,char *fldname){
    Scan *scan = (Scan*)data;
    ProductScan *productScan = scan->scanUnion.productScan;
    if(productScan->s1->hasField(productScan->s1,fldname)){
        return productScan->s1->getInt(productScan->s1,fldname);
    }else{
        return productScan->s2->getInt(productScan->s2,fldname);
    }
}
char * ProductScanGetString(void *data,char *fldname){
    Scan *scan = (Scan*)data;
    ProductScan *productScan = scan->scanUnion.productScan;
    if(productScan->s1->hasField(productScan->s1,fldname)){
        return productScan->s1->getString(productScan->s1,fldname);
    }else{
        return productScan->s2->getString(productScan->s2,fldname);
    }
}
Constant * ProductScanGetVal(void *data,char *fldname){
    Scan *scan = (Scan*)data;
    ProductScan *productScan = scan->scanUnion.productScan;
    if(productScan->s1->hasField(productScan->s1,fldname)){
        return productScan->s1->getVal(productScan->s1,fldname);
    }else{
        return productScan->s2->getVal(productScan->s2,fldname);
    }
}
bool ProductScanHasField(void *data,char *fldname){
    Scan *scan = (Scan*)data;
    ProductScan *productScan = scan->scanUnion.productScan;
    return productScan->s1->hasField(productScan->s1,fldname) || productScan->s2->hasField(productScan->s2,fldname);
}
void ProductScanClose(void *data){
    Scan *scan = (Scan*)data;
    ProductScan *productScan = scan->scanUnion.productScan;
    productScan->s1->close(productScan->s1);
    productScan->s2->close(productScan->s2);
}
ProductScan *ProductScanInit(Scan*s1,Scan*s2){
    ProductScan *productScan = malloc(sizeof (ProductScan));
    productScan->s1 = s1;
    productScan->s2 = s2;
    Scan *scan = ScanInit(productScan,SCAN_PRODUCT_CODE);
    ProductScanBeforeFirst(scan);
//    productScan->ops->beforeFirst = ProductScanBeforeFirst;
//    productScan->ops->hasField = ProductScanHasField;
//    productScan->ops->next = ProductScanNext;
//    productScan->ops->getVal = ProductScanGetVal;
//    productScan->ops->getInt = ProductScanGetInt;
//    productScan->ops->getVal = ProductScanGetVal;
//    productScan->ops->getString = ProductScanGetString;
//    productScan->ops->close = ProductScanClose;
    return productScan;
}
