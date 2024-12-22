#include "test.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "map.h"
#include "Schema.h"
#include "Transaction.h"
#include "Trie.h"
#include "MetadataManager.h"
#include "StatInfo.h"
#include "List.h"
#include "SimpleDB.h"
#include "TransactionManager.h"

// 测试 BlockID 功能
void BlockTest() {
    BlockID id1, id2, id3;
    char *name = "王立泽";
    int id = 1;

    printf("%s\n", name);
    BlockID_Init(&id1, name, id);
    BlockID_Init(&id2, name, id);
    printf("%s%d\n", getFileName(id1), getBlockId(id1));
    printf("%s\n", BlockIDToString(id1));
    printf("%d\n", BlockIdEqual(id1, id2));

    BlockID_Init(&id3, name, id + 1);
    printf("%d\n", BlockIdEqual(id1, id3));
}

//// 测试 ByteBuffer 功能(旧版)
//void testByteBuffer() {
//    ByteBuffer *buffer = bufferAllocate(64);
//    if (buffer == NULL) {
//        printf("分配缓冲区失败。\n");
//        return;
//    }
//
//    // 向缓冲区写入数据
//    bufferPutInt(buffer, 123);
//    bufferFlip(buffer);
//
//    int32_t intData;
//    bufferGetIntPosition(buffer, 0, &intData);
//    printf("读取的32位数据: %d\n", intData);
//
//    // 测试写入和读取不同位置的值
//    bufferPutPosition(buffer, 0, 1);
//    bufferPutShortPosition(buffer, 1, 255);
//    bufferPutIntPosition(buffer, 3, 0x12345678);
//    bufferPutLongPosition(buffer, 7, 0xABCDEF123456789);
//
//    int8_t byteData;
//    int16_t shortData;
//    int64_t longData;
//    bufferFlip(buffer);
//
//    bufferGetPosition(buffer, 0, &byteData);
//    bufferGetShortPosition(buffer, 1, &shortData);
//    bufferGetIntPosition(buffer, 3, &intData);
//    bufferGetLongPosition(buffer, 7, &longData);
//
//    printf("指定位置读取的字节数据: %d\n", byteData);
//    printf("指定位置读取的16位数据: %d\n", shortData);
//    printf("指定位置读取的32位数据: 0x%08X\n", intData);
//    printf("指定位置读取的64位数据: 0x%016llX\n", longData);
//
//    bufferFree(buffer);
//}

// 测试文件写入
void file_test() {
    FILE *p = fopen("test\\test.txt", "w");
    fprintf(p, "%s", "hello world!");
    fclose(p);
}

// 测试 Page 相关功能
//void PageTest() {
//    Page p;
//    PageInit(&p, 1024);
//    PageSetString(&p, 0, "测试字符串");
//    char *str = PageGetString(&p, 0);
//    printf("页面内容: %s\n", str);
//}

// 测试 FileManager 和 OpenFile 功能
void TestFileManagerAndOpenFile() {
    char *testDir = "./testDB";
    mkdir(testDir);

    FILE *file1 = fopen("./testDB/tempFile1.txt", "w");
    FILE *file2 = fopen("./testDB/regularFile.txt", "w");
    FILE *file3 = fopen("./testDB/tempFile2.txt", "w");

    fclose(file1);
    fclose(file2);
    fclose(file3);

    FileManager *fm = FileManagerInit(testDir, 1024);
    printf("Remaining files in directory:\n");
    struct dirent *ptr;
    while ((ptr = readdir(fm->dbDirectory)) != NULL) {
        if (strcmp(ptr->d_name, ".") != 0 && strcmp(ptr->d_name, "..") != 0) {
            printf("File: %s\n", ptr->d_name);
        }
    }

    OpenFileAdd(fm->openFile, "testFile1.txt");
    OpenFileAdd(fm->openFile, "testFile2.txt");

    FileSet *testFile = GetFile(fm, "testFile1.txt");
    if (testFile == NULL) {
        printf("File 'testFile1.txt' not opened yet.\n");
    }

    remove("./testDB/regularFile.txt");
    rmdir(testDir);

    printf("Test complete.\n");
}

// 测试 FileManager 和页面的写入与读取功能
void testFileManager() {
    char *dbDirectoryName = "test_db";
    int blockSize = 1024;
    FileManager *fm = FileManagerInit(dbDirectoryName, blockSize);
    Page *page, *p;
    page = PageInit(blockSize);
    p=PageInit(blockSize);
    BlockID blockId = { "testfile4.txt", 1 };
    int pos = 380;
    PageSetString(page, pos, "王立泽");
    FileManagerWrite(fm, blockId, page);
    FileManagerRead(fm, blockId, p);
    char *str = PageGetString(p, pos);
    printf("%s\n", str);

    FileManagerClose(fm);
}
void testByteBuffer(){
    ByteBuffer *byteBuffer = bufferAllocate(1024);
    bufferPutInt(byteBuffer,123);
    ByteBufferData  *bufferData = ByteBufferDataInit();
    if(bufferGetLongPosition(byteBuffer,0,bufferData)==0)
        printf("123");
    char*data = "王立泽";
    int len = strlen(data);
    uint8_t *type = malloc(sizeof (uint8_t)*len);
    type[0] = BYTEBUFFER_STRING;
    bufferPutBytesPosition(byteBuffer,8,data,type,len);
    bufferGetBytesPosition(byteBuffer,8,bufferData,len);
    printf("%s",bufferData->bytesData->data);
}
void testPage(){
    Page *p;
    p=PageInit(1024);
    PageSetString(p, 0, "qwe");
    char *str = PageGetString(p, 0);
    printf("页面内容: %s\n", str);
}
BytesData * LogCreateLogRecord(char*str,int n){
    int spos = 0,nops = spos+ PageMaxLength(strlen(str));
    BytesData *bytesData = malloc(sizeof(BytesData));
    bytesData->length = nops+sizeof(int);
    ByteBuffer *byteBuffer = bufferAllocate(bytesData->length);
    bytesData->data = byteBuffer->data;
    bytesData->type = byteBuffer->type;
    Page *p = PageInitBytes(byteBuffer);
    PageSetString(p,0,str);
    PageSetInt(p,strlen(str)+sizeof(int),n);
//    char *s = PageGetString(p,0);
//    printf("%s",PageGetString(p,0));
//    ByteBufferData *byteBufferData = ByteBufferDataInit();
//    PageGetInt(p,strlen(str)+sizeof(int),byteBufferData);
//    printf("%d",*byteBufferData->intData);
    return bytesData;
}
void  LogCreateRecord(LogManager *logManager,int start,int end){
    printf("Creating records:\n");
    char str[128]; ;
    for(int i=start;i<end;i++){
        sprintf(str,"record+%d",i);
        ByteBufferData *byteBufferData = ByteBufferDataInit();
        byteBufferData->bytesData = LogCreateLogRecord(str,i);
        int lsn = LogManagerAppend(logManager,byteBufferData->bytesData->data,byteBufferData->bytesData->type,byteBufferData->bytesData->length);
        printf("%d ",lsn);
//        ByteBufferData *out = ByteBufferDataInit();
//        PageGetInt(logManager->logPage,0,out);
//        out=PageGetBytes(logManager->logPage,380);
//        ByteBuffer *byteBuffer = bufferAllocate(out->bytesData->length);
//        byteBuffer->type = out->bytesData->type;
//        byteBuffer->data = out->bytesData->data;
//        Page  *p = PageInitBytes(byteBuffer);
//        ByteBufferData *out1 = ByteBufferDataInit();
//        char*ans = PageGetString(p,0);
//        printf("%s\n",ans);
//        printf("%d\n",lsn);
    }
    printf("\n");
}
void LogPrintRecord(LogManager *logManager,char*str){
    printf("%s\n",str);
    LogIterator *logIterator= LogManager2LogManager(logManager);
    while(LogIteratorHasNext(logIterator)){
        ByteBufferData *byteBufferData = LogIteratorNext(logIterator);
        ByteBuffer * buffer =bufferAllocate(byteBufferData->bytesData->length);
        buffer->data = byteBufferData->bytesData->data;
        buffer->type = byteBufferData->bytesData->type;
        Page *p = PageInitBytes(buffer);
        char* s = PageGetString(p,0);
        int nos = PageMaxLength(strlen(s));
        ByteBufferData *byteBufferData1 = ByteBufferDataInit();
//        PageGetInt(p,0,byteBufferData1);
        PageGetInt(p,nos,byteBufferData1);
        char out[128];
        sprintf(out,"%s:%d",s,*byteBufferData1->intData);
        printf("%s\n",out);
    }
}
void testLog(){
    time_t timep;
    struct tm *p;
    time(&timep);
    p = gmtime(&timep);
    char fileName[512];
    char *filename="123";
    sprintf(fileName,"log_%d_%d_%d_%d_%d_%d",p->tm_year+1900,p->tm_mon+1,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec);
    FileManager *fileManager= FileManagerInit("Logtest",400);
    LogManager *logManager = LogManagerInit(fileManager,fileName);
    LogPrintRecord(logManager,"The initial empty log file:");
    LogCreateRecord(logManager,1,10000);
    LogManagerFlushLSN(logManager,100000);
    LogPrintRecord(logManager,"The log file now has these records:");
}
void testSize(){
    int a = 17,b = 21;
    printf("%d",(a-b)<(int)sizeof(int));
}
char * getDataString(char *filename){
    time_t timep;
    struct tm *p;
    time(&timep);
    p = gmtime(&timep);
    char fileName[512];
    sprintf(fileName,"%s_%d_%d_%d_%d_%d_%d",filename,p->tm_year+1900,p->tm_mon+1,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec);
    return strdup(fileName);
}
void testFileManagerAppendNew(){
    FileManager *fm = FileManagerInit("testFileManager",20);
    Page *p = PageInit(20);
    Page *page = PageInit(20);
    PageSetInt(p,0,1);
    PageSetInt(p,4,2);
    PageSetInt(p,8,3);
    PageSetInt(p,12,1);
    PageSetInt(p,16,2);
    BlockID bld ;
  BlockID_Init(&bld,"test",0);
    FileManagerWrite(fm,bld,p);
//    FileManagerRead(fm,bld,page);
    BlockID newBlockId = FileManagerAppend(fm,"test2");
    PageSetInt(p,0,10);
    PageSetInt(p,4,20);
    PageSetInt(p,8,30);
    PageSetInt(p,12,10);
    PageSetInt(p,16,20);
    FileManagerWrite(fm,newBlockId,p);
    FileManagerRead(fm,bld,page);
    ByteBufferData *out = ByteBufferDataInit();
    PageGetInt(page,16,out);
    printf("%d",*out->intData);
}
void testBufferFile(){
    FileManager *fileManager = FileManagerInit("bufferfiletest",400);
    LogManager *logManager = LogManagerInit(fileManager,"log");
    BufferManager *bufferManager = BufferManagerInit(fileManager,logManager,8);
    BlockID blockId;
    BlockID_Init(&blockId,"testfile1",2);
    Buffer *b1 = BufferManagerPin(bufferManager,blockId);
    int pos1 = 88;
    Page *p = b1->page;
    PageSetString(p,pos1,"abcdefg");
    int pos2 = pos1 + PageMaxLength(strlen("abcdefg"));
    PageSetInt(p,pos2,1);
    BufferSetModified(b1,1,0);
    BufferManagerUnpin(bufferManager,b1);

    Buffer *b2 = BufferManagerPin(bufferManager,blockId);
    Page *page = b2->page;
    char*ans =PageGetString(page,pos1);
    ByteBufferData *bufferData = ByteBufferDataInit();
    PageGetInt(page,pos2,bufferData);
    printf("%s+%d",ans,*bufferData->intData);
    BufferManagerUnpin(bufferManager,b2);
}
void testBufferManager(){
    FileManager *fileManager = FileManagerInit("buffermgrtest",400);
    LogManager *logManager = LogManagerInit(fileManager,"log");
    BufferManager *bufferManager = BufferManagerInit(fileManager,logManager,3);
    Buffer **pBuffer  = malloc(sizeof(Buffer*)*6);
    for(int i=0;i<6;i++){
        pBuffer[i] = NULL;
    }
    BlockID blockId ;
    BlockID_Init(&blockId,"testfile",0);
    pBuffer[0] = BufferManagerPin(bufferManager,blockId);
    BlockID_Init(&blockId,"testfile",1);
    pBuffer[1] = BufferManagerPin(bufferManager,blockId);
    BlockID_Init(&blockId,"testfile",2);
    pBuffer[2] = BufferManagerPin(bufferManager,blockId);
    BufferManagerUnpin(bufferManager,pBuffer[1]);
    pBuffer[1] = NULL;
    BlockID_Init(&blockId,"testfile",0);
    pBuffer[3] = BufferManagerPin(bufferManager,blockId);
    BlockID_Init(&blockId,"testfile",1);
    pBuffer[4] = BufferManagerPin(bufferManager,blockId);
    int a = bufferManager->numAvailable;
    printf("%d",a);
    BufferManagerUnpin(bufferManager,pBuffer[2]);
    pBuffer[2] = NULL;
    BlockID_Init(&blockId,"testfile",3);
    pBuffer[5] = BufferManagerPin(bufferManager,blockId);
    pBuffer[5] = BufferManagerPin(bufferManager,blockId);
    for(int i=0;i<6;i++){
        Buffer *buffer = pBuffer[i];
        if(buffer!=NULL){
            printf("%d+%s\n",i,BlockIDToString(buffer->blockId));
        }
    }
}
void testBuffer(){
    FileManager *fileManager = FileManagerInit("buffertest",400);
    LogManager *logManager = LogManagerInit(fileManager,"log");
    BufferManager *bufferManager = BufferManagerInit(fileManager,logManager,3);
    BlockID blockId;
    BlockID_Init(&blockId,"testFile",1);
    Buffer * buffer = BufferManagerPin(bufferManager,blockId);
    Page *page = buffer->page;
    ByteBufferData  *byteBufferData = ByteBufferDataInit();
    PageGetInt(page,80,byteBufferData);
    int n = *byteBufferData->intData;
    PageSetInt(page,80,n+1);
    BufferSetModified(buffer,1,0);
    BufferManagerUnpin(bufferManager,buffer);
    PageGetInt(buffer->page,80,byteBufferData);
    int n1 = *byteBufferData->intData;
    Page *page2 = PageInit(fileManager->blockSize);
    FileManagerRead(fileManager,blockId,page2);
    ByteBufferData *bufferData = ByteBufferDataInit();
    PageGetInt(page2,80,byteBufferData);
    int n2 = *byteBufferData->intData;
    printf("%d\n",*byteBufferData->intData);
    blockId.blockId++;
    Buffer *buffer1 = BufferManagerPin(bufferManager,blockId);
    BufferManagerUnpin(bufferManager,buffer1);
    blockId.blockId=1;
    buffer1 = BufferManagerPin(bufferManager,blockId);
    Page *page1 = buffer1->page ;
    PageSetInt(page1,80,999);
    BufferSetModified(buffer1,1,0);

    FileManagerRead(fileManager,blockId,page2);
    PageGetInt(page2,80,byteBufferData);
    printf("%d",*byteBufferData->intData);
}
void mapTest(){
    // 创建并初始化一个整数类型的哈希映射
    map_int_t m;
    map_init(&m);

    // 添加一些键值对
    map_set(&m, "apple", 1);
    map_set(&m, "banana", 2);
    map_set(&m, "cherry", 3);

    // 获取值
    int *val = map_get(&m, "banana");
    if (val) {
        printf("Value for 'banana': %d\n", *val);
    } else {
        printf("Key 'banana' not found\n");
    }

    // 遍历所有的键值对
    const char *key;
    map_iter_t iter = map_iter(&m);
    while ((key = map_next(&m, &iter))) {
        printf("Key: %s, Value: %d\n", key, *map_get(&m, key));
    }

    // 清理资源
    map_deinit(&m);
}
void testMapStruct(){
    typedef struct {
        int id;
        char name[50];
    } MyStruct;
    map_t(MyStruct) my_map;
    map_init(&my_map);

    // 添加数据
    MyStruct data1 = {1, "Example1"};
    map_set(&my_map, "key1", data1);

    MyStruct data2 = {2, "Example2"};
    map_set(&my_map, "key2", data2);

    // 获取数据
    MyStruct *retrieved_2data = &data1;
    MyStruct *retrieved_1data = (MyStruct *)map_get(&my_map, "key1");
    retrieved_1data->id=4;
    MyStruct *retrieved_data = (MyStruct *)map_get(&my_map, "key1");
    if (retrieved_data) {
        printf("ID: %d, Name: %s\n", retrieved_data->id, retrieved_data->name);
    } else {
        printf("未找到 key1\n");
    }

    // 清理资源
    map_deinit(&my_map);

}
void test_SchemaAddField() {
    // 创建Schema
    Schema *schema = SchemaInit();
    if (!schema) {
        fprintf(stderr, "Failed to allocate memory for Schema\n");
        return;
    }

    // 添加字段
    SchemaAddField(schema, "name", FILE_INFO_CODE_VARCHAR, 50);
    SchemaAddField(schema, "age", FILE_INFO_CODE_INTEGER, 0);
    SchemaAddField(schema, "address", FILE_INFO_CODE_VARCHAR, 100);

    // 验证链表
    FieldNode *current = schema->fields;  // 从头开始
    while (current) {
        printf("Field: %s, Type: %d, Length: %d\n", current->fileName, current->type, current->length);
        current = current->next;
    }

    // 验证哈希映射
    const char *keys[] = {"name", "age", "address"};
    for (int i = 0; i < 3; i++) {
        FileInfo *fileInfo = map_get(schema->MapFileInfo, keys[i]);  // 修改为直接获取 FileInfo*
        if (fileInfo) {
            printf("Map: Key: %s, Type: %d, Length: %d\n", keys[i], fileInfo->type, fileInfo->length);
        } else {
            printf("Key not found: %s\n", keys[i]);
        }
    }

    // 释放资源
    SchemaFree(schema);
}
void LayoutTest(){
    Schema *schema = SchemaInit();
    SchemaAddIntField(schema,"A");
    SchemaAddStringField(schema,"B",9);
    Layout *layout = LayoutInit(schema,NULL,0);
    FieldNode *fieldNode = layout->schema->fields;
    while(fieldNode!=NULL){
        int offset = LayoutOffset(layout,fieldNode->fileName);
        printf("%s,%d\n",fieldNode->fileName,offset);
        fieldNode = fieldNode->next;
    }
}
typedef struct LinkNode{
    int data;
    struct LinkNode *next;
}LinkNode;
LinkNode *LinkInit(){
    LinkNode*head = malloc(sizeof(LinkNode));
    head->next = NULL;
    return head;
}
void LinkAdd(LinkNode*head,int data){
    LinkNode *p = head;
    while(p->next!=NULL){
        p = p->next;
    }
    LinkNode *NewLinkNode = malloc(sizeof(LinkNode));
    NewLinkNode->next = NULL;
    NewLinkNode->data = data;
    p->next = NewLinkNode;
}
void LinkDelete(LinkNode*head,int data){
    LinkNode *p = head->next;
    LinkNode *q = head;
    while(p){
        if(p->data==data){
            q->next = p->next;
            break;
        }
        q = p;
        p = p->next;
    }
    free(p);
}
void LinkShow(LinkNode*head){
    int i = 0;
    LinkNode*p = head->next;
    while(p){
        printf("%d:%d\n",i++,p->data);
        p=p->next;
    }
}
void testLink(){
    LinkNode *head = LinkInit();
    LinkAdd(head,1);
    LinkAdd(head,2);
    LinkAdd(head,3);
    LinkShow(head);
    LinkDelete(head,4);
    LinkShow(head);
}
void testLogManager(LogManager*logManager){
    BlockID  blockId;
    BlockID_Init(&blockId,logManager->currentBlockId.fileName,0);
    Page *page = PageInit(400);
    FileManagerRead(logManager->fileManager,blockId,page);
    ByteBufferData *out = ByteBufferDataInit();
    PageGetInt(page, 0, out);
    int a = *out->intData;
    ByteBufferData *out1 = ByteBufferDataInit();
}
void test_BufferList() {
    // 初始化 BufferManager
    FileManager *fileManager = FileManagerInit("buffertest",400);
    LogManager *logManager = LogManagerInit(fileManager,"log");
    BufferManager *bufferManager = BufferManagerInit(fileManager,logManager,3);

    // 初始化 BufferList
    BufferList *bufferList = BufferListInit(bufferManager);


    // 创建一个 BlockID
    BlockID blockId;
    BlockID_Init(&blockId, "test_file.txt", 1);

    // 测试 BufferListPin
    Buffer *buffer = BufferListGetBuffer(bufferList, blockId);

    BufferListPin(bufferList, blockId);
    buffer = BufferListGetBuffer(bufferList, blockId);


    // 测试 BufferListUnPin
    BufferListUnPin(bufferList, blockId);
    buffer = BufferListGetBuffer(bufferList, blockId);


    // 测试 BufferListUnpinAll
    BufferListPin(bufferList, blockId); // 重新 pin 以测试 unpin all
    BufferListUnpinAll(bufferList);
    buffer = BufferListGetBuffer(bufferList, blockId);


    // 清理
    BufferListUnpinAll(bufferList); // 确保没有残留
    free(bufferList->pin);
    free(bufferList->buffers);
    free(bufferList);
}
void mapStrTest(){
    // 创建并初始化一个整数类型的哈希映射
    map_str_t m;
    map_init(&m);

    // 添加一些键值对
    map_set(&m, "apple", "12");
    map_set(&m, "banana", "23");
    map_set(&m, "cherry", "32");

    // 获取值
    char**val = map_get(&m, "banana");
    if (val) {
        printf("Value for 'banana': %s\n", *val);
    } else {
        printf("Key 'banana' not found\n");
    }

    // 遍历所有的键值对
    const char *key;
    map_iter_t iter = map_iter(&m);
    while ((key = map_next(&m, &iter))) {
        printf("Key: %s, Value: %s\n", key, *map_get(&m, key));
    }

    // 清理资源
    map_deinit(&m);
}
void testBlockIDConversion() {
    // 创建一个 BlockID 实例
    BlockID originalBlock;
    BlockID_Init(&originalBlock, "example.txt", 123);

    // 将 BlockID 转换为字符串
    char *str = BlockIDToString(originalBlock);
    if (str == NULL) {
        fprintf(stderr, "Failed to allocate memory for the string.\n");
        return;
    }
    printf("Generated string: %s\n", str);

    // 将字符串转换回 BlockID
    BlockID parsedBlock = BloCKIDString2BlockID(str);
    printf("Parsed BlockID - File: %s, ID: %d\n", parsedBlock.fileName, parsedBlock.blockId);

    // 检查转换前后是否相等
    if (BlockIdEqual(originalBlock, parsedBlock)) {
        printf("Conversion successful!\n");
    } else {
        printf("Conversion failed!\n");
    }

    // 清理
    free(str);
    free((void *)parsedBlock.fileName);
}
void OP(){
    printf("This is op\n");
}
void Function(){
    printf("This is Function\n");
}
void TestFunction(){
   void (*p)() = NULL;
   p = OP;
   p();
   p = Function;
   p();
}
void testStruct() {
    struct tese2;  // 前向声明 struct tese2

    typedef struct test1 {
        struct tese2 *tes2;  // 使用指针避免递归定义
        int a;
    } test1;

    struct tese2 {
        test1 test11;

    };
}
void testPrintRecord(char *LogName);
void testRecoveryPrintValue(FileManager *fileManager,BlockID blockId0,BlockID blockId1,char *msg){
    printf("%s\n",msg);
    Page *page0 = PageInit(fileManager->blockSize);
    Page *page1 = PageInit(fileManager->blockSize);
    FileManagerRead(fileManager,blockId1,page1);
    FileManagerRead(fileManager,blockId0,page0);
    int pos = 0;
    for(int i=0;i<6;i++){
        ByteBufferData *out0 = ByteBufferDataInit();
        PageGetInt(page0, pos, out0);
        ByteBufferData *out1 = ByteBufferDataInit();
        PageGetInt(page1, pos, out1);
        printf("%d:%d %d ",i,*out0->intData,*out1->intData);
        pos+=4;
    }
    printf("%s %s", PageGetString(page0,100), PageGetString(page1,100));
    printf("\n");

}
void testRecoveryInitialize(FileManager*fileManager,LogManager*logManager,BufferManager*bufferManager,BlockID blockId0,BlockID blockId1) {
    Transaction *tx1 = TransactionInit(fileManager,logManager,bufferManager);
    Transaction *tx2 = TransactionInit(fileManager,logManager,bufferManager);
    TransactionPin(tx1,blockId0);
    TransactionPin(tx2,blockId1);
    int pos = 0;
    for(int i=0;i<6;i++){
        TransactionSetInt(tx1,blockId0,pos,pos,false);
        TransactionSetInt(tx2,blockId1,pos,pos,false);
        pos+=sizeof(int);
    }
    TransactionSetString(tx1,blockId0,100,"abc",false);
    TransactionSetString(tx2,blockId1,100,"def",false);
    TransactionCommit(tx1);
    TransactionCommit(tx2);
    testRecoveryPrintValue(fileManager,blockId0,blockId1,"After Initialization:");
    testPrintRecord(logManager->logFile);
}

