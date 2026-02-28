//
// Created by Lenovo on 2025/6/23.
//

#ifndef NEWDBMS_FILEMANAGER_H
#define NEWDBMS_FILEMANAGER_H
#include "BlockId.h"
#include "Page.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdbool.h>
#include <malloc.h>
#include <unistd.h>
#include "CMap.h"
#include "rwlock.h"
typedef struct FileManager{
    CString* dbDirectoryName; // 数据库目录名称。
    DIR *dbDirectory;      // 数据库目录的句柄。
    int blockSize;         // 文件系统的块大小。
    bool isNew;            // 标记数据库是否为新创建。
    CMap cMap;
//    RWLock *rwLock;
}FileManager;
/**
 * 初始化一个新的 FileManager 实例。
 *
 * @param dbDirectoryName 数据库目录的路径。
 * @param blockSize 文件系统中的块大小。
 * @return 返回初始化后的 FileManager 指针。
 */
FileManager *FileManagerInit(CString *dbDirectoryName, int blockSize);

/**
 * 从磁盘读取指定 BlockID 的数据块内容到 Page 中。
 *
 * @param fm 指向 FileManager 的指针。
 * @param blockId 要读取的数据块 ID。
 * @param page 指向要填充的 Page 对象。
 */
void FileManagerRead(FileManager *fm, BlockID *blockId, Page *page);

/**
 * 将指定 Page 的内容写入到磁盘上的对应 BlockID 数据块中。
 *
 * @param fm 指向 FileManager 的指针。
 * @param blockId 目标数据块 ID。
 * @param page 指向包含要写入的数据的 Page 对象。
 */
void FileManagerWrite(FileManager *fm, BlockID *blockId, Page *page);

/**
 * 关闭 FileManager 管理的所有文件，并释放资源。
 *
 * @param fm 指向 FileManager 的指针。
 */
void FileManagerDestroy(FileManager *fm);

/**
 * 获取指定文件的长度（以块数计算）。
 *
 * @param fm 指向 FileManager 的指针。
 * @param filename 文件名。
 * @return 返回文件的长度（块数）。
 */
int FileManagerLength(FileManager *fm, CString *filename);

/**
 * 在指定文件末尾追加一个新数据块，并返回新块的 BlockID。
 *
 * @param fm 指向 FileManager 的指针。
 * @param filename 文件名。
 * @return 返回新创建的数据块的 BlockID。
 */
BlockID* FileManagerAppend(FileManager *fm, CString *filename);

/**
 * 获取指定文件名对应的文件指针。
 *
 * @param fm 指向 FileManager 的指针。
 * @param cs 文件名的 CString 对象。
 * @return 返回对应的文件指针。
 */
FILE* FileManagerGetFile(FileManager *fm, CString *cs);
#endif //NEWDBMS_FILEMANAGER_H
