//
// Created by yaohuayuan on 2024/9/19.
//

#include "FileManager.h"
FileSet * OpenFileGet(OpenFile *head,const char* filename){
    OpenFile *q = head->next;
    while(q){
        if (strcmp(q->fileName, filename) == 0) {
            return q->file;
        }
        q=q->next;
    }
    return NULL;
}
OpenFile *openFileInit() {
    OpenFile *head = malloc(sizeof(OpenFile));
    if (!head) {
        perror("Failed to allocate memory for OpenFile");
        exit(EXIT_FAILURE);
    }
    head->file = malloc(sizeof (FileSet));
    head->file->File_Data=NULL;
    head->file->File_Type=NULL;
    head->fileName = NULL;
    head->next = NULL;
    return head;
}
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
    newOpenFile->fileName= strdup(fileName);
    newOpenFile->next=NULL;
    q->next = newOpenFile;
}
FileManager * FileManagerInit(char * dbDirectoryName,int blockSize){
    FileManager *fm = malloc(sizeof (FileManager));
    fm->dbDirectoryName=dbDirectoryName;
    fm->dbDirectory = opendir(dbDirectoryName);
    fm->blockSize = blockSize;
    fm->openFile = openFileInit();
    if(fm->dbDirectory==NULL){
        fm->isNew=true;
    }else{
        fm->isNew=false;
    }
    if(fm->isNew==true){
        mkdir(dbDirectoryName);
        fm->dbDirectory = opendir(dbDirectoryName);
    }

    struct dirent *ptr;
    while ((ptr = readdir(fm->dbDirectory)) != NULL) {
//        printf("d_name : %s\n", ptr->d_name);
        if (strncmp(ptr->d_name, "temp", 4) == 0) {
            char filePath[512];
            snprintf(filePath, sizeof(filePath), "%s/%s", dbDirectoryName, ptr->d_name);
//            if (remove(filePath) == 0) {
//                printf("Deleted temp file: %s\n", filePath);
//            } else {
//                perror("Failed to delete temp file");
//            }
        }
    }
    return fm;
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
void FileManagerRead(FileManager *fm,BlockID blockId,Page *page){
    FileSet *fileSet = GetFile(fm,blockId.fileName);
    FILE *f = fileSet->File_Data;
    FILE *fType = fileSet->File_Type;
    if (f == NULL) {
        perror("File not found");
        return;
    }
    fseek(f,fm->blockSize*getBlockId(blockId),SEEK_SET);
    fseek(fType,fm->blockSize*getBlockId(blockId),SEEK_SET);
    size_t bytesRead = fread(page->buffer->data, 1, fm->blockSize, f);
    size_t bytesReadType = fread(page->buffer->type, 1, fm->blockSize, fType);
    if (bytesRead != fm->blockSize || bytesReadType!=fm->blockSize) {
        perror("Error! FileManager 132. Failed to read the complete block");
    }
}
void FileManagerWrite(FileManager *fm, BlockID blockId, Page *page) {
    FileSet *fileSet = GetFile(fm, blockId.fileName);
    FILE *f = fileSet->File_Data;
    FILE *fType = fileSet->File_Type;

    if (f == NULL || fType == NULL) {
        perror("File not found");
        return;
    }

    fseek(f, fm->blockSize * getBlockId(blockId), SEEK_SET);
    fseek(fType, fm->blockSize * getBlockId(blockId), SEEK_SET);

    size_t bytesWritten = fwrite(page->buffer->data, 1, fm->blockSize, f);
    size_t bytesWrittenType = fwrite(page->buffer->type, 1, fm->blockSize, fType);

    if (bytesWritten != fm->blockSize || bytesWrittenType != fm->blockSize) {
        perror("Failed to write the complete block");
    }

    // 强制刷新文件缓冲区，确保立即保存数据
    fflush(f);
    fflush(fType);

    // 可选：如果希望确保数据立即写入到磁盘，可以在写入完成后关闭文件
    // fclose(f);
    // fclose(fType);

}
void FileManagerClose(FileManager *fm) {
    OpenFile *current = fm->openFile;
    while (current != NULL) {
        if (current->file != NULL) {
            if(current->file->File_Data!=NULL){
                fclose(current->file->File_Data);
            }
            if(current->file->File_Type!=NULL){
                fclose(current->file->File_Type);
            }

        }
        OpenFile *next = current->next;
        free(current->fileName);
        free(current);
        current = next;
    }
    closedir(fm->dbDirectory);
    free(fm);
}
int FileManagerLength(FileManager *fm,char *filename){
    FileSet *fileSet = GetFile(fm,filename);
    FILE *f = fileSet->File_Data;
    fseek(f, 0, SEEK_END);
//    int data = (int)(ftell(f)/fm->blockSize);
//    printf("%d", data);
    return (int)(ftell(f)/fm->blockSize);
}
BlockID FileManagerAppend(FileManager *fm,char *filename){
    int newBlkNum = FileManagerLength(fm,filename);
    BlockID blk;
    char *buffer = malloc(fm->blockSize);  // 创建一个新块的字节数组
    BlockID_Init(&blk,filename,newBlkNum);
    FileSet *fileset = GetFile(fm,filename);
    FILE *f = fileset->File_Data;
    FILE *fType = fileset->File_Type;
    fseek(f, (long)blk.blockId * fm->blockSize, SEEK_SET);  // 设置写入位置
    size_t bytesWritten = fwrite(buffer, 1, fm->blockSize, f);  // 写入数据
    fflush(f);  // 确保数据写入
    fseek(fType, (long)blk.blockId * fm->blockSize, SEEK_SET);  // 设置写入位置
    size_t bytesWrittenType =fwrite(buffer, 1, fm->blockSize, fType);  // 写入数据
    fflush(fType);
    if (bytesWritten != fm->blockSize || bytesWrittenType!=fm->blockSize) {
        perror("Failed to append block");
    }
    free(buffer);  // 释放内存


    return blk;  // 返回新块的标识符
}
