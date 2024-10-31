---
这里是一些记录，读者可以按照这个记录进行代码仿写和研究

本质上来说是对JavaDBMS的一个仿写,存在的一些问题也是对Java版本的代码思考

由于是在9.23开始写的,前面有一些会有一些遗漏

无论是什么!***拓展性***一定要好!!!!

## 9月
### 18号
主要是编写file(BlockId)、Head(ByteBuffer)和test文件
1. 创建并并编写BlockId文件
2. 创建Page文件,发现缺少一个缓冲池ByteBuffer开始编写ByteBuffer文件
3. 创建ByteBuffer文件(仿照 Java 中的 ByteBuffer)
4. 创建test测试BlockId、ByteBuffer文件
### 19号
主要是编写file(Page FileManager)  head(ByteBuffer)
1. 编写Page发现ByteBuffer缺少指定position 重新加上ByteBuffer指定position的api
2. 编写test文件测试Page功能
    1. 确定API命名为`{文件名}{函数}`在各个都是大写开头区分单词
    2. 不符合文件为:BlockId和ByteBuffer
3. 创建FileManager文件
### 20号
主要是编写file(FileManager)
1. 编写FileManager文件中openFile
    1. 一一对应选择选择链表,后期看一下是否需要换成其他高效寻找的方法
2. 重新学习文件操作
3. 测试openFile文件
### 21号
主要是编写file(FileManager)
1. 编写FileManager,并对其中的read和write进行深入研究
    1. read应该是将一个BlockId内容读到Page里面
    2. write应该是将一个Page内容写到BlockId里面
2. 编写test文件发现创建新文件有错误
### 22号
主要是编写file(FileManager) log(LogIterator)
1. 测试修改调试寻找到问题所在是头指针存储为空但是访问了头指针,修改后没有问题(暂时)
2. 创建Log文件编写LogIterator文件
3. 创建开始编写LogManager
4. 重新编写了Cmake文件
### 24号
主要是编写log文件

开始思考第一个问题:

``` javascript
  public synchronized int append(byte[] logrec) {
      int boundary = logpage.getInt(0);
      int recsize = logrec.length;
      int bytesneeded = recsize + Integer.BYTES;
      if (boundary - bytesneeded < Integer.BYTES) { 
      // the log record doesn't fit,
         flush();        // so move to the next block.
         currentblk = appendNewBlock();
         boundary = logpage.getInt(0);
      }
      int recpos = boundary - bytesneeded;

      logpage.setBytes(recpos, logrec);
      logpage.setInt(0, recpos); // the new boundary
      latestLSN += 1;
      return latestLSN;
   }
```
单独看着一个代码很容易理解,就是不够新加一个文件进行存储,但是在他的应用时,是这样写的:

```shell
    private static void createRecords(int start, int end) {
      System.out.print("Creating records: ");
      for (int i=start; i<=end; i++) {
         byte[] rec = createLogRecord("record"+i, i+100);
         int lsn = lm.append(rec);
         System.out.print(lsn + " ");
      }
      System.out.println();
   }

   // Create a log record having two values: a string and an integer.
   private static byte[] createLogRecord(String s, int n) {
      int spos = 0;
      int npos = spos + Page.maxLength(s.length());
      byte[] b = new byte[npos + Integer.BYTES];
      Page p = new Page(b);
      p.setString(spos, s);
      p.setInt(npos, n);
      
      return b;
   }
```
特别是createLogRecord中返回的b好像只是一个初始化的比特数组.   
经过查询后发现,原来是先把b放进入页面中,在向b中放数据,也就说我的Page也需要加上这一个东西,我也是明白用数组初始化的作用了

**该问题解决方案**:
还是利用前面的直接存储char*


