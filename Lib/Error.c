#include "Error.h"

Error *ErrorInit() {
    Error *error = malloc(sizeof(Error));
    if (!error) {
        return NULL;
    }

    error->errorCode = Error_NULL;
    error->reason = NULL;
    return error;
}
