#include "DBTrace.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CList.h"
#include "CString.h"
#include "Expression.h"
#include "Parser.h"
#include "Predicate.h"
#include "ProductPlan.h"
#include "ProductScan.h"
#include "ProjectPlan.h"
#include "ProjectScan.h"
#include "QueryData.h"
#include "Scan.h"
#include "Schema.h"
#include "SelectPlan.h"
#include "SelectScan.h"
#include "TablePlan.h"
#include "TableScan.h"
#include "Transaction.h"

static bool g_trace_enabled = false;
static int g_trace_indent = 0;

void DBTraceEnable(void) {
    g_trace_enabled = true;
}

void DBTraceDisable(void) {
    g_trace_enabled = false;
}

bool DBTraceIsEnabled(void) {
    return g_trace_enabled;
}

void DBTracePush(void) {
    if (g_trace_indent < 64) {
        g_trace_indent++;
    }
}

void DBTracePop(void) {
    if (g_trace_indent > 0) {
        g_trace_indent--;
    }
}

void DBTraceLog(const char *stage, const char *message) {
    if (!g_trace_enabled) {
        return;
    }

    printf("[TRACE_%s] ", stage ? stage : "GENERAL");
    for (int i = 0; i < g_trace_indent; i++) {
        printf("  ");
    }
    printf("%s\n", message ? message : "");
}

void DBTraceLogf(const char *stage, const char *fmt, ...) {
    if (!g_trace_enabled) {
        return;
    }

    char message[512];
    va_list args;
    va_start(args, fmt);
    vsnprintf(message, sizeof(message), fmt ? fmt : "", args);
    va_end(args);

    DBTraceLog(stage, message);
}

static const char *safe_cstring(CString *value) {
    if (!value) {
        return "(null)";
    }
    const char *text = CStringGetPtr(value);
    return text ? text : "(null)";
}

static const char *schema_type_name(int type) {
    switch (type) {
        case FILE_INFO_CODE_INTEGER:
            return "INT";
        case FILE_INFO_CODE_VARCHAR:
            return "VARCHAR";
        case FILE_INFO_CODE_CHAR:
            return "CHAR";
        default:
            return "TYPE";
    }
}

static void append_text(char *buffer, size_t size, size_t *used, const char *text) {
    if (!buffer || !used || *used >= size) {
        return;
    }

    int written = snprintf(buffer + *used, size - *used, "%s", text ? text : "");
    if (written < 0) {
        return;
    }
    if ((size_t)written >= size - *used) {
        *used = size - 1;
        return;
    }
    *used += (size_t)written;
}

static void trace_cstring_list(const char *stage, const char *label, CList *list) {
    char buffer[512];
    size_t used = 0;

    buffer[0] = '\0';
    if (!list || !list->head) {
        DBTraceLogf(stage, "%s: (empty)", label);
        return;
    }

    for (CListNode *node = list->head; node; node = node->next) {
        const char *text = safe_cstring((CString *)node->data);
        int written = snprintf(buffer + used, sizeof(buffer) - used, "%s%s",
                               used == 0 ? "" : ", ", text);
        if (written < 0) {
            break;
        }
        if ((size_t)written >= sizeof(buffer) - used) {
            used = sizeof(buffer) - 1;
            break;
        }
        used += (size_t)written;
    }

    DBTraceLogf(stage, "%s: %s", label, buffer);
}

static void format_cstring_list(CList *list, char *buffer, size_t size) {
    size_t used = 0;

    if (!buffer || size == 0) {
        return;
    }

    buffer[0] = '\0';
    if (!list || !list->head) {
        append_text(buffer, size, &used, "(empty)");
        return;
    }

    for (CListNode *node = list->head; node; node = node->next) {
        if (used > 0) {
            append_text(buffer, size, &used, ", ");
        }
        append_text(buffer, size, &used, safe_cstring((CString *)node->data));
    }
}

static void format_schema_field_names(Schema *schema, char *buffer, size_t size) {
    size_t used = 0;

    if (!buffer || size == 0) {
        return;
    }

    buffer[0] = '\0';
    if (!schema || !schema->fields) {
        append_text(buffer, size, &used, "(empty)");
        return;
    }

    for (FieldNode *field = schema->fields; field; field = field->next) {
        if (used > 0) {
            append_text(buffer, size, &used, ", ");
        }
        append_text(buffer, size, &used, safe_cstring(field->fileName));
    }
}