吐槽一句***当文件多的时候GPT这一类就没啥用了,还是要靠自己***
完成Log文件 明天进行测试
### 25号
修改一下  
原先的ByteBuffer为:
```c
typedef struct{
    uint64_t size;      // 缓冲区总大小（字节数）
    uint8_t* data;      // 指向缓冲区数据的指针
    uint64_t position;  // 当前读写位置
    uint64_t limit;     // 缓冲区的限制（即读写的最大位置）
} ByteBuffer;
```
改为:
```c

typedef struct{
    uint64_t size;      // 缓冲区总大小（字节数）
    uint8_t* data;      // 指向缓冲区数据的指针
    uint64_t position;  // 当前读写位置
    uint64_t limit;     // 缓冲区的限制（即读写的最大位置）
    uint8_t* type;
} ByteBuffer;
```
## 26号
针对上述问题继续改正,首先加了一个枚举类型进行判断是什么类型,并加一个#define 将int重命名为BYTEBUFFER_STATUS
```c
#define BYTEBUFFER_STATUS int

typedef enum {
    BYTEBUFFER_INIT = 0,
    BYTEBUFFER_INT,
    BYTEBUFFER_SHORT,
    BYTEBUFFER_LONG,
    BYTEBUFFER_STRING,
    BYTEBUFFER_BYTE,
    BYTEBUFFER_BYTES,
} BYTEBUFFER_STATUS_CODE;
```
针对提取得到的信息修改,并加入新的初始化函数
```c
typedef struct{
    int length;
    char *data;
}StringData;
typedef struct{
    int length;
    uint8_t *data;
}BytesData;
typedef struct {
    BYTEBUFFER_STATUS StatusCode;
    int * intData;
    short *shortData;
    long long* longData;
    StringData *stringData;
    uint8_t *byteData;
    BytesData *bytesData;
}ByteBufferData;
ByteBufferData* ByteBufferDataInit();
```
这样一来易于**扩展类型**:只需要加入`BYTEBUFFER_STATUS_CODE`中,并在`ByteBufferData`中加入初始化函数即可
到9.45分之前Page还没有修改orz (已完成)

(感觉会在写好多数据结构啊!不会要手写红黑树这些吧?)

开始进行测试文件编写

发现了一个编译错误,原理应该是我把所有头文件放到ALLHead里面造成了循环引用的错误

!Page需要一个读取`Bytes`然后能解析这个`Bytes`源数据的函数(未来可能实现)

下面是对Log文件夹变现写

