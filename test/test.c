#include "test.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "map.h"
#include "Schema.h"
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
    LogCreateRecord(logManager,1,500);
    LogManagerFlushLSN(logManager,500);
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
    return fileName;
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
    testStruct();
    return 0;
}