void testRecoveryModify(FileManager*fileManager,LogManager*logManager,BufferManager*bufferManager,BlockID blockId0,BlockID blockId1) {
    Transaction *tx1 = TransactionInit(fileManager,logManager,bufferManager);
    Transaction *tx2 = TransactionInit(fileManager,logManager,bufferManager);
    TransactionPin(tx1,blockId0);
    TransactionPin(tx2,blockId1);
    int pos = 0;
    for(int i=0;i<6;i++){
        TransactionSetInt(tx1,blockId0,pos,pos+100,true);
        TransactionSetInt(tx2,blockId1,pos,pos+100,true);
        pos+=sizeof(int);
    }
    TransactionSetString(tx1,blockId0,100,"uyw",true);
    TransactionSetString(tx2,blockId1,100,"xyz",true);
    BufferManagerFlushAll(bufferManager,3);
    BufferManagerFlushAll(bufferManager,4);
    TransactionCommit(tx1);
    TransactionCommit(tx2);
    testRecoveryPrintValue(fileManager,blockId0,blockId1,"After modification:");
    TransactionRollback(tx1);
    TransactionRollback(tx2);
    testRecoveryPrintValue(fileManager,blockId0,blockId1,"After Rollback:");

}

void testRecoveryRecover(FileManager*fileManager,LogManager*logManager,BufferManager*bufferManager,BlockID blockId0,BlockID blockId1) {
    Transaction *tx1 = TransactionInit(fileManager,logManager,bufferManager);
    TransactionRecover(tx1);
    testRecoveryPrintValue(fileManager,blockId0,blockId1,"After Recovery:");
}

