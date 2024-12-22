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
#include <unistd.h> // for fsync

/**
 * 定义 FileSet 结构体，用于表示文件的一组相关联的数据和类型文件。
 */
typedef struct {
    FILE *File_Data; // 数据文件指针。
    FILE *File_Type; // 类型文件指针（可能用于存储元数据或特殊信息）。
} FileSet;

/**
 * 定义 OpenFile 结构体，用于链表中表示打开的文件节点。
 */
typedef struct node {
    char *fileName; // 文件名字符串。
    FileSet *file;  // 指向 FileSet 的指针，表示该文件的相关文件集。
    struct node *next; // 指向下一个 OpenFile 节点的指针。
} OpenFile;

/**
 * 定义 FileManager 结构体，负责管理数据库文件的操作。
 */
typedef struct {
    char *dbDirectoryName; // 数据库目录名称。
    DIR *dbDirectory;      // 数据库目录的句柄。
    int blockSize;         // 文件系统的块大小。
    bool isNew;            // 标记数据库是否为新创建。
    OpenFile *openFile;    // 指向 OpenFile 链表头节点的指针，用于管理已打开的文件。
} FileManager;

/**
 * 根据文件名从 OpenFile 链表中获取对应的 FileSet。
 *
 * @param head 指向 OpenFile 链表头节点的指针。
 * @param fileName 要查找的文件名。
 * @return 如果找到对应文件，则返回其 FileSet；否则返回 NULL。
 */
FileSet *OpenFileGet(OpenFile *head, const char *fileName);

/**
 * 初始化一个新的 OpenFile 链表头节点。
 *
 * @return 返回初始化后的 OpenFile 头节点指针。
 */
OpenFile *openFileInit();

/**
 * 将新的文件添加到 OpenFile 链表中。
 *
 * @param head 指向 OpenFile 链表头节点的指针。
 * @param fileName 要添加的文件名。
 */
void OpenFileAdd(OpenFile *head, const char *fileName);

/**
 * 获取指定文件的 FileSet，如果文件未打开则尝试打开它。
 *
 * @param fm 指向 FileManager 的指针。
 * @param fileName 要获取的文件名。
 * @return 返回对应的 FileSet；如果无法打开文件，则返回 NULL。
 */
FileSet *GetFile(FileManager *fm, const char *fileName);

/**
 * 初始化一个新的 FileManager 实例。
 *
 * @param dbDirectoryName 数据库目录的路径。
 * @param blockSize 文件系统中的块大小。
 * @return 返回初始化后的 FileManager 指针。
 */
FileManager *FileManagerInit(char *dbDirectoryName, int blockSize);

/**
 * 从磁盘读取指定 BlockID 的数据块内容到 Page 中。
 *
 * @param fm 指向 FileManager 的指针。
 * @param blockId 要读取的数据块 ID。
 * @param page 指向要填充的 Page 对象。
 */
void FileManagerRead(FileManager *fm, BlockID blockId, Page *page);

/**
 * 将指定 Page 的内容写入到磁盘上的对应 BlockID 数据块中。
 *
 * @param fm 指向 FileManager 的指针。
 * @param blockId 目标数据块 ID。
 * @param page 指向包含要写入的数据的 Page 对象。
 */
void FileManagerWrite(FileManager *fm, BlockID blockId, Page *page);

/**
 * 关闭 FileManager 管理的所有文件，并释放资源。
 *
 * @param fm 指向 FileManager 的指针。
 */
void FileManagerClose(FileManager *fm);

/**
 * 获取指定文件的长度（以块数计算）。
 *
 * @param fm 指向 FileManager 的指针。
 * @param filename 文件名。
 * @return 返回文件的长度（块数）。
 */
int FileManagerLength(FileManager *fm, char *filename);

/**
 * 在指定文件末尾追加一个新数据块，并返回新块的 BlockID。
 *
 * @param fm 指向 FileManager 的指针。
 * @param filename 文件名。
 * @return 返回新创建的数据块的 BlockID。
 */
BlockID FileManagerAppend(FileManager *fm, char *filename);

#endif //DBMS_C_FILEMANAGER_H