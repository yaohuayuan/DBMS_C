#include "DBError.h"

#include <stdarg.h>
#include <string.h>

void DBErrorInit(DBError *err) {
    if (!err) {
        return;
    }

    err->code = DB_OK;
    err->module = NULL;
    err->file = NULL;
    err->line = 0;
    err->message[0] = '\0';
}

void DBErrorClear(DBError *err) {
    DBErrorInit(err);
}

bool DBStatusIsOk(DBStatus status) {
    return status == DB_OK;
}

const char *DBStatusToString(DBStatus status) {
    switch (status) {
        case DB_OK:
            return "DB_OK";
        case DB_ERR_INVALID_ARGUMENT:
            return "DB_ERR_INVALID_ARGUMENT";
        case DB_ERR_OUT_OF_MEMORY:
            return "DB_ERR_OUT_OF_MEMORY";
        case DB_ERR_FILE_OPEN_FAILED:
            return "DB_ERR_FILE_OPEN_FAILED";
        case DB_ERR_FILE_READ_FAILED:
            return "DB_ERR_FILE_READ_FAILED";
        case DB_ERR_FILE_WRITE_FAILED:
            return "DB_ERR_FILE_WRITE_FAILED";
        case DB_ERR_PARSE_SYNTAX:
            return "DB_ERR_PARSE_SYNTAX";
        case DB_ERR_PARSE_UNSUPPORTED:
            return "DB_ERR_PARSE_UNSUPPORTED";
        case DB_ERR_TABLE_NOT_FOUND:
            return "DB_ERR_TABLE_NOT_FOUND";
        case DB_ERR_FIELD_NOT_FOUND:
            return "DB_ERR_FIELD_NOT_FOUND";
        case DB_ERR_TYPE_MISMATCH:
            return "DB_ERR_TYPE_MISMATCH";
        case DB_ERR_RECORD_NOT_FOUND:
            return "DB_ERR_RECORD_NOT_FOUND";
        case DB_ERR_SLOT_OUT_OF_RANGE:
            return "DB_ERR_SLOT_OUT_OF_RANGE";
        case DB_ERR_BUFFER_FULL:
            return "DB_ERR_BUFFER_FULL";
        case DB_ERR_LOCK_CONFLICT:
            return "DB_ERR_LOCK_CONFLICT";
        case DB_ERR_LOCK_TIMEOUT:
            return "DB_ERR_LOCK_TIMEOUT";
        case DB_ERR_DEADLOCK_DETECTED:
            return "DB_ERR_DEADLOCK_DETECTED";
        case DB_ERR_TX_ABORTED:
            return "DB_ERR_TX_ABORTED";
        case DB_ERR_LOG_WRITE_FAILED:
            return "DB_ERR_LOG_WRITE_FAILED";
        case DB_ERR_RECOVERY_FAILED:
            return "DB_ERR_RECOVERY_FAILED";
        case DB_ERR_INTERNAL:
            return "DB_ERR_INTERNAL";
        default:
            return "DB_ERR_UNKNOWN";
    }
}

void DBErrorSet(DBError *err, DBStatus code, const char *module, const char *file, int line, const char *message) {
    if (!err) {
        return;
    }

    err->code = code;
    err->module = module;
    err->file = file;
    err->line = line;
    snprintf(err->message, sizeof(err->message), "%s", message ? message : "");
}

void DBErrorSetf(DBError *err, DBStatus code, const char *module, const char *file, int line, const char *fmt, ...) {
    if (!err) {
        return;
    }

    err->code = code;
    err->module = module;
    err->file = file;
    err->line = line;

    if (!fmt) {
        err->message[0] = '\0';
        return;
    }

    va_list args;
    va_start(args, fmt);
    vsnprintf(err->message, sizeof(err->message), fmt, args);
    va_end(args);
}

void DBErrorPrint(const DBError *err, FILE *out) {
    if (!err) {
        return;
    }

    if (!out) {
        out = stderr;
    }

    fprintf(out, "[%s]", DBStatusToString(err->code));
    if (err->module) {
        fprintf(out, " module=%s", err->module);
    }
    if (err->file) {
        fprintf(out, " at %s:%d", err->file, err->line);
    }
    if (err->message[0] != '\0') {
        fprintf(out, " %s", err->message);
    }
    fprintf(out, "\n");
}
