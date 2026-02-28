//
// Created by Lenovo on 2025/6/23.
//

#include "FileManager.h"
FileManager * FileManagerInit(CString * dbDirectoryName,int blockSize){
    if(dbDirectoryName==NULL||blockSize<=0){
        return NULL;
    }
    FileManager *fm = malloc(sizeof (FileManager));
    fm->dbDirectoryName= CStringCreateFromCString(dbDirectoryName) ;
    fm->dbDirectory = opendir(CStringGetPtr(dbDirectoryName));
    fm->blockSize = blockSize;
    CMapInit(&fm->cMap,sizeof(CString),sizeof(FILE*), CStringCompareVoid, CStringDestroyVoid,NULL,
             CStringCreateFromCStringVoid,NULL);
    if(fm->dbDirectory==NULL){
        fm->isNew=true;
    }else{
        fm->isNew=false;
    }
    if(fm->isNew==true){
        if (mkdir(CStringGetPtr(dbDirectoryName)) != 0 && errno != EEXIST) {
            free(fm);
            return NULL;
        }
        fm->dbDirectory = opendir(CStringGetPtr(dbDirectoryName));
    }
    struct dirent *ptr;
    while ((ptr = readdir(fm->dbDirectory)) != NULL) {
        if (strncmp(ptr->d_name, "temp", 4) == 0) {
            char filePath[512];
            snprintf(filePath, sizeof(filePath), "%s/%s",CStringGetPtr(dbDirectoryName), ptr->d_name);
        }
    }
//    RwLockInit(fm->rwLock);
    return fm;
}
static void build_file_path(FileManager*fm,CString*filename,char*out,size_t cap){
    snprintf(out, cap, "%s/%s",
             CStringGetPtr(fm->dbDirectoryName),
             CStringGetPtr(filename));
}
FILE* FileManagerGetFile(FileManager *fm, CString *cs) {
    FILE** f = CMapFind(&fm->cMap, cs);
    if (f != NULL) return *f;

    char path[512];
    build_file_path(fm,cs,path,512);

    // 使用 "ab+" 或 "rb+" 配合检测
    FILE *file = fopen(path, "rb+");
    if (!file) {
        file = fopen(path, "wb+"); // 不存在则创建
    }

    if (file) {
        // 注意：CMap 的 Insert 逻辑应确保能自动管理 key/value 的内存
        CMapInsert(&fm->cMap, cs, &file);
    }
    return file;
}
void FileManagerRead(FileManager *fm,BlockID *blockId,Page *page){
    FILE *f= FileManagerGetFile(fm,blockId->fileName);
//    RwLockReadLock(fm->rwLock);
    if (f == NULL) {
        perror("File not found");
        return;
    }
    long target_offset = (long)fm->blockSize * BlockIDGetBlockID(blockId);
    fseek(f, 0, SEEK_END);
    long file_size = ftell(f);
    if (target_offset >= file_size) {
        memset(page->buffer->data, 0, fm->blockSize);
        return;
    }

    fseek(f,fm->blockSize*BlockIDGetBlockID(blockId),SEEK_SET);
    size_t bytesRead = fread(page->buffer->data, 1, fm->blockSize, f);
    if (bytesRead < fm->blockSize) {
        memset(page->buffer->data + bytesRead, 0,
               fm->blockSize - bytesRead);
    }

//    RwLockUnlockRead(fm->rwLock);
//    CMapUpdate(&fm->cMap,blockId->fileName,f);
}
void FileManagerWrite(FileManager *fm,BlockID *blockId,Page *page){
    FILE *f= FileManagerGetFile(fm,blockId->fileName);
    if (f == NULL) {
        perror("File not found");
        return;
    }
    fseek(f,fm->blockSize*BlockIDGetBlockID(blockId),SEEK_SET);
    size_t bytesRead = fwrite(page->buffer->data, 1, fm->blockSize, f);
    if (bytesRead != fm->blockSize ) {
        perror("Error! . Failed to read the complete block");
    }
    fflush(f);
//    CMapUpdate(&fm->cMap,blockId->fileName,f);
}
int FileManagerLength(FileManager *fm,CString *filename){
    FILE *f= FileManagerGetFile(fm, filename);
    fseek(f, 0, SEEK_END);
    return (int)(ftell(f)/fm->blockSize);
}
BlockID* FileManagerAppend(FileManager *fm,CString *filename){
    int newBlkNum = FileManagerLength(fm,filename);
    char *buffer = calloc(1, fm->blockSize);
    BlockID *blk=BlockIDInit(CStringCreateFromCString(filename),newBlkNum);
    FILE *f= FileManagerGetFile(fm, filename);
    fseek(f, (long)blk->BlockID * fm->blockSize, SEEK_SET);  // 设置写入位置
    size_t bytesWritten = fwrite(buffer, 1, fm->blockSize, f);  // 写入数据
    fflush(f);  // 确保数据写入
    if (bytesWritten != fm->blockSize ) {
        perror("Failed to append block");
    }
    free(buffer);  // 释放内存


    return blk;  // 返回新块的标识符
}
void FileManagerDestroy(FileManager *fm){
    CMapIterator end =  CMapIteratorEnd(&fm->cMap);
    for(CMapIterator iter = CMapIteratorBegin(&fm->cMap); !CMapIteratorEqual(&iter,&end); CMapIteratorNext(&iter)){
        FILE** f=CMapIteratorValue(&iter);
        fclose(*f);
    }
    CMapDestroy(&fm->cMap);
    if (fm->dbDirectory){
        closedir(fm->dbDirectory);
    }
}