//
// Created by yaohuayuan on 2024/9/19.
//

#ifndef DBMS_C_FILEMANAGER_H
#define DBMS_C_FILEMANAGER_H
#include "BlockId.h"
#include "Page.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdbool.h>
#include <malloc.h>
typedef struct {
    FILE *File_Data;
    FILE *File_Type;
}FileSet;
typedef struct node{
    char *fileName;
    FileSet * file;
    struct node* next;
}OpenFile;
typedef struct{
    char * dbDirectoryName;
    DIR * dbDirectory;
    int blockSize;
    bool isNew;
    OpenFile *openFile;
}FileManager;
FileSet * OpenFileGet(OpenFile *head,const char* fileName);
OpenFile *openFileInit();
void OpenFileAdd(OpenFile *head,const char *fileName);
FileSet * GetFile(FileManager *fm, const char*fileName);
FileManager * FileManagerInit(char * dbDirectoryName,int blockSize);
void FileManagerRead(FileManager *fm,BlockID blockId,Page *page);
void FileManagerWrite(FileManager *fm,BlockID blockId,Page *page);
void FileManagerClose(FileManager *fm);
int FileManagerLength(FileManager *fm,char *filename);
BlockID FileManagerAppend(FileManager *fm,char *filename);
#endif //DBMS_C_FILEMANAGER_H