void testRecovery(){
    time_t timep;
    struct tm *p;
    time(&timep);
    p = gmtime(&timep);
    char *fileName="1234567";
    char *LogName="test6.log";
//    char LogName[512];
//    sprintf(LogName,"Log_%d_%d_%d_%d_%d_%d",p->tm_year+1900,p->tm_mon+1,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec);
//    char fileName[512];
//    sprintf(fileName,"Text_%d_%d_%d_%d_%d_%d",p->tm_year+1900,p->tm_mon+1,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec);
    FileManager *fileManager = FileManagerInit("RecoveryTest",400);
    LogManager *logManager = LogManagerInit(fileManager,LogName);
    BufferManager *bufferManager = BufferManagerInit(fileManager,logManager,3);
    BlockID blockId0,blockId1;
    BlockID_Init(&blockId0,fileName,0);
    BlockID_Init(&blockId1,fileName,1);
    if(FileManagerLength(fileManager,fileName)==0){
        testRecoveryInitialize(fileManager,logManager,bufferManager,blockId0,blockId1);
        testRecoveryModify(fileManager,logManager,bufferManager,blockId0,blockId1);
        testPrintRecord(LogName);
    }else{
        testRecoveryRecover(fileManager,logManager,bufferManager,blockId0,blockId1);
    }
}
void testPrintRecord(char *LogName){
//    char *LogName="test3.log";
    FileManager *fileManager = FileManagerInit("RecoveryTest",400);
    LogManager *logManager = LogManagerInit(fileManager,LogName);
    int lastBlock = FileManagerLength(fileManager,LogName)-1;
    BlockID blockId;
    BlockID_Init(&blockId,LogName,lastBlock);
    Page *page = PageInit(fileManager->blockSize);
    FileManagerRead(fileManager,blockId,page);
    LogIterator*  logIterator = LogManager2LogManager(logManager);
    while (LogIteratorHasNext(logIterator)){
        ByteBufferData *byteBufferData = LogIteratorNext(logIterator);
        ByteBuffer * buffer =bufferAllocate(byteBufferData->bytesData->length);
        buffer->data = byteBufferData->bytesData->data;
        buffer->type = byteBufferData->bytesData->type;
        LogRecord *logRecord = LogRecordInit(buffer);
        char * val = logRecord->LogRecordToString(logRecord);
        printf("%s\n",val);
    }
}
void testTX(){
    time_t timep;
    struct tm *p;
    time(&timep);
    p = gmtime(&timep);
    char *fileName="1234567";
    char *LogName="test.log";
//    char LogName[512];
//    sprintf(LogName,"Log_%d_%d_%d_%d_%d_%d",p->tm_year+1900,p->tm_mon+1,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec);
//    char fileName[512];
//    sprintf(fileName,"Text_%d_%d_%d_%d_%d_%d",p->tm_year+1900,p->tm_mon+1,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec);
    FileManager *fileManager = FileManagerInit("TXTest",400);
    LogManager *logManager = LogManagerInit(fileManager,LogName);
    BufferManager *bufferManager = BufferManagerInit(fileManager,logManager,8);
    Transaction *tx1 = TransactionInit(fileManager,logManager,bufferManager);
    BlockID blockId;
    BlockID_Init(&blockId,"testfile",1);
//    TransactionPin(tx1,blockId);
//    TransactionSetInt(tx1,blockId,80,1,false);
//    TransactionSetString(tx1,blockId,40,"one",false);
//    TransactionCommit(tx1);
    Transaction *tx2 = TransactionInit(fileManager,logManager,bufferManager);
    TransactionPin(tx2,blockId);
    int IVal = TransactionGetInt(tx2,blockId,80);
    char * SVal = TransactionGetString(tx2,blockId,40);
    printf("%d %s\n",IVal,SVal);
    Transaction *tx3 = TransactionInit(fileManager,logManager,bufferManager);
    TransactionPin(tx3,blockId);
    TransactionSetInt(tx3,blockId,80,2,false);
    TransactionRollback(tx3);
    Transaction *tx4 = TransactionInit(fileManager,logManager,bufferManager);
    TransactionPin(tx4,blockId);
    int IVal1 = TransactionGetInt(tx4,blockId,80);
    char * SVal1 = TransactionGetString(tx4,blockId,40);
    printf("%d %s\n",IVal1,SVal1);
}
int getRandomInt(int max) {
    return rand() % (max + 1);
}

void TableScanTest() {
    time_t timep;
    struct tm *p;
    time(&timep);
    p = gmtime(&timep);
    char *fileName="1234567";
    char *LogName="test.log";
//    char *TableName = "T";
//    char LogName[512];
//    sprintf(LogName,"Log_%d_%d_%d_%d_%d_%d",p->tm_year+1900,p->tm_mon+1,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec);
//    char fileName[512];
//    sprintf(fileName,"Text_%d_%d_%d_%d_%d_%d",p->tm_year+1900,p->tm_mon+1,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec);
    char TableName [512];
    sprintf(TableName,"Table_%d_%d_%d_%d_%d_%d",p->tm_year+1900,p->tm_mon+1,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec);
    FileManager *fileManager = FileManagerInit("TableTest",400);
    LogManager *logManager = LogManagerInit(fileManager,LogName);
    BufferManager *bufferManager = BufferManagerInit(fileManager,logManager,8);
    Transaction *tx = TransactionInit(fileManager,logManager,bufferManager);
    Schema *schema = SchemaInit();
    SchemaAddIntField(schema,"A");
    SchemaAddStringField(schema,"B",9);
    Layout *layout = LayoutInit(schema,NULL,0);
    FieldNode* fieldNodeHead = layout->schema->fields;
    while(fieldNodeHead){
        int offset = LayoutOffset(layout,fieldNodeHead->fileName);
        printf("%s:%d\n",fieldNodeHead->fileName,offset);
        fieldNodeHead = fieldNodeHead->next;
    }
    printf("Filling the table with 50 random records.\n");
    TableScan *tableScan = TableScanInit(tx,TableName,layout);
    for(int i=0;i<50;i++){
        TableScanInsert(tableScan);
        int n = (int)getRandomInt(50);
        TableScanSetInt(tableScan,"A",n);
        char string[256];
        sprintf(string,"rec%d",n);
        TableScanSetString(tableScan,"B",string);
        printf("inserting into slot %s :%d +%s\n",RIDToString(TableScanGetRID(tableScan)),n,string);

    }
    printf("Deleting these records, whose A-values are less than 25.\n");
    int count = 0;
    TableScanBeforeFirst(tableScan);
    while (TableScanNext(tableScan)){
        int a = TableScanGetInt(tableScan,"A");
        char * b = TableScanGetString(tableScan,"B");
        if(a<25){
            count++;
            printf("slot %s:{%d,%s}\n",RIDToString(TableScanGetRID(tableScan)),a,b);
            TableScanDelete(tableScan);
        }
    }
    printf("%d, values under 10 were deleted.\n",count);
    printf("Here are the remaining records.\n");
    TableScanBeforeFirst(tableScan);
    while (TableScanNext(tableScan)){
        int a = TableScanGetInt(tableScan,"A");
        char * b = TableScanGetString(tableScan,"B");
        printf("slot %s:{%d,%s}\n",RIDToString(TableScanGetRID(tableScan)),a,b);
    }
    TableScanClose(tableScan);
    TransactionCommit(tx);
}
void TestStreamTokenizer() {
    // 输入测试字符串
    char input[] = "SELECT name, age FROM users WHERE_132 age > 25 AND status = '1active' ";

    // 初始化 StreamTokenizer
    StreamTokenizer *tokenizer = StreamTokenizerInit(input);
    if (!tokenizer) {
        printf("Failed to initialize StreamTokenizer.\n");
        return;
    }

    // 设置为小写模式（可选）
    StreamTokenizerLowerCaseMode(tokenizer);
    StreamTokenizerWordChars(tokenizer,'_','_');
    // 准备解析
    StreamTokenizerSearchReady(tokenizer);

    // 输出解析结果
    printf("Tokenization result for: \"%s\"\n", input);
    while (tokenizer->type != TOKEN_END_OF) {
        StreamTokenizerNext(tokenizer);

        // 根据类型输出结果
        switch (tokenizer->type) {
            case TOKEN_WORD:
                printf("TOKEN_WORD: %s\n", tokenizer->sValue);
                break;
            case TOKEN_INT_CONSTANT:
                printf("TOKEN_INT_CONSTANT: %d\n", tokenizer->iValue);
                break;
            case TOKEN_DELIM:
                printf("TOKEN_DELIM: %s\n", tokenizer->sValue);
                break;
            case TOKEN_UNKNOWN:
                printf("TOKEN_UNKNOWN: %s\n", tokenizer->sValue);
                break;
            case TOKEN_END_OF:
                printf("TOKEN_END_OF\n");
                break;
            default:
                printf("UNKNOWN TOKEN TYPE\n");
                break;
        }
    }

}
void testTrie(){
    Trie *root = TrieInit();

    // 插入单词
    TrieInsert(root, "hello");
    TrieInsert(root, "world");
    TrieInsert(root, "trie");
    TrieInsert(root, "data");
    TrieInsert(root, "structure");

    // 测试查找功能
    printf("Search for 'hello': %s\n", TrieSearchIn(root, "hello") ? "Found" : "Not Found");
    printf("Search for 'world': %s\n", TrieSearchIn(root, "world") ? "Found" : "Not Found");
    printf("Search for 'trie': %s\n", TrieSearchIn(root, "trie") ? "Found" : "Not Found");
    printf("Search for 'data': %s\n", TrieSearchIn(root, "data") ? "Found" : "Not Found");
    printf("Search for 'structure': %s\n", TrieSearchIn(root, "structure") ? "Found" : "Not Found");
    printf("Search for 'algorithm': %s\n", TrieSearchIn(root, "algorithm") ? "Found" : "Not Found");


}
void debugPrintToken(Lexer *lexer) {
    printf("Token Type: %d, Token Value: %s\n", lexer->tokenizer->type, lexer->tokenizer->sValue);
}

