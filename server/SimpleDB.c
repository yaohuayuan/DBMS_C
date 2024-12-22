//
// Created by yaohuayuan on 2024/11/21.
//

#include "SimpleDB.h"
#include "SimpleDB.h"

SimpleDB *SimpleDBInit(char *dirname,int blocksize,int buffersize){
    SimpleDB *simpleDB = malloc(sizeof (SimpleDB));
    if(blocksize==SIMPLE_DB_INIT_VAL||buffersize==SIMPLE_DB_INIT_VAL){
        simpleDB->fileManager = FileManagerInit(dirname,BLOCK_SIZE);
        simpleDB->logManager = LogManagerInit(simpleDB->fileManager,LOG_FILE);
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
        simpleDB->fileManager = FileManagerInit(dirname,blocksize);
        simpleDB->logManager = LogManagerInit(simpleDB->fileManager,LOG_FILE);
        simpleDB->bufferManager = BufferManagerInit(simpleDB->fileManager,simpleDB->logManager,buffersize);
    }

    return simpleDB;
}
Transaction *SimpleDataNewTX(SimpleDB*simpleDb){
    return TransactionInit(simpleDb->fileManager,simpleDb->logManager,simpleDb->bufferManager);
}
Transaction *SimpleDataNewTXName(SimpleDB*simpleDb,char *name){
    Transaction *transaction = TransactionInit(simpleDb->fileManager,simpleDb->logManager,simpleDb->bufferManager);
//    TransactionManagerAdd(simpleDb->transactionManager,name,transaction);
    return transaction;
}