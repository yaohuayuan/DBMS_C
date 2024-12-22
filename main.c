#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SimpleDB.h"
#define MAX(a, b) ((a) > (b) ? (a) : (b))
void SelectDataDisplay(Plan *plan,Scan*scan){
    ProjectPlan*projectPlan = plan->planUnion.projectPlan;
    int length = 5;
    FieldNode*fieldNode = projectPlan->schema->fields;
    while(fieldNode){
        length = MAX(length,fieldNode->length);
        fieldNode = fieldNode->next;
    }
    ProjectScan*projectScan = scan->scanUnion.projectScan;
    List *field = projectScan->fieldList;
    ListNode *fieldHead = field->head;
    int count = 0;
    while(fieldHead){
        count++;
        printf("|%*s|",length,fieldHead->data->stringData);
        fieldHead = fieldHead->next;

    } printf("\n");
    int n = (length+2)*count;
    for(int i=0;i<n;i++){
        printf("-");
    }
    printf("\n");
    while (scan->next(scan)) {
        fieldNode = projectPlan->schema->fields;
        while (fieldNode){
            if(fieldNode->type == FILE_INFO_CODE_INTEGER){
                int result = scan->getInt(scan, fieldNode->fileName);
                printf("|%*d|",length,result);
            }else if(fieldNode->type == FILE_INFO_CODE_VARCHAR){
                char *result = scan->getString(scan, fieldNode->fileName);
                printf("|%*s|",length,result);
            }

            fieldNode = fieldNode->next;
        }
        printf("\n");
    }

}

int main() {
    // 初始化数据库
    TransactionManager *transactionManager = TransactionManagerInit();
    SimpleDB *DBMS = SimpleDBInit("Show", SIMPLE_DB_INIT_VAL, SIMPLE_DB_INIT_VAL);
    Transaction *transaction = SimpleDataNewTX(DBMS);
    TransactionManagerAdd(transactionManager,transaction);
    Planner *planner = DBMS->planer;

    printf("Database has started successfully. Type 'exit' to close the database.\n");

    char cmd[512];  // 用于存储用户输入的命令

    while (1) {
        printf("SQL>");
        if (scanf(" %[^\n]", cmd) == EOF) {  // 读取一整行，支持带空格的 SQL 语句
            break;
        }

        if (strcmp(cmd, "exit") == 0) {
            printf("Exiting the database...\n");
            break;
        }

        // 处理命令
        if (strncmp(cmd, "SELECT", 6) == 0 || strncmp(cmd, "select", 6) == 0) {
            // SELECT 查询语句
            Plan *plan = PlannerCreateQueryPlan(planner, cmd, transaction);
            Scan *scan = plan->open(plan);

            // 遍历查询结果
            printf("Query results:\n");
            SelectDataDisplay(plan,scan);

            scan->close(scan);
        }else if (strncmp(cmd, "COMMIT", 6) == 0 || strncmp(cmd, "COMMIT", 6) == 0){
            TransactionCommit(transaction);
            transaction = SimpleDataNewTX(DBMS);
            TransactionManagerAdd(transactionManager,transaction);
        }else if(strncmp(cmd, "ROLLBACK", 8) == 0 || strncmp(cmd, "rollback", 8) == 0){
            TransactionRollback(transaction);
            transaction = SimpleDataNewTX(DBMS);
            TransactionManagerAdd(transactionManager,transaction);
        }else if(strncmp(cmd, "START_TRANSACTION", 17) == 0){
            transaction = SimpleDataNewTX(DBMS);
            TransactionManagerAdd(transactionManager,transaction);
        }else if (strncmp(cmd, "SWITCH", 6) == 0 || strncmp(cmd, "switch", 6) == 0) {
            char txnId[128];
            sscanf(cmd + 7, "%s", txnId);  // 获取事务 ID
            TransactionManagerSwitch(transactionManager, txnId);
        }
            // 处理显示所有事务命令
        else if (strncmp(cmd, "DISPLAY", 7) == 0 || strncmp(cmd, "display", 7) == 0) {
            TransactionManagerDisplay(transactionManager);
        }
        else {
            int result = PlannerExecuteUpdate(planner, cmd, transaction);
            printf("Command executed. Rows affected: %d\n", result);
        }
    }

    // 提交事务并释放资源
    TransactionCommit(transaction);
    printf("Database closed.\n");

    return 0;
}
