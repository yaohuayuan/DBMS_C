//
// Created by Lenovo on 2025/7/31.
//

#include "DBMS.h"
#include "MetadataManager.h"
#include "BasicUpdatePlanner.h"
#include "BasicQueryPlanner.h"
#include "Planner.h"
#include "LogManager.h"
SimpleDB *SimpleDBInit(char *dirname,int blocksize,int buffersize){
    SimpleDB *simpleDB = malloc(sizeof (SimpleDB));
    if(blocksize==SIMPLE_DB_INIT_VAL||buffersize==SIMPLE_DB_INIT_VAL){
        simpleDB->fileManager = FileManagerInit(CStringCreateFromCStr(dirname) ,BLOCK_SIZE);
        simpleDB->logManager = LogManagerInit(simpleDB->fileManager, CStringCreateFromCStr(LOG_FILE));
        simpleDB->bufferManager = BufferManagerInit(simpleDB->fileManager,simpleDB->logManager,BUFFER_SIZE);
//        simpleDB->transactionManager = TransactionManagerInit();
        Transaction *tx = SimpleDataNewTX(simpleDB);
        bool isNew = simpleDB->fileManager->isNew;
        if(isNew){
            printf("creating new database\n");
        }else{
            printf("recovering existing database\n");
        }
        simpleDB->metadataMgr = MetadataMgrInit(isNew,tx);
        BasicUpdatePlanner *updatePlanner = BasicUpdatePlannerInit(simpleDB->metadataMgr);
        BasicQueryPlanner *queryPlanner = BasicQueryPlannerInit(simpleDB->metadataMgr);
        simpleDB->planer = PlannerInit(queryPlanner,updatePlanner);
        TransactionCommit(tx);
    }else{
        simpleDB->fileManager = FileManagerInit(CStringCreateFromCStr(dirname),blocksize);
        simpleDB->logManager = LogManagerInit(simpleDB->fileManager,CStringCreateFromCStr(LOG_FILE));
        simpleDB->bufferManager = BufferManagerInit(simpleDB->fileManager,simpleDB->logManager,buffersize);
    }

    return simpleDB;
}
Transaction *SimpleDataNewTX(SimpleDB*simpleDb){
    return TransactionInit(simpleDb->fileManager,simpleDb->logManager,simpleDb->bufferManager);
}
Transaction *SimpleDataNewTXName(SimpleDB*simpleDb,char **name){
    Transaction *transaction = TransactionInit(simpleDb->fileManager,simpleDb->logManager,simpleDb->bufferManager);
//    TransactionManagerAdd(simpleDb->transactionManager,name,transaction);
    return transaction;
}