static void trace_schema_fields(Schema *schema) {
    char buffer[512];
    size_t used = 0;

    buffer[0] = '\0';
    if (!schema || !schema->fields) {
        DBTraceLog("PARSE", "fields: (empty)");
        return;
    }

    for (FieldNode *field = schema->fields; field; field = field->next) {
        if (used > 0) {
            append_text(buffer, sizeof(buffer), &used, ", ");
        }
        append_text(buffer, sizeof(buffer), &used, safe_cstring(field->fileName));
        append_text(buffer, sizeof(buffer), &used, " ");
        append_text(buffer, sizeof(buffer), &used, schema_type_name(field->type));
        if (field->type == FILE_INFO_CODE_VARCHAR || field->type == FILE_INFO_CODE_CHAR) {
            char length[32];
            snprintf(length, sizeof(length), "(%d)", field->length);
            append_text(buffer, sizeof(buffer), &used, length);
        }
    }

    DBTraceLogf("PARSE", "fields: %s", buffer);
}

static char *safe_predicate_string(Predicate *predicate) {
    if (!predicate) {
        return strdup("(none)");
    }

    char *text = PredicateToString(predicate);
    if (!text || text[0] == '\0') {
        free(text);
        return strdup("(none)");
    }
    return text;
}

static char *safe_expression_string(Expression *expression) {
    if (!expression) {
        return strdup("(none)");
    }

    char *text = ExpressionToString(expression);
    if (!text || text[0] == '\0') {
        free(text);
        return strdup("(none)");
    }
    return text;
}

static char *safe_query_string(QueryData *queryData) {
    if (!queryData) {
        return strdup("(none)");
    }

    char *text = QueryDataToString(queryData);
    if (!text || text[0] == '\0') {
        free(text);
        return strdup("(none)");
    }
    return text;
}

static void trace_insert_values(InsertData *data) {
    char buffer[512];
    size_t used = 0;

    buffer[0] = '\0';
    if (!data || !data->flds || !data->vals) {
        DBTraceLog("PARSE", "values: (unavailable)");
        return;
    }

    CListNode *fieldNode = data->flds->head;
    CListNode *valueNode = data->vals->head;
    while (fieldNode && valueNode) {
        char *valueText = ConstantToString((Constant *)valueNode->data);
        if (used > 0) {
            append_text(buffer, sizeof(buffer), &used, ", ");
        }
        append_text(buffer, sizeof(buffer), &used, safe_cstring((CString *)fieldNode->data));
        append_text(buffer, sizeof(buffer), &used, "=");
        append_text(buffer, sizeof(buffer), &used, valueText ? valueText : "(null)");
        free(valueText);
        fieldNode = fieldNode->next;
        valueNode = valueNode->next;
    }

    DBTraceLogf("PARSE", "values: %s", used > 0 ? buffer : "(empty)");
}

void QueryDataTrace(QueryData *data) {
    if (!g_trace_enabled) {
        return;
    }

    DBTraceLog("PARSE", "QueryData");
    DBTracePush();
    if (!data) {
        DBTraceLog("PARSE", "(null)");
        DBTracePop();
        return;
    }

    trace_cstring_list("PARSE", "fields", data->fields);
    trace_cstring_list("PARSE", "tables", data->tables);

    char *predicate = PredicateToString(data->predicate);
    DBTraceLogf("PARSE", "predicate: %s", predicate && predicate[0] ? predicate : "(none)");
    free(predicate);
    DBTracePop();
}

void CommandDataTrace(CommandData *commandData) {
    if (!g_trace_enabled) {
        return;
    }

    if (!commandData) {
        DBTraceLog("UPDATE", "unknown command");
        return;
    }

    switch (commandData->code) {
        case CMD_CREATE_TABLE:
            DBTraceLog("UPDATE", "CREATE TABLE command");
            DBTracePush();
            DBTraceLogf("PARSE", "table: %s", safe_cstring(commandData->data.createTableData->tblname));
            trace_schema_fields(commandData->data.createTableData->schema);
            DBTracePop();
            break;
        case CMD_CREATE_VIEW: {
            char *query = safe_query_string(commandData->data.createViewData->queryData);
            DBTraceLog("UPDATE", "CREATE VIEW command");
            DBTracePush();
            DBTraceLogf("PARSE", "view: %s", safe_cstring(commandData->data.createViewData->viewName));
            DBTraceLogf("PARSE", "definition: %s", query);
            DBTracePop();
            free(query);
            break;
        }
        case CMD_CREATE_INDEX:
            DBTraceLog("UPDATE", "CREATE INDEX command");
            DBTracePush();
            DBTraceLogf("PARSE", "index: %s", safe_cstring(commandData->data.createIndexData->idxname));
            DBTraceLogf("PARSE", "table: %s", safe_cstring(commandData->data.createIndexData->tblname));
            DBTraceLogf("PARSE", "field: %s", safe_cstring(commandData->data.createIndexData->fldname));
            DBTracePop();
            break;
        case CMD_INSERT_DATA:
            DBTraceLog("UPDATE", "INSERT command");
            DBTracePush();
            DBTraceLogf("PARSE", "table: %s", safe_cstring(commandData->data.insertData->tblname));
            trace_insert_values(commandData->data.insertData);
            DBTracePop();
            break;
        case CMD_MODIFY_DATA: {
            char *newValue = safe_expression_string(commandData->data.modifyData->newVal);
            char *predicate = safe_predicate_string(commandData->data.modifyData->predicate);
            DBTraceLog("UPDATE", "UPDATE command");
            DBTracePush();
            DBTraceLogf("PARSE", "table: %s", safe_cstring(commandData->data.modifyData->tblname));
            DBTraceLogf("PARSE", "set: %s=%s",
                        safe_cstring(commandData->data.modifyData->fldname), newValue);
            DBTraceLogf("PARSE", "predicate: %s", predicate);
            DBTracePop();
            free(newValue);
            free(predicate);
            break;
        }
        case CMD_DELETE_DATA: {
            char *predicate = safe_predicate_string(commandData->data.deleteData->predicate);
            DBTraceLog("UPDATE", "DELETE command");
            DBTracePush();
            DBTraceLogf("PARSE", "table: %s", safe_cstring(commandData->data.deleteData->tblname));
            DBTraceLogf("PARSE", "predicate: %s", predicate);
            DBTracePop();
            free(predicate);
            break;
        }
        default:
            DBTraceLogf("UPDATE", "command code=%d", commandData->code);
            break;
    }
}

void UpdateExecutionTraceCreateTable(CreateTableData *data) {
    DBTraceLog("PLANNER", "dispatch to BasicUpdatePlannerExecuteCreateTable");
    DBTraceLogf("METADATA", "register table schema in catalog%s%s",
                data ? " for " : "", data ? safe_cstring(data->tblname) : "");
}

void UpdateExecutionTraceCreateView(CreateViewData *data) {
    DBTraceLog("PLANNER", "dispatch to BasicUpdatePlannerExecuteCreateView");
    DBTraceLogf("METADATA", "register view definition in catalog%s%s",
                data ? " for " : "", data ? safe_cstring(data->viewName) : "");
}

void UpdateExecutionTraceCreateIndex(CreateIndexData *data) {
    DBTraceLog("PLANNER", "dispatch to BasicUpdatePlannerExecuteCreateIndex");
    DBTraceLogf("METADATA", "register index metadata%s%s",
                data ? " for " : "", data ? safe_cstring(data->idxname) : "");
}

void UpdateExecutionTraceInsert(InsertData *data) {
    DBTraceLog("PLANNER", "dispatch to BasicUpdatePlannerExecuteInsert");
    DBTraceLogf("SCAN", "open table scan for %s", data ? safe_cstring(data->tblname) : "(null)");
    DBTraceLog("RECORD", "insert new record");
    DBTraceLog("TX", "write through transaction layer");
}

void UpdateExecutionTraceModify(ModifyData *data) {
    DBTraceLog("PLANNER", "dispatch to BasicUpdatePlannerExecuteModify");
    DBTraceLogf("SCAN", "locate records matching predicate on %s",
                data ? safe_cstring(data->tblname) : "(null)");
    DBTraceLog("RECORD", "update matched record");
    DBTraceLog("TX", "write old value log and mark modified");
}

void UpdateExecutionTraceDelete(DeleteData *data) {
    DBTraceLog("PLANNER", "dispatch to BasicUpdatePlannerExecuteDelete");
    DBTraceLogf("SCAN", "locate records matching predicate on %s",
                data ? safe_cstring(data->tblname) : "(null)");
    DBTraceLog("RECORD", "delete matched record");
    DBTraceLog("TX", "write through transaction layer");
}

void TransactionTraceCommit(Transaction *transaction) {
    DBTraceLog("TX", "COMMIT command");
    DBTraceLogf("TX", "transaction: %d", transaction ? transaction->txNum : -1);
    DBTraceLog("TX", "flush modified buffers");
    DBTraceLog("TX", "write commit log");
    DBTraceLog("OUTPUT", "transaction committed");
}

