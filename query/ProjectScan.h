//
// Created by yaohuayuan on 2024/11/19.
//

#ifndef DBMS_C_PROJECTSCAN_H
#define DBMS_C_PROJECTSCAN_H
#ifndef PROJECT_SCAN_H
#define PROJECT_SCAN_H

#include "Scan.h"

typedef struct ProjectScan ProjectScan;

ProjectScan* ProjectScanInit(Scan *s, const char **fieldlist, int numFields);

#endif // PROJECT_SCAN_H
#endif //DBMS_C_PROJECTSCAN_H
