#ifndef DBMS_C_DBERROR_H
#define DBMS_C_DBERROR_H

#include <stdbool.h>
#include <stdio.h>

typedef enum DBStatus {
    DB_OK = 0,

    DB_ERR_INVALID_ARGUMENT,
    DB_ERR_OUT_OF_MEMORY,

    DB_ERR_FILE_OPEN_FAILED,
    DB_ERR_FILE_READ_FAILED,
    DB_ERR_FILE_WRITE_FAILED,

    DB_ERR_PARSE_SYNTAX,
    DB_ERR_PARSE_UNSUPPORTED,

    DB_ERR_TABLE_NOT_FOUND,
    DB_ERR_FIELD_NOT_FOUND,
    DB_ERR_TYPE_MISMATCH,
    DB_ERR_RECORD_NOT_FOUND,
    DB_ERR_SLOT_OUT_OF_RANGE,

    DB_ERR_BUFFER_FULL,

    DB_ERR_LOCK_CONFLICT,
    DB_ERR_LOCK_TIMEOUT,
    DB_ERR_DEADLOCK_DETECTED,

    DB_ERR_TX_ABORTED,
    DB_ERR_LOG_WRITE_FAILED,
    DB_ERR_RECOVERY_FAILED,

    DB_ERR_INTERNAL,
} DBStatus;

typedef struct DBError {
    DBStatus code;
    const char *module;
    const char *file;
    int line;
    char message[256];
} DBError;

void DBErrorInit(DBError *err);
void DBErrorClear(DBError *err);
bool DBStatusIsOk(DBStatus status);
const char *DBStatusToString(DBStatus status);
void DBErrorSet(DBError *err, DBStatus code, const char *module, const char *file, int line, const char *message);
void DBErrorSetf(DBError *err, DBStatus code, const char *module, const char *file, int line, const char *fmt, ...);
void DBErrorPrint(const DBError *err, FILE *out);

#define DB_SET_ERROR(err, code, module, message) \
    DBErrorSet((err), (code), (module), __FILE__, __LINE__, (message))

#define DB_SET_ERRORF(err, code, module, fmt, ...) \
    DBErrorSetf((err), (code), (module), __FILE__, __LINE__, (fmt), ##__VA_ARGS__)

#define DB_RETURN_ERROR(err, code, module, message) \
    do { \
        DB_SET_ERROR((err), (code), (module), (message)); \
        return (code); \
    } while (0)

#define DB_TRY(expr) \
    do { \
        DBStatus db_status__ = (expr); \
        if (!DBStatusIsOk(db_status__)) { \
            return db_status__; \
        } \
    } while (0)

#endif // DBMS_C_DBERROR_H