void TransactionTraceRollback(Transaction *transaction) {
    DBTraceLog("TX", "ROLLBACK command");
    DBTraceLogf("TX", "transaction: %d", transaction ? transaction->txNum : -1);
    DBTraceLog("TX", "rollback transaction changes if any");
    DBTraceLog("OUTPUT", "transaction rolled back");
}

static void trace_plan_node(Plan *plan) {
    if (!plan) {
        DBTraceLog("PLAN", "(null)");
        return;
    }

    switch (plan->code) {
        case PLAN_PROJECT_CODE: {
            char fields[512];
            format_schema_field_names(plan->planUnion.projectPlan ? plan->planUnion.projectPlan->schema : NULL,
                                      fields, sizeof(fields));
            DBTraceLogf("PLAN", "ProjectPlan fields=%s", fields);
            DBTracePush();
            trace_plan_node(plan->planUnion.projectPlan ? plan->planUnion.projectPlan->p : NULL);
            DBTracePop();
            break;
        }
        case PLAN_SELECT_CODE: {
            char *predicate = NULL;
            if (plan->planUnion.selectPlan) {
                predicate = PredicateToString(plan->planUnion.selectPlan->predicate);
            }
            DBTraceLogf("PLAN", "SelectPlan predicate=%s",
                        predicate && predicate[0] ? predicate : "(none)");
            free(predicate);
            DBTracePush();
            trace_plan_node(plan->planUnion.selectPlan ? plan->planUnion.selectPlan->p : NULL);
            DBTracePop();
            break;
        }
        case PLAN_PRODUCT_CODE:
            DBTraceLog("PLAN", "ProductPlan");
            DBTracePush();
            if (plan->planUnion.productPlan) {
                trace_plan_node(plan->planUnion.productPlan->p1);
                trace_plan_node(plan->planUnion.productPlan->p2);
            }
            DBTracePop();
            break;
        case PLAN_TABLE_CODE:
            DBTraceLogf("PLAN", "TablePlan table=%s",
                        plan->planUnion.tablePlan ? safe_cstring(plan->planUnion.tablePlan->tblname) : "(null)");
            break;
        default:
            DBTraceLogf("PLAN", "Plan code=%d", plan->code);
            break;
    }
}

void PlanTraceTree(Plan *plan) {
    if (!g_trace_enabled) {
        return;
    }

    DBTraceLog("PLAN", "Plan tree");
    DBTracePush();
    trace_plan_node(plan);
    DBTracePop();
}

static void trace_scan_node(Scan *scan) {
    if (!scan) {
        DBTraceLog("SCAN", "(null)");
        return;
    }

    switch (scan->code) {
        case SCAN_PROJECT_CODE: {
            char fields[512];
            format_cstring_list(scan->scanUnion.projectScan ? scan->scanUnion.projectScan->fieldList : NULL,
                                fields, sizeof(fields));
            DBTraceLogf("SCAN", "ProjectScan fields=%s", fields);
            DBTracePush();
            trace_scan_node(scan->scanUnion.projectScan ? scan->scanUnion.projectScan->s : NULL);
            DBTracePop();
            break;
        }
        case SCAN_SELECT_CODE:
            DBTraceLog("SCAN", "SelectScan");
            DBTracePush();
            trace_scan_node(scan->scanUnion.selectScan ? scan->scanUnion.selectScan->s : NULL);
            DBTracePop();
            break;
        case SCAN_PRODUCT_CODE:
            DBTraceLog("SCAN", "ProductScan");
            DBTracePush();
            if (scan->scanUnion.productScan) {
                trace_scan_node(scan->scanUnion.productScan->s1);
                trace_scan_node(scan->scanUnion.productScan->s2);
            }
            DBTracePop();
            break;
        case SCAN_TABLE_CODE:
            DBTraceLogf("SCAN", "TableScan file=%s",
                        scan->scanUnion.tableScan ? safe_cstring(scan->scanUnion.tableScan->fileName) : "(null)");
            break;
        default:
            DBTraceLogf("SCAN", "Scan code=%d", scan->code);
            break;
    }
}

void ScanTraceChain(Scan *scan) {
    if (!g_trace_enabled) {
        return;
    }

    DBTraceLog("SCAN", "Scan chain");
    DBTracePush();
    trace_scan_node(scan);
    DBTracePop();
}
