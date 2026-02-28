//
// Created by yaohuayuan on 2024/10/21.
//

#include "Error.h"
Error * ErrorInit(){
    Error *error = malloc(sizeof (Error));
    error->errorCode=Error_NULL;
    error->reason = NULL;
    return error;
}