接下来更多内容应该等到十月假期了
# 29号
今天遇到一个奇怪的错误(segmentation fault),基本上之前之前也遇到过也是空指针,但是这次不一样:
原始代码如图所示:
```c
void PageInit(Page *p,int size){
    p->buffer  = bufferAllocate(size);
}
//Page.c文件
```
在运行如下代码时就会出现改问题:
```c
void testLog(){
    FileManager *fileManager= FileManagerInit("Logtest",1024);
    LogManager *logManager = LogManagerInit(fileManager,"Logtese2");
    LogCreateRecord(logManager,0,10);
}
```
我也怀疑是Page初始化有问题,但是又在下方代码没有问题:
```c
void testPage(){
    Page *p;
    p=PageInit(1024);
    PageSetString(&p, 0, "qwe");
    char *str = PageGetString(&p, 0);
    printf("页面内容: %s\n", str);
}
```
最后改成如下代码后没有报错:
```c
Page *PageInit(int size){
    Page *p = malloc(sizeof(Page));
    p->buffer  = bufferAllocate(size);
    return p;
}
```
具体原因确实不知道.
同时对LogIterator进行修改,原始代码如下:
```c
char *LogIteratorNext(LogIterator *logIterator){
    if(logIterator->currentPos == logIterator->fm->blockSize){
        logIterator->blockId.blockId--;
        LogIteratorMoveToBlock(logIterator);
    }
    char * rec = PageGetString(logIterator->page,logIterator->currentPos) ;
    int data;
    PageGetInt(logIterator->page,logIterator->currentPos,&data);
    logIterator->currentPos = data+1;
    return rec;
}
```
修改完为:
```c
ByteBufferData *LogIteratorNext(LogIterator *logIterator){
    if(logIterator->currentPos == logIterator->fm->blockSize){
        logIterator->blockId.blockId--;
        LogIteratorMoveToBlock(logIterator);
    }
    ByteBufferData* byteBufferData = PageGetBytes(logIterator->page,0);
    logIterator->currentPos += sizeof(int)+byteBufferData->bytesData->length;
    return byteBufferData;
}
```
在test.c文件加入如下函数进行测试:
```c
BytesData * LogCreateLogRecord(char*str,int n){
    int spos = 0,nops = spos+ PageMaxLength(strlen(str));
    BytesData *bytesData = malloc(sizeof(BytesData));
    bytesData->length = strlen(str)+2*sizeof (int)+1;
    ByteBuffer *byteBuffer = bufferAllocate(bytesData->length);
    bytesData->data = byteBuffer->data;
    bytesData->type = byteBuffer->type;
    Page *p = PageInitBytes(byteBuffer);
    PageSetString(p,0,str);
    PageSetInt(p,strlen(str)+sizeof(int),n);
    printf("%s",PageGetString(p,0));
    ByteBufferData *byteBufferData = malloc(sizeof(ByteBufferData));
    PageGetInt(p,strlen(str)+2,byteBufferData);
    return bytesData;

}
void  LogCreateRecord(LogManager *logManager,int start,int end){
    printf("Creating records:");
    char str[128]; ;
    for(int i=0;i<end;i++){
        sprintf(str,"record+%d",i);
        ByteBufferData *byteBufferData = ByteBufferDataInit();
        byteBufferData->bytesData = LogCreateLogRecord(str,i);
        int lsn = LogManagerAppend(logManager,byteBufferData->bytesData->data,byteBufferData->bytesData->length);
        printf("%d",lsn);
        printf("\n");
    }
    printf("\n");
}
void LogPrintRecord(LogManager *logManager,char*str){
    printf("%s",str);
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
        PageGetInt(p,0,byteBufferData1);
        PageGetInt(p,nos,byteBufferData1);
        char out[128];
        sprintf(out,"%s:%d",s,*byteBufferData1->intData);
        printf("%s",out);
    }
}
void testLog(){
    FileManager *fileManager= FileManagerInit("Logtest",1024);
    LogManager *logManager = LogManagerInit(fileManager,"Logtese2");
    LogCreateRecord(logManager,1,10);
    LogPrintRecord(logManager,"The log file now has these records:");
}
```
有问题但是太晚了睡觉了明天改报错为`-1073741819 (0xC0000005)`
### 30号
在检测时候发现一个大问题:在FileManager中读写为:
```c
void FileManagerRead(FileManager *fm,BlockID blockId,Page *page){
    FILE *f = GetFile(fm,blockId.fileName);
    if (f == NULL) {
        perror("File not found");
        return;
    }
    fseek(f,fm->blockSize*getBlockId(blockId),SEEK_SET);
    size_t bytesRead = fread(page->buffer->data, 1, fm->blockSize, f);
    if (bytesRead != fm->blockSize) {
        perror("Failed to read the complete block");
    }
}void FileManagerWrite(FileManager *fm, BlockID blockId, Page *page) {
    FILE *f = GetFile(fm, blockId.fileName);
    if (f == NULL) {
        perror("File not found");
        return;
    }
    fseek(f, fm->blockSize * getBlockId(blockId), SEEK_SET);
    size_t bytesWritten = fwrite(page->buffer->data, 1, fm->blockSize, f);
    if (bytesWritten != fm->blockSize) {
        perror("Failed to write the complete block");
    }
    fflush(f);
}
```
也就是说实际上只是写一个data,但实际上一个是写data和type(后面会改),现在看一下是不是这个问题导致的.
确定了是这个问题导致的,在这一个代码中`PageGetInt(logIterator->page,0,byteBufferData);` 读取的type为0,而在Buffer中GetInt为:`if(buffer->type[buffer->position]!=BYTEBUFFER_INT)
return 0;`也就是说我需要存储type的文件.现在这个也是用链表存储的,在考虑是否要改成Map提高效率 今天先不换了,以后有机会再换

先给FileManager加一个文件集;
```c
typedef struct {
    FILE *File_Data;
    FILE *File_Type;
}FileSet
```
对其都进行了修改,基本是返回值类型变为FileSet再取出来里面的File

对于这个重新修改测试函数,应该是问题不大
## 10月
### 1号
貌似发现问题所在：
```c
BlockID LogManagerAppendNewBlock(LogManager* logManager){
    BlockID  blockId = FileManagerAppend(logManager->fileManager,logManager->logFile);
    PageSetInt(logManager->logPage,0,logManager->fileManager->blockSize);
    FileManagerWrite(logManager->fileManager,logManager->currentBlockId,logManager->logPage);
    return blockId;
}
```
这是我写的版本,Java版本为:
```Java
   private BlockId appendNewBlock() {
      BlockId blk = fm.append(logfile);     
      logpage.setInt(0, fm.blockSize());
      fm.write(blk, logpage);
      return blk;
   }
```
然后在Page中也发现了错误
```c
int PageGetInt(Page *p, int position, ByteBufferData *output){
    return bufferGetInt(p->buffer,output);
}
```
改为(?竟然现在才发现这个错误)
```c
int PageGetInt(Page *p, int position, ByteBufferData *output){
    return bufferGetIntPosition(p->buffer,position,output);
}
```
### 5号
发现的bug:在(ByteBuffer)setBytes中有如下的代码
```c
uint64_t bufferPutBytesPosition(ByteBuffer* buffer, uint64_t position, uint8_t* data,uint8_t *type, uint64_t size) {
    // 检查缓冲区是否有效以及写入是否越界
    if (buffer == NULL || data == NULL || position + size > buffer->limit) {
        return 0;
    }
    memcpy(buffer->data + position, data, size);
    memcpy(buffer->type + position, type, size);
    return size;
}
```
这是修改完后版本,修改前时只放入了第一个的位置,不妥.
### 6号
在FileManager中写的时候加入了fflush 也就是立即写入文件
```c
void FileManagerWrite(FileManager *fm, BlockID blockId, Page *page) {
    FileSet *fileSet = GetFile(fm, blockId.fileName);
    FILE *f = fileSet->File_Data;
    FILE *fType = fileSet->File_Type;
    if (f == NULL) {
        perror("File not found");
        return;
    }
    fseek(f, fm->blockSize * getBlockId(blockId), SEEK_SET);
    fseek(fType, fm->blockSize * getBlockId(blockId), SEEK_SET);
    size_t bytesWritten = fwrite(page->buffer->data, 1, fm->blockSize, f);
    fwrite(page->buffer->type, 1, fm->blockSize, fType);
    if (bytesWritten != fm->blockSize) {
        perror("Failed to write the complete block");
    }
    fflush(f);
    fflush(fType);
}
```
至此,写到一个块内没有问题,但写入多个快有问题
```c
int LogManagerAppend(LogManager *logManager, uint8_t *data, uint8_t *type,int size) {
    int boundary;
    ByteBufferData *byteBufferData = ByteBufferDataInit();
    PageGetInt(logManager->logPage, 0, byteBufferData);
    boundary = *byteBufferData->intData;
//    printf("Current boundary: %d\n", boundary); // 调试信息

    int byteNeeded = size + sizeof(int);
    
    if (boundary - byteNeeded < sizeof (int)) { // the log record doesn't fit
//        printf("Boundary too small, flushing and appending new block\n");
        LogManagerFlush(logManager);
        logManager->currentBlockId = LogManagerAppendNewBlock(logManager);
        PageGetInt(logManager->logPage, 0, byteBufferData);
        boundary = *byteBufferData->intData;
//        printf("New boundary after flush: %d\n", boundary); // 调试信息
    }
    int recPos = boundary - byteNeeded;
    PageSetBytes(logManager->logPage, recPos, data,type, size);
    PageSetInt(logManager->logPage, 0, recPos); // the new boundary
    logManager->latestLSN += 1;
//    printf("Updated boundary: %d\n", recPos); // 调试信息
    return logManager->latestLSN;
}
```
这是现在版本,发现`if (boundary - byteNeeded < sizeof (int))`这里怎么样也进不去 后来研究发现强转化为(int)
### 8号
竟然被如此傻逼的问题卡了这么久！
用fwrite把数据写入文件，再用fread读取，发现后半部分的数据可能是错的。
原因:`原本要写入文件的数据中，有0x0A，如果用的是文本模式打开的文件流，在windows下0x0A会被转换为0x0D和0x0A`
修改方案:
```c
void OpenFileAdd(OpenFile *head,const char *fileName){
    OpenFile *q = head;
    while(q->next){
        q = q->next;
    }
    OpenFile *newOpenFile = openFileInit();
    FILE *file = fopen(fileName,"rb+");
    char str[512];
    sprintf(str,"%s_type",fileName);
    FILE *file_type = fopen(str,"rb+");
    newOpenFile->file->File_Data=file;
    newOpenFile->file->File_Type=file_type;
    newOpenFile->fileName=fileName;
    newOpenFile->next=NULL;
    q->next = newOpenFile;
}
FileSet *GetFile(FileManager *fm, const char *fileName) {
    FileSet *file = OpenFileGet(fm->openFile, fileName);
    if (file== NULL) {
        // 构建文件路径
        char filePath[512];
        snprintf(filePath, sizeof(filePath), "%s/%s", fm->dbDirectoryName, fileName);
        // 先尝试以 "r+" 模式打开文件，如果文件不存在，则创建新文件
        FILE *newFile = fopen(filePath, "rb+");  // 以读写模式打开，文件不存在时不创建
        if (newFile == NULL) {
            // 文件不存在时，以 "w+" 模式创建文件
            newFile = fopen(filePath, "wb+");
            if (newFile == NULL) {
                perror("Failed to open or create file");
                return NULL;
            }
        }
        char filePathType[512];
        snprintf(filePathType, sizeof(filePath), "%s/%s_type", fm->dbDirectoryName, fileName);
        FILE *newFileType = fopen(filePathType, "rb+");  // 以读写模式打开，文件不存在时不创建
        if (newFileType == NULL) {
            // 文件不存在时，以 "w+" 模式创建文件
            newFileType = fopen(filePathType, "wb+");
            if (newFileType == NULL) {
                perror("Failed to open or create file");
                return NULL;
            }
        }
        // 将新打开的文件添加到 openFile 链表中
        OpenFileAdd(fm->openFile, fileName);
        OpenFile *current = fm->openFile;
        while (current->next != NULL) {
            current = current->next;
        }
        FileSet *fileSet = malloc(sizeof(FileSet));
        fileSet->File_Data = newFile;
        fileSet->File_Type = newFileType;
        current->file->File_Data = newFile;
        current->file->File_Type = newFileType;
        current->fileName = strdup(fileName);  // 复制文件名
        return fileSet;  // 返回文件指针
    }
    return file;  // 如果文件已经打开，直接返回
}
```
都加上了b进行读取.至此 Log修改完成! 终于写完了Log 对这几个功能修改更完善,了解更深了
# 21号
金工实习结束了 我又来继续更新了

Buffer文件测试没有问题

开始写record文件夹 在此之前要实现Map(head) 这里采用了[GitHub](https://github.com/rxi/map)上面的一个map(哈希版本) 。