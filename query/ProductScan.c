//
// Created by yaohuayuan on 2024/11/19.
//

#include "ProductScan.h"
#include "ProductScan.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct ProductScan {
    Scan scan;
    Scan *s1;
    Scan *s2;
} ProductScan;

static void ProductScanBeforeFirst(ProductScan *ps) {
    ps->s1->ops->beforeFirst(ps->s1);
    ps->s1->ops->next(ps->s1);
    ps->s2->ops->beforeFirst(ps->s2);
}

static bool ProductScanNext(ProductScan *ps) {
    if (ps->s2->ops->next(ps->s2)) {
        return true;
    } else {
        ps->s2->ops->beforeFirst(ps->s2);
        return ps->s2->ops->next(ps->s2) && ps->s1->ops->next(ps->s1);
    }
}

static int ProductScanGetInt(ProductScan *ps, const char *fldname) {
    if (ps->s1->ops->hasField(ps->s1, fldname)) {
        return ps->s1->ops->getInt(ps->s1, fldname);
    } else {
        return ps->s2->ops->getInt(ps->s2, fldname);
    }
}

static const char* ProductScanGetString(ProductScan *ps, const char *fldname) {
    if (ps->s1->ops->hasField(ps->s1, fldname)) {
        return ps->s1->ops->getString(ps->s1, fldname);
    } else {
        return ps->s2->ops->getString(ps->s2, fldname);
    }
}

static Constant* ProductScanGetVal(ProductScan *ps, const char *fldname) {
    if (ps->s1->ops->hasField(ps->s1, fldname)) {
        return ps->s1->ops->getVal(ps->s1, fldname);
    } else {
        return ps->s2->ops->getVal(ps->s2, fldname);
    }
}

static bool ProductScanHasField(ProductScan *ps, const char *fldname) {
    return ps->s1->ops->hasField(ps->s1, fldname) || ps->s2->ops->hasField(ps->s2, fldname);
}

static void ProductScanClose(ProductScan *ps) {
    ps->s1->ops->close(ps->s1);
    ps->s2->ops->close(ps->s2);
}

static ScanOps productScanOps = {
        .beforeFirst = (void (*)(Scan *))ProductScanBeforeFirst,
        .next = (bool (*)(Scan *))ProductScanNext,
        .getInt = (int (*)(Scan *, const char *))ProductScanGetInt,
        .getString = (const char* (*)(Scan *, const char *))ProductScanGetString,
        .getVal = (Constant* (*)(Scan *, const char *))ProductScanGetVal,
        .hasField = (bool (*)(Scan *, const char *))ProductScanHasField,
        .close = (void (*)(Scan *))ProductScanClose
};

ProductScan* ProductScanInit(Scan *s1, Scan *s2) {
    ProductScan *ps = (ProductScan *)malloc(sizeof(ProductScan));
    if (ps == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    ps->s1 = s1;
    ps->s2 = s2;
    ps->scan.ops = &productScanOps;
    ps->scan.ops->beforeFirst(&ps->scan);
    return ps;
}