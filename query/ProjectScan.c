//
// Created by yaohuayuan on 2024/11/19.
//

#include "ProjectScan.h"
#include "ProjectScan.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct ProjectScan {
    Scan scan;
    Scan *s;
    char **fieldlist;
    int numFields;
} ProjectScan;

static bool ProjectScanHasField(ProjectScan *ps, const char *fldname) {
    for (int i = 0; i < ps->numFields; ++i) {
        if (strcmp(ps->fieldlist[i], fldname) == 0) {
            return true;
        }
    }
    return false;
}

static void ProjectScanBeforeFirst(ProjectScan *ps) {
    ps->s->ops->beforeFirst(ps->s);
}

static bool ProjectScanNext(ProjectScan *ps) {
    return ps->s->ops->next(ps->s);
}

static int ProjectScanGetInt(ProjectScan *ps, const char *fldname) {
    if (ProjectScanHasField(ps, fldname)) {
        return ps->s->ops->getInt(ps->s, fldname);
    } else {
        fprintf(stderr, "Field %s not found.\n", fldname);
        exit(EXIT_FAILURE);
    }
}

static const char* ProjectScanGetString(ProjectScan *ps, const char *fldname) {
    if (ProjectScanHasField(ps, fldname)) {
        return ps->s->ops->getString(ps->s, fldname);
    } else {
        fprintf(stderr, "Field %s not found.\n", fldname);
        exit(EXIT_FAILURE);
    }
}

static Constant* ProjectScanGetVal(ProjectScan *ps, const char *fldname) {
    if (ProjectScanHasField(ps, fldname)) {
        return ps->s->ops->getVal(ps->s, fldname);
    } else {
        fprintf(stderr, "Field %s not found.\n", fldname);
        exit(EXIT_FAILURE);
    }
}

static bool ProjectScanHasFieldWrapper(Scan *scan, const char *fldname) {
    ProjectScan *ps = (ProjectScan *)scan;
    return ProjectScanHasField(ps, fldname);
}

static void ProjectScanClose(ProjectScan *ps) {
    ps->s->ops->close(ps->s);
}

static ScanOps projectScanOps = {
        .beforeFirst = (void (*)(Scan *))ProjectScanBeforeFirst,
        .next = (bool (*)(Scan *))ProjectScanNext,
        .getInt = (int (*)(Scan *, const char *))ProjectScanGetInt,
        .getString = (const char* (*)(Scan *, const char *))ProjectScanGetString,
        .getVal = (Constant* (*)(Scan *, const char *))ProjectScanGetVal,
        .hasField = (bool (*)(Scan *, const char *))ProjectScanHasFieldWrapper,
        .close = (void (*)(Scan *))ProjectScanClose
};

ProjectScan* ProjectScanInit(Scan *s, const char **fieldlist, int numFields) {
    ProjectScan *ps = (ProjectScan *)malloc(sizeof(ProjectScan));
    if (ps == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    ps->s = s;
    ps->fieldlist = (char **)malloc(numFields * sizeof(char *));
    if (ps->fieldlist == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < numFields; ++i) {
        ps->fieldlist[i] = strdup(fieldlist[i]);
        if (ps->fieldlist[i] == NULL) {
            fprintf(stderr, "Memory allocation failed.\n");
            exit(EXIT_FAILURE);
        }
    }
    ps->numFields = numFields;
    ps->scan.ops = &projectScanOps;
    return ps;
}

// 释放 ProjectScan 资源
void ProjectScanFree(ProjectScan *ps) {
    for (int i = 0; i < ps->numFields; ++i) {
        free(ps->fieldlist[i]);
    }
    free(ps->fieldlist);
    free(ps);
}