cmake_minimum_required(VERSION 3.24)
project(DBMS_C C)

# 设置 C 标准
set(CMAKE_C_STANDARD 11)

# 包含头文件目录
include_directories(
        file
        head
        test
        log
        buffer
        record
)

## 添加可执行文件以及其相关源文件
add_executable(DBMS_C
        file/BlockId.c
        file/Page.c
        file/FileManager.c
        head/ByteBuffer.c
        test/test.c
        log/LogIterator.c log/LogIterator.h log/LogManager.c log/LogManager.h buffer/Buffer.c buffer/Buffer.h buffer/BufferManager.c buffer/BufferManager.h head/Error.c head/Error.h record/Schema.c record/Schema.h head/map.c head/map.h)

## 添加头文件列表，确保 CMake 能找到它们
#target_include_directories(DBMS_C PUBLIC
#        file/BlockId.h
#        file/Page.h
#        file/FileManager.h
#        head/ByteBuffer.h
#        test/test.h
#        head/AllHead.h
#        )

