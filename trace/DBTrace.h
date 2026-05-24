#ifndef DBMS_C_DBTRACE_H
#define DBMS_C_DBTRACE_H

#include <stdbool.h>

typedef struct QueryData QueryData;
typedef struct Plan Plan;
typedef struct Scan Scan;
typedef struct CommandData CommandData;
typedef struct CreateTableData CreateTableData;
typedef struct CreateViewData CreateViewData;
typedef struct CreateIndexData CreateIndexData;
typedef struct InsertData InsertData;
typedef struct ModifyData ModifyData;
typedef struct DeleteData DeleteData;
typedef struct Transaction Transaction;

void DBTraceEnable(void);
void DBTraceDisable(void);
bool DBTraceIsEnabled(void);
void DBTraceLog(const char *stage, const char *message);
void DBTraceLogf(const char *stage, const char *fmt, ...);
void DBTracePush(void);
void DBTracePop(void);

void QueryDataTrace(QueryData *data);
void PlanTraceTree(Plan *plan);
void ScanTraceChain(Scan *scan);
void CommandDataTrace(CommandData *commandData);
void UpdateExecutionTraceCreateTable(CreateTableData *data);
void UpdateExecutionTraceCreateView(CreateViewData *data);
void UpdateExecutionTraceCreateIndex(CreateIndexData *data);
void UpdateExecutionTraceInsert(InsertData *data);
void UpdateExecutionTraceModify(ModifyData *data);
void UpdateExecutionTraceDelete(DeleteData *data);
void TransactionTraceCommit(Transaction *transaction);
void TransactionTraceRollback(Transaction *transaction);

#endif // DBMS_C_DBTRACE_H