void testMetadataMgr(){
    time_t timep;
    struct tm *p;
    time(&timep);
    p = gmtime(&timep);
//    char *fileName="1234567";
//    char *LogName="test.log";
//    char *TableName = "T";
    char LogName[512];
    sprintf(LogName,"Log_%d_%d_%d_%d_%d_%d",p->tm_year+1900,p->tm_mon+1,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec);
    char fileName[512];
    sprintf(fileName,"Text_%d_%d_%d_%d_%d_%d",p->tm_year+1900,p->tm_mon+1,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec);
    char TableName [512];
    sprintf(TableName,"Table_%d_%d_%d_%d_%d_%d",p->tm_year+1900,p->tm_mon+1,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec);
    FileManager *fileManager = FileManagerInit("MetadataTest",400);
    LogManager *logManager = LogManagerInit(fileManager,LogName);
    BufferManager *bufferManager = BufferManagerInit(fileManager,logManager,8);
    Transaction *tx = TransactionInit(fileManager,logManager,bufferManager);
    MetadataMgr *metadataMgr = MetadataMgrInit(true,tx);
    Schema *schema = SchemaInit();
    SchemaAddIntField(schema,"A");
    SchemaAddStringField(schema,"B",9);
    MetadataMgrCreateTable(metadataMgr,"MyTable",schema,tx);
    Layout *layout = MetadataMgrGetLayout(metadataMgr,"MyTable",tx);
    int size = layout->SlotSize;
    Schema *schema2 = layout->schema;
    printf("MyTable has slot size%d\nIts fields are:\n",size);
    FieldNode* head = malloc(sizeof(FieldNode));
    head = schema2->fields;
    while(head){
        char * fldname = strdup(head->fileName);
        char type[128];
        if(SchemaType(schema2,fldname)==FILE_INFO_CODE_INTEGER){
            type[0]='i';
            type[1]='n';
            type[2]='t';
            type[3]='\0';
        }else{
            int len = SchemaType(schema2,fldname);
            sprintf(type,"varchar %d",len);
        }
        printf("%s",type);
        head = head->next;
    }

    TableScan*tableScan = TableScanInit(tx,"MyTable",layout);
    for(int i=0;i<50;i++){
        TableScanInsert(tableScan);
        int n = getRandomInt(50);
        char str[128];
        sprintf(str,"rec%d",n);
        TableScanSetInt(tableScan,"A",n);
        TableScanSetString(tableScan,"B",str);
    }
    StatInfo *statInfo = MetadataMgrGetStatInfo(metadataMgr,"MyTable",layout,tx);
    printf("B(MyTable) = %d\nR(MyTable) =%d\nV(MyTable,A) =%d\nV(MyTable,B) =%d\n",StatInfoBlocksAccessed(statInfo),StatInfoRecordsOutput(statInfo),
           StatInfoDistinctValues(statInfo,"A"), StatInfoDistinctValues(statInfo,"B"));

    char * viewdef = "select B from MyTable where A = 1";
    MetadataMgrCreateView(metadataMgr,"viewA",viewdef,tx);
    char *v = MetadataMgrGetViewDef(metadataMgr,"viewA",tx);
    printf("View def = %s",v);

    MetadataMgrCreateIndex(metadataMgr,"indexA","MyTable","A",tx);
    MetadataMgrCreateIndex(metadataMgr,"indexB","MyTable","B",tx);
    map_IndexInfo_t *idxmap = MetadataManagerGetIndexInfo(metadataMgr,"MyTable",tx);

    IndexInfo* indexInfo = map_get(idxmap,"A");
    printf("B(indexA) = %d\nR(indexA) = %d\nV(indexA,A) = %d\nV(indexA,B) = %d\n",IndexInfoBlocksAccessed(indexInfo),IndexInfoRecordsOutput(indexInfo),
           IndexInfoDistinctValues(indexInfo,"A"), IndexInfoDistinctValues(indexInfo,"B"));

    indexInfo = map_get(idxmap,"B");
    printf("B(indexA) = %d\nR(indexA) = %d\nV(indexA,A) = %d\nV(indexA,B) = %d\n",IndexInfoBlocksAccessed(indexInfo),IndexInfoRecordsOutput(indexInfo),
           IndexInfoDistinctValues(indexInfo,"A"), IndexInfoDistinctValues(indexInfo,"B"));
    TransactionCommit(tx);
}
void  tableTest(){
    time_t timep;
    struct tm *p;
    time(&timep);
    p = gmtime(&timep);
//    char *fileName="1234567";
//    char *LogName="test.log";
//    char *TableName = "T";
    char LogName[512];
    sprintf(LogName,"Log_%d_%d_%d_%d_%d_%d",p->tm_year+1900,p->tm_mon+1,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec);
    char fileName[512];
    sprintf(fileName,"Text_%d_%d_%d_%d_%d_%d",p->tm_year+1900,p->tm_mon+1,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec);
    char TableName [512];
    sprintf(TableName,"Table_%d_%d_%d_%d_%d_%d",p->tm_year+1900,p->tm_mon+1,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec);
    FileManager *fileManager = FileManagerInit("TableMgrTest",400);
    LogManager *logManager = LogManagerInit(fileManager,LogName);
    BufferManager *bufferManager = BufferManagerInit(fileManager,logManager,8);
    Transaction *tx = TransactionInit(fileManager,logManager,bufferManager);
    TableManager *tableManager = TableManagerInit(true,tx);
    Schema *schema = SchemaInit();

    SchemaAddIntField(schema,"A");
    SchemaAddStringField(schema,"B",9);
    TableManagerCreateTable(tableManager,"MyTable",schema,tx);

    Layout * layout = TableManagerGetLayout(tableManager,"MyTable",tx);
    int size = layout->SlotSize;
    Schema *schema2 = layout->schema;
    printf("size%d\n",size);
    printf("MyTable has slot size%d\nIts fields are:\n",size);
    FieldNode* head = malloc(sizeof(FieldNode));
    head = schema2->fields;
    while(head){
        char * fldname = strdup(head->fileName);
        char type[128];
        if(SchemaType(schema2,fldname)==FILE_INFO_CODE_INTEGER){
            type[0]='i';
            type[1]='n';
            type[2]='t';
            type[3]='\0';
        }else{
            int len = SchemaType(schema2,fldname);
            sprintf(type,"varchar %d",len);
        }
        printf("%s",type);
        head = head->next;
    }
}
void testLinkList(){
    List *list = ListInit(LIST_TYPE_CONSTANT);
    Constant *constant = ConstantInitInt(1);
    Constant *constant1 = ConstantInitInt(2);
    Constant *constant2 = ConstantInitString("123");
    ListAppend(list,constant,sizeof (Constant));
    ListAppend(list,constant1,sizeof (Constant));
    ListAppend(list,constant2,sizeof (Constant));
    ListPrint(list);
    printf("%d",ListContains(list,constant1));
}
const char* TokenTypeToString(TokenType type) {
    switch (type) {
        case TOKEN_WORD: return "TOKEN_WORD";
        case TOKEN_INT_CONSTANT: return "TOKEN_INT_CONSTANT";
        case TOKEN_DELIM: return "TOKEN_DELIM";
        case TOKEN_UNKNOWN: return "TOKEN_UNKNOWN";
        case TOKEN_NULL: return "TOKEN_NULL";
        case TOKEN_END_OF: return "TOKEN_END_OF";
        default: return "UNKNOWN_TYPE";
    }
}
void testStreamTokenizer() {
    // 测试字符串
    char *testInput = "CREATE TABLE students (id INT, name VARCHAR(50), age INT); INSERT INTO students VALUES (1, 'Alice', 20);";

    // 初始化 StreamTokenizer
    StreamTokenizer *tokenizer = StreamTokenizerInit(testInput);

    printf("Testing StreamTokenizer with input: \"%s\"\n", testInput);

    // 遍历所有的 Token
    while (tokenizer->type != TOKEN_END_OF) {
        StreamTokenizerNext(tokenizer);

        // 打印 Token 信息
        printf("Token Type: %s\n", TokenTypeToString(tokenizer->type));

        if (tokenizer->type == TOKEN_WORD || tokenizer->type == TOKEN_DELIM || tokenizer->type == TOKEN_UNKNOWN) {
            printf("Token Value: %s\n", tokenizer->sValue);
        } else if (tokenizer->type == TOKEN_INT_CONSTANT) {
            printf("Token Value: %d\n", tokenizer->iValue);
        }

        printf("-------------------\n");
    }

    // 释放资源
    map_deinit(tokenizer->keySet);
    free(tokenizer->data);
    free(tokenizer);
}
void testConcurrencyManager() {
    // 初始化 ConCurrencyManager
    ConCurrencyManager *conCurrencyManager = ConCurrencyManagerInit();

    // 创建一个 BlockID（假设文件名是 "testfile"，块 ID 是 1）
    BlockID blockId;
    BlockID_Init(&blockId, "testfile", 1);

    // 测试事务 1 获取共享锁（SLock）
    printf("Txn1 trying to get SLock on block 1...\n");
    ConCurrencyManagerSLock(conCurrencyManager, blockId);
    printf("Txn1 obtained SLock on block 1.\n");

    // 测试事务 2 获取共享锁（SLock），应该成功
    printf("Txn2 trying to get SLock on block 1...\n");
    ConCurrencyManagerSLock(conCurrencyManager, blockId);
    printf("Txn2 obtained SLock on block 1.\n");

    // 测试事务 1 升级到排他锁（XLock）
    printf("Txn1 trying to upgrade to XLock on block 1...\n");
    ConCurrencyManagerXLock(conCurrencyManager, blockId);
    printf("Txn1 upgraded to XLock on block 1.\n");

    // 测试事务 2 尝试获取排他锁（XLock），应该失败，因为事务 1 已经持有排他锁
    printf("Txn2 trying to get XLock on block 1...\n");
    ConCurrencyManagerXLock(conCurrencyManager, blockId);
    printf("Txn2 failed to get XLock on block 1.\n");

    // 测试释放锁
    printf("Txn1 releasing all locks...\n");
    ConCurrencyManagerRelease(conCurrencyManager);  // 释放所有锁
    printf("Txn1 released all locks.\n");

    // 清理内存
    free(conCurrencyManager);
}
void SimpleDBTest(){
    SimpleDB *simpleDB = SimpleDBInit("PlanTest", SIMPLE_DB_INIT_VAL, SIMPLE_DB_INIT_VAL);
    Transaction *transaction = SimpleDataNewTX(simpleDB);
    Planner* planner = simpleDB->planer;

    // CREATE TABLE
    char *cmd = "CREATE TABLE T1(A INT, B VARCHAR(13))";
    PlannerExecuteUpdate(planner, cmd, transaction);

    // INSERT random data
    int n = 5;
    printf("Inserting %d random records.\n", n);
    for (int i = 0; i < n; i++) {
//        int a = getRandomInt(50);
        int a = 10;
        char b[512];
        sprintf(b, "rec%d", a);
        char cmd1[512];
        sprintf(cmd1, "INSERT INTO T1(A, B) VALUES( %d, '%s')", a, b);
        PlannerExecuteUpdate(planner, cmd1, transaction);
    }

    // SELECT query
    char *cmd2 = "SELECT B FROM T1 WHERE A = 10";
    Plan *p = PlannerCreateQueryPlan(planner, cmd2, transaction);
    Scan *s = p->open(p);
    int count = 0;
    while (s->next(s)) {
        char *val = s->getString(s, "b");
        printf("%s\n", val);
        count++;
    }
    p = PlannerCreateQueryPlan(planner, cmd2, transaction);
    s = p->open(p);



    printf("%d\n",count);
    // UPDATE operation
    char *cmd3 = "UPDATE T1 SET B = 'newValue' WHERE A = 10";
    int num = PlannerExecuteUpdate(planner, cmd3, transaction);
    printf("Number of rows updated: %d\n", num);

    // Delete or further operations can be added here...
    // char*cmd4 = "DELETE FROM T1 WHERE A = 10";
    // PlannerExecuteUpdate(planner, cmd4, transaction);

    s->close(s);
    TransactionCommit(transaction);
}

void TestListFunctions() {
    // 测试 LIST_TYPE_STRING 类型
    printf("\nTesting LIST_TYPE_STRING:\n");
    List *stringList = ListInit(LIST_TYPE_STRING);

    ListAppend(stringList, "Hello", 0);
    ListAppend(stringList, "World", 0);
    ListAppend(stringList, "Test", 0);

    printf("Initial list: ");
    ListPrint(stringList);

    printf("Contains 'World': %s\n", ListContains(stringList, "World") ? "Yes" : "No");
    printf("Contains 'NotFound': %s\n", ListContains(stringList, "NotFound") ? "Yes" : "No");

    printf("Removing 'World'...\n");
    ListRemoveByValue(stringList, "World");
    printf("List after removal: ");
    ListPrint(stringList);

    printf("Removing index 0...\n");
    ListRemoveByIndex(stringList, 0);
    printf("List after index removal: ");
    ListPrint(stringList);

    ListFree(stringList);

    // 测试 LIST_TYPE_CONSTANT 类型
    printf("\nTesting LIST_TYPE_CONSTANT:\n");
    List *constantList = ListInit(LIST_TYPE_CONSTANT);

    Constant *c1 = ConstantInitInt(42);

    Constant *c2 = ConstantInitString("Constant String");

    ListAppend(constantList, c1, 0);
    ListAppend(constantList, c2, 0);

    printf("Initial list: ");
    ListPrint(constantList);

    printf("Removing constant value (42)...\n");
    ListRemoveByValue(constantList, c1);
    printf("List after removal: ");
    ListPrint(constantList);

    free(c1);
    free(c2->value.sval);
    free(c2);
    ListFree(constantList);

    // 测试 LIST_TYPE_TERM 类型
    printf("\nTesting LIST_TYPE_TERM:\n");
    List *termList = ListInit(LIST_TYPE_TERM);

//    Term *t1 = TermInit("x = 10");
//    Term *t2 = TermInit("y = 20");
//
//    ListAppend(termList, t1, 0);
//    ListAppend(termList, t2, 0);
//
//    printf("Initial list: ");
//    ListPrint(termList);
//
//    printf("Removing term 'x = 10'...\n");
//    ListRemoveByValue(termList, t1);
//    printf("List after removal: ");
//    ListPrint(termList);
//
//    ListFree(termList);
}
void testTerm() {
    // 创建常量
    Constant *const1 = ConstantInitInt(10);
    Constant *const2 = ConstantInitInt(20);
    Constant *const3 = ConstantInitInt(10); // 用于测试相等

    // 创建表达式
    Expression *expr1 = ExpressionInitConstant(const1);
    Expression *expr2 = ExpressionInitConstant(const2);
    Expression *expr3 = ExpressionInitConstant(const3);

    // 创建字段名表达式
    Expression *fieldExpr1 = ExpressionInitFieldName("field1");
    Expression *fieldExpr2 = ExpressionInitFieldName("field2");

    // 创建 Term
    Term *term1 = TermInit(expr1, expr2);
    Term *term2 = TermInit(expr1, expr3);
    Term *term3 = TermInit(fieldExpr1, expr1); // 用于字段名匹配
    Term *term4 = TermInit(fieldExpr1, fieldExpr2); // 两个字段名

    // 打印 Term
    printf("Term 1: %s\n", TermToString(term1));
    printf("Term 2: %s\n", TermToString(term2));
    printf("Term 3: %s\n", TermToString(term3));
    printf("Term 4: %s\n", TermToString(term4));

    // 测试 TermIsSatisfied
//    Scan *scan = ScanInit();
//    ScanSetValue(scan, "field1", 10);
//    ScanSetValue(scan, "field2", 20);
//
//    printf("Term 1 satisfied: %s\n", TermIsSatisfied(term1, scan) ? "true" : "false");
//    printf("Term 2 satisfied: %s\n", TermIsSatisfied(term2, scan) ? "true" : "false");
//    printf("Term 3 satisfied: %s\n", TermIsSatisfied(term3, scan) ? "true" : "false");
//
//    // 测试 TermReductionFactor
//    Plan *plan = PlanInit();
//    PlanSetDistinctValues(plan, "field1", 5);
//    PlanSetDistinctValues(plan, "field2", 10);
//
//    printf("Reduction factor for Term 4: %d\n", TermReductionFactor(term4, plan));
//
//    // 测试 TermEquatesWithConstant
//    Constant *equatedConst = TermEquatesWithConstant(term3, "field1");
//    if (equatedConst) {
//        printf("Term 3 equates field1 with constant: %s\n", ConstantToString(equatedConst));
//    } else {
//        printf("Term 3 does not equate field1 with any constant.\n");
//    }
//
//    // 测试 TermEquatesWithField
//    char *equatedField = TermEquatesWithField(term4, "field1");
//    if (equatedField) {
//        printf("Term 4 equates field1 with field: %s\n", equatedField);
//    } else {
//        printf("Term 4 does not equate field1 with any field.\n");
//    }
//
//    // 测试 TermAppliesTo
//    Schema *schema = SchemaInit();
//    SchemaAddField(schema, "field1");
//    SchemaAddField(schema, "field2");
//
//    printf("Term 4 applies to schema: %s\n", TermAppliesTo(term4, schema) ? "true" : "false");
//
//    // 清理资源
//    ConstantFree(const1);
//    ConstantFree(const2);
//    ConstantFree(const3);
//    ExpressionFree(expr1);
//    ExpressionFree(expr2);
//    ExpressionFree(expr3);
//    ExpressionFree(fieldExpr1);
//    ExpressionFree(fieldExpr2);
//
//    SchemaFree(schema);
}
void ScanTest1(){
    SimpleDB *simpleDB = SimpleDBInit("ScanTest1",SIMPLE_DB_INIT_VAL,SIMPLE_DB_INIT_VAL);
    Transaction *transaction = SimpleDataNewTX(simpleDB);

    Schema *schema1 = SchemaInit();
    SchemaAddIntField(schema1,"A");
    SchemaAddStringField(schema1,"B",9);

    Layout *layout = LayoutInit(schema1,NULL,-1);
    TableScan*tableScan = TableScanInit(transaction,"T",layout);
    Scan*s1 = ScanInit(tableScan,SCAN_TABLE_CODE);

    s1->beforeFirst(s1);
    int n = 200;
    printf("Inserting %d random records\n",n);
    for(int i=0;i<n;i++){
        s1->insert(s1);
        int k = getRandomInt(50);
        char b[512];
        sprintf(b,"rec%d",k);
        s1->setInt(s1,"A",k);
        s1->setString(s1,"B",b);
    }
    s1->close(s1);
    TableScan*tableScan2 = TableScanInit(transaction,"T",layout);
    Scan*s2 = ScanInit(tableScan2,SCAN_TABLE_CODE);
    Constant *c = ConstantInitInt(10);
    Term *term = TermInit(ExpressionInitFieldName("A"), ExpressionInitConstant(c));
    Predicate*predicate = PredicateInit(term);
    printf("%s\n",PredicateToString(predicate));

    SelectScan*selectScan = SelectScanInit(s2,predicate);
    Scan*s3 = ScanInit(selectScan,SCAN_SELECT_CODE);
    List*fields = ListInit(LIST_TYPE_STRING);
    ListAppend(fields,"B",0);
    ProjectScan*projectScan = ProjectScanInit(s3,fields);
    Scan*s4 = ScanInit(projectScan,SCAN_PROJECT_CODE);
    while (s4->next(s4)){
        printf("%s\n",s4->getString(s4,"B"));
    }
    s4->close(s4);
    TransactionCommit(transaction);
}
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
//int SimpleDBWork() {
//    printf("Welcome to the Database Management System (DBMS)\n");
//    printf("Type 'USE <database_name>' to select a database.\n");
//    printf("Type 'EXIT' to exit the system.\n");
//
//    char cmd[512];  // 用于存储用户输入的命令
//    SimpleDB *DBMS = NULL; // 初始化DBMS指针
//    TransactionManager* txnManager = NULL; // 初始化事务管理器
//    Planner *planner = NULL; // 初始化规划器指针
//    Transaction *transaction;
//    while (1) {
//        printf("SQL> ");
//        if (scanf(" %[^\n]", cmd) == EOF) {  // 读取一整行，支持带空格的 SQL 语句
//            break;
//        }
//
//        // 退出DBMS系统
//        if (strcmp(cmd, "EXIT") == 0 || strcmp(cmd, "exit") == 0) {
//            printf("Exiting the database...\n");
//            break;
//        }
//
//        // 选择数据库
//        if (strncmp(cmd, "USE ", 4) == 0) {
//            // 提取数据库名称
//            char database[512];
//            sscanf(cmd, "USE %s", database);
//
//            // 如果数据库已存在，则初始化DBMS
//
//            DBMS = SimpleDBInit(database, SIMPLE_DB_INIT_VAL, SIMPLE_DB_INIT_VAL);
//            if (DBMS == NULL) {
//                printf("Failed to initialize database: %s\n", database);
//                continue;
//            }
//            planner = DBMS->planer;
//            txnManager = DBMS->transactionManager;
//            transaction = SimpleDataNewTX(DBMS);
//            printf("Database '%s' has been selected successfully.\n", database);
//            continue;
//        }
//
//        // 在没有选择数据库之前，不执行其他操作
//        if (DBMS == NULL) {
//            printf("Please select a database first using 'USE <database_name>'.\n");
//            continue;
//        }
//
//        // 处理SQL命令
//        if (strncmp(cmd, "SELECT", 6) == 0 || strncmp(cmd, "select", 6) == 0) {
//            // SELECT 查询语句
//            Plan *plan = PlannerCreateQueryPlan(planner, cmd, NULL);  // 暂时不依赖事务
//            Scan *scan = plan->open(plan);
//
//            // 遍历查询结果
//            printf("Query results:\n");
//            SelectDataDisplay(plan, scan);
//
//            scan->close(scan);
//        } else if (strncmp(cmd, "COMMIT_ALL", 10) == 0 || strncmp(cmd, "commit_all", 10) == 0) {
//            // COMMIT 当前选中的事务
//            if (txnManager->mapTransactionData->base.nnodes == 0) {
//                printf("No transaction to commit.\n");
//                continue;
//            }
//            char txnName[512];
//            scanf("%s", txnName);
//            TransactionManagerCommit(txnManager);
//        } else if (strncmp(cmd, "COMMIT", 6) == 0 || strncmp(cmd, "commit", 6) == 0){
//            TransactionCommit(transaction);
//            transaction = SimpleDataNewTX(DBMS);
//        }else if(strncmp(cmd, "ROLLBACK", 8) == 0 || strncmp(cmd, "rollback", 8) == 0){
//            TransactionRollback(transaction);
//            transaction = SimpleDataNewTX(DBMS);
//        }
//        else if (strncmp(cmd, "ROLLBACK_ALL", 12) == 0 || strncmp(cmd, "rollback_all", 12) == 0) {
//            // ROLLBACK 当前选中的事务
//            if (txnManager->mapTransactionData->base.nnodes == 0) {
//                printf("No transaction to rollback.\n");
//                continue;
//            }
//            char txnName[512];
//            scanf("%s", txnName);
//            TransactionManagerRollback(txnManager);
//        } else if (strncmp(cmd, "NEW_TRANSACTION", 15) == 0) {
//            // 新建事务
//            char txnName[512];
//            printf("Enter transaction name: ");
//            scanf("%s", txnName);
//            transaction = SimpleDataNewTXName(DBMS,txnName);
//            printf("Transaction '%s' has been created.\n", txnName);
//        } else if (strncmp(cmd, "SHOW_TRANSACTIONS", 17) == 0) {
//            // 展示所有事务
//            printf("--- Active Transactions ---\n");
//            TransactionManagerDisplay(txnManager);
//        } else if (strncmp(cmd, "SWITCH_TRANSACTION", 18) == 0 || strncmp(cmd, "switch", 18) == 0) {
//            // 切换事务
//            char txnName[512];
//            printf("Enter transaction name to switch: ");
//            scanf("%s", txnName);
//            Transaction* txn = TransactionManagerSwitch(txnManager, txnName);
//            if (txn != NULL) {
//                printf("Switched to transaction '%s'.\n", txnName);
//            } else {
//                printf("Transaction '%s' not found.\n", txnName);
//            }
//        }else if(strncmp(cmd, "SHOW_NOW_TRANSACTIONS", 21) == 0) {
//            printf("transaction %d work now\n",transaction->txNum);
//        }
//        else {
//            // 其他 SQL 命令执行
//            int result = PlannerExecuteUpdate(planner, cmd, NULL);
//            printf("Command executed. Rows affected: %d\n", result);
//        }
//    }
//
//    // 关闭数据库之前，提交事务并释放资源
//    if (txnManager->mapTransactionData->base.nnodes > 0) {
//        TransactionManagerCommit(txnManager);  // 默认提交 txn1（可根据实际情况调整）
//    }
//
//    printf("Database closed.\n");
//    return 0;
//}
void testTransactionManager() {
    // 初始化事务管理器
    SimpleDB *simpleDB = SimpleDBInit("TransactionManagerTest", SIMPLE_DB_INIT_VAL, SIMPLE_DB_INIT_VAL);
    Transaction *txn1 = SimpleDataNewTX(simpleDB);
    Transaction *txn2 = SimpleDataNewTX(simpleDB);
    Transaction *txn3 = SimpleDataNewTX(simpleDB);
    TransactionManager* txnManager = TransactionManagerInit();

    TransactionManagerAdd(txnManager,"txn1", txn1);
    TransactionManagerAdd(txnManager,"txn2", txn2);
    TransactionManagerAdd(txnManager,"txn3",txn3);

    // 展示当前的事务信息
    printf("\n--- Initial Transaction List ---\n");
    TransactionManagerDisplay(txnManager);

    // 切换到事务2
    printf("\n--- Switching to Transaction txn2 ---\n");
    Transaction *transaction = TransactionManagerSwitch(txnManager, "txn2");
    TransactionCommit(transaction);
    // 展示当前的事务信息
    printf("\n--- Transaction List After Switching ---\n");
    TransactionManagerDisplay(txnManager);

    // 切换到事务3
    printf("\n--- Switching to Transaction txn3 ---\n");
    TransactionManagerSwitch(txnManager, "txn3");

    // 展示当前的事务信息
    printf("\n--- Transaction List After Switching ---\n");
    TransactionManagerDisplay(txnManager);

    // 切换回事务1
    printf("\n--- Switching to Transaction txn1 ---\n");
    TransactionManagerSwitch(txnManager, "txn1");

    // 展示当前的事务信息
    printf("\n--- Transaction List After Switching ---\n");
    TransactionManagerDisplay(txnManager);

    // 释放资源


    printf("\n--- Test Completed ---\n");
}
int main() {
//    BlockTest();
//testByteBuffer();
//    file_test();
   // TestFileManagerAndOpenFile();
   //testFileManager();
//    testByteBuffer();
//    printf("%llu",strlen("123"));
    //testPage();
    // LogCreateLogRecord("wqq",12);
    //testLog();
    //testSize();
    //testFileManagerAppendNew();
    //testBufferFile();
    //testBufferManager();
    //testBuffer();
    //mapTest();
    //testMapStruct();
    //test_SchemaAddField();
    //LayoutTest();
    //testLink();
    //mapStrTest();
    //testBlockIDConversion();
    //TestFunction();
    //testStruct();
   // testRecovery();
    //testTX();
    //TableScanTest();
    //TestStreamTokenizer();
    //testTrie();
    //testMetadataMgr();
    //tableTest();
    //testLexer();
    //testLinkList();
    //SimpleDBTest();
    //TestListFunctions();
    //testTerm();
    //ScanTest1();
    testConcurrencyManager();
    //SimpleDBWork();
    //testTransactionManager();
    return 0;
}
