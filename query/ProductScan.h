//
// Created by yaohuayuan on 2024/11/19.
//

#ifndef DBMS_C_PRODUCTSCAN_H
#define DBMS_C_PRODUCTSCAN_H
#ifndef PRODUCT_SCAN_H
#define PRODUCT_SCAN_H

#include "Scan.h"

typedef struct ProductScan ProductScan;

ProductScan* ProductScanInit(Scan *s1, Scan *s2);

#endif // PRODUCT_SCAN_H
#endif //DBMS_C_PRODUCTSCAN_H
