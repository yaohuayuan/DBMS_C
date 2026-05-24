#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "DBMS.h"
#include "ProjectScan.h"
#include "Plan.h"
#include "Planner.h"
#include "TransactionManager.h"
#include "trace/DBTrace.h"
#include "Lib/CString.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))

static char *trim_inplace(char *s) {
    if (s == NULL) return s;

    // 去掉前导空白
    while (*s && isspace((unsigned char)*s)) {
        s++;
    }

    // 如果全是空白
    if (*s == '\0') {
        return s;
    }

    // 去掉末尾空白
    char *end = s + strlen(s) - 1;
    while (end > s && isspace((unsigned char)*end)) {
        *end = '\0';
        end--;
    }

    return s;
}

static int starts_with_ignore_case(const char *s, const char *prefix) {
    while (*prefix && *s) {
        if (tolower((unsigned char)*s) != tolower((unsigned char)*prefix)) {
            return 0;
        }
        s++;
        prefix++;
    }
    return *prefix == '\0';
}
static int is_meta_command(const char *s) {
    return starts_with_ignore_case(s, "exit") ||
           starts_with_ignore_case(s, "commit") ||
           starts_with_ignore_case(s, "rollback") ||
           starts_with_ignore_case(s, "display") ||
           starts_with_ignore_case(s, "start_new_transaction") ||
           starts_with_ignore_case(s, "switch") ||
           starts_with_ignore_case(s, "trace") ||
           starts_with_ignore_case(s, "bench");
}
void SelectDataDisplay(Plan *plan, Scan *scan) {
    DBTraceLog("OUTPUT", "Rendering SELECT result rows");

    ProjectPlan *projectPlan = plan->planUnion.projectPlan;
    int length = 5;
    FieldNode *fieldNode = projectPlan->schema->fields;

    while (fieldNode) {
        length = MAX(length, fieldNode->length);
        fieldNode = fieldNode->next;
    }

    ProjectScan *projectScan = scan->scanUnion.projectScan;
    CList *field = projectScan->fieldList;
    CListNode *fieldHead = field->head;
    int count = 0;

    while (fieldHead) {
        count++;
        CString *fieldCStr = ((CString *)fieldHead->data);
        const char *fieldName = CStringGetPtr(fieldCStr);
        printf("|%*s|", length, fieldName);
        fieldHead = fieldHead->next;
    }
    printf("\n");

    int n = (length + 2) * count;
    for (int i = 0; i < n; i++) {
        printf("-");
    }
    printf("\n");

    int rows = 0;
    while (scan->next(scan)) {
        rows++;
        fieldNode = projectPlan->schema->fields;
        while (fieldNode) {
            if (fieldNode->type == FILE_INFO_CODE_INTEGER) {
                int result = scan->getInt(scan, fieldNode->fileName);
                printf("|%*d|", length, result);
            } else if (fieldNode->type == FILE_INFO_CODE_VARCHAR) {
                const char *result = scan->getString(scan, fieldNode->fileName);
                printf("|%*s|", length, result);
            }
            fieldNode = fieldNode->next;
        }
        printf("\n");
    }
    DBTraceLogf("OUTPUT", "result rows: %d", rows);
}
int main(int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--trace") == 0) {
            DBTraceEnable();
        }
    }

    TransactionManager *transactionManager = TransactionManagerInit();

    // 初始化数据库
    SimpleDB *DBMS = SimpleDBInit("Show2", SIMPLE_DB_INIT_VAL, SIMPLE_DB_INIT_VAL);
    Transaction *transaction = SimpleDataNewTX(DBMS);
    Planner *planner = DBMS->planer;

    // 把初始事务加入事务管理器，避DISPLAY 时看不到当前事务
    TransactionManagerAdd(transactionManager, transaction);

    printf("Database has started successfully. Type 'exit' to close the database.\n");

    char sql[4096] = {0};
    char line[512];

    while (1) {
        printf("SQL> ");
        sql[0] = '\0';

        while (1) {
            if (!fgets(line, sizeof(line), stdin)) {
                goto shutdown;
            }

            char *trimmedLine = trim_inplace(line);

            // 空行直接继续
            if (trimmedLine[0] == '\0') {
                if (sql[0] != '\0') {
                    printf(" ...> ");
                } else {
                    printf("SQL> ");
                }
                continue;
            }

            // 跳过 // 注释行，方便直接粘贴测试脚本
            if (starts_with_ignore_case(trimmedLine, "//")) {
                if (sql[0] != '\0') {
                    printf(" ...> ");
                } else {
                    printf("SQL> ");
                }
                continue;
            }

            // 如果当前还没开始拼 SQL，且这一行是元命令，则允许不写分号直接执
            if (sql[0] == '\0' && is_meta_command(trimmedLine) && strchr(trimmedLine, ';') == NULL) {
                strncpy(sql, trimmedLine, sizeof(sql) - 1);
                sql[sizeof(sql) - 1] = '\0';
                break;
            }

            // 拼接输入，行与行之间补一个空格，避免单词粘连
            size_t currentLen = strlen(sql);
            size_t addLen = strlen(trimmedLine);
            size_t remain = sizeof(sql) - currentLen - 1;

            if (currentLen > 0) {
                if (remain < 2) {
                    printf("Input too long.\n");
                    sql[0] = '\0';
                    break;
                }
                strncat(sql, " ", remain);
                currentLen = strlen(sql);
                remain = sizeof(sql) - currentLen - 1;
            }

            if (addLen > remain) {
                printf("Input too long.\n");
                sql[0] = '\0';
                break;
            }

            strncat(sql, trimmedLine, remain);

            // SQL 以分号结
            if (strchr(trimmedLine, ';')) {
                break;
            }

            printf(" ...> ");
        }

        char *cmd = trim_inplace(sql);

        if (cmd[0] == '\0') {
            continue;
        }

        // 去掉第一个分
        char *semicolon = strchr(cmd, ';');
        if (semicolon) {
            *semicolon = '\0';
        }
        cmd = trim_inplace(cmd);

        if (strcmp(cmd, "exit") == 0) {
            printf("Exiting the database...\n");
            break;
        }

        if (starts_with_ignore_case(cmd, "select")) {
            CString *csql = CStringCreateFromCStr(cmd);
            Plan *plan = PlannerCreateQueryPlan(planner, csql, transaction);
            CStringDestroy(csql);

            Scan *scan = plan->open(plan);
            ScanTraceChain(scan);
            printf("Query results:\n");
            SelectDataDisplay(plan, scan);
            scan->close(scan);

        } else if (starts_with_ignore_case(cmd, "trace on")) {
            DBTraceEnable();
            printf("Trace mode enabled.\n");

        } else if (starts_with_ignore_case(cmd, "trace off")) {
            DBTraceDisable();
            printf("Trace mode disabled.\n");

        } else if (starts_with_ignore_case(cmd, "trace")) {
            printf("Trace mode is %s.\n", DBTraceIsEnabled() ? "on" : "off");

        } else if (starts_with_ignore_case(cmd, "commit")) {
            TransactionCommit(transaction);
            transaction = SimpleDataNewTX(DBMS);
            TransactionManagerAdd(transactionManager, transaction);

        } else if (starts_with_ignore_case(cmd, "rollback")) {
            TransactionRollback(transaction);
            transaction = SimpleDataNewTX(DBMS);
            TransactionManagerAdd(transactionManager, transaction);

        } else if (starts_with_ignore_case(cmd, "start_new_transaction")) {
            transaction = SimpleDataNewTX(DBMS);
            TransactionManagerAdd(transactionManager, transaction);

        } else if (starts_with_ignore_case(cmd, "switch")) {
            char txnId[128] = {0};

            if (sscanf(cmd + 6, "%127s", txnId) != 1) {
                printf("Usage: SWITCH <transaction_id>\n");
                continue;
            }

            TransactionManagerSwitch(transactionManager, txnId);

        } else if (starts_with_ignore_case(cmd, "display")) {
            TransactionManagerDisplay(transactionManager);

        } else if (starts_with_ignore_case(cmd, "bench")) {
            char operation[32] = {0};
            int times = 0;

            if (sscanf(cmd + 5, "%31s %d", operation, &times) != 2 || times <= 0) {
                printf("Usage: BENCH <operation> <times>\n");
                continue;
            }

            Transaction *tx = SimpleDataNewTX(DBMS);
            clock_t start = clock();

            for (int i = 0; i < times; ++i) {
                char benchSql[256];

                if (strcmp(operation, "insert") == 0) {
                    sprintf(benchSql, "INSERT INTO test(a, b) VALUES(%d, 'hello%d')", i, i);
                } else {
                    printf("Unsupported operation: %s\n", operation);
                    break;
                }

                CString *cSql = CStringCreateFromCStr(benchSql);
                PlannerExecuteUpdate(planner, cSql, tx);
                CStringDestroy(cSql);
            }

            TransactionCommit(tx);
            clock_t end = clock();
            double duration = (double)(end - start) / CLOCKS_PER_SEC;
            printf("[BENCHMARK] Executed %d txns in %.2f sec, TPS = %.2f\n",
                   times, duration, times / duration);

        } else {
            CString *csql = CStringCreateFromCStr(cmd);
            int result = PlannerExecuteUpdate(planner, csql, transaction);
            CStringDestroy(csql);
            DBTraceLogf("OUTPUT", "command completed, rows affected: %d", result);
            printf("Command executed. Rows affected: %d\n", result);
        }
    }

shutdown:
    TransactionCommit(transaction);
    printf("Database closed.\n");
    return 0;
}
