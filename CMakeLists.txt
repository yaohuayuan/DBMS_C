# 设置最低要求的 CMake 版本
cmake_minimum_required(VERSION 3.24)

# 项目名称以及版本号
project(DBMS_C VERSION 1.0.0 LANGUAGES C)

# 设置 C 标准
set(CMAKE_C_STANDARD 11)

# 可执行文件输出路径
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# 将源文件分组，避免重复列出
set(FILE_SOURCES
        file/BlockId.c
        file/Page.c
        file/FileManager.c
        )

set(HEAD_SOURCES
        head/ByteBuffer.c
        head/Error.c
        head/map.c
        head/List.c head/List.h head/Trie.c head/Trie.h head/StreamTokenizer.c head/StreamTokenizer.h)

set(TEST_SOURCES
        test/test.c
        )

set(LOG_SOURCES
        log/LogIterator.c
        log/LogManager.c
        )

set(BUFFER_SOURCES
        buffer/Buffer.c
        buffer/BufferManager.c
        )

set(RECORD_SOURCES
        record/Schema.c
        record/Layout.c
        record/RID.c
        record/RecordPage.c record/RecordPage.h record/TableScan.c record/TableScan.h)

set(TX_SOURCES
        tx/BufferList.c
        tx/concurrency/LockTable.c
        tx/concurrency/ConcurrencyManager.c
        tx/recovery/LogRecord.c
        tx/recovery/RecoveryManager.c tx/recovery/RecoveryManager.h tx/Transaction.c tx/Transaction.h)

set(QUERY_SOURCE


        query/Constant.c query/Constant.h query/Expression.c query/Expression.h query/Scan.c query/Scan.h query/UpdateScan.c query/UpdateScan.h query/ProjectScan.c query/ProjectScan.h query/ProductScan.c query/ProductScan.h)
set(METADATA_SOURCE
        metadata/TableManager.c metadata/TableManager.h
        metadata/ViewManager.c metadata/ViewManager.h metadata/StatInfo.c metadata/StatInfo.h metadata/StartManager.c metadata/StartManager.h)
set(INDEX_SOURCE

)
set(PLAN_SOURCE

)
set(PARSE_SOURCE

        parse/Lexer.c parse/Lexer.h)
# 将所有源文件放入一个变量中，方便管理
set(SOURCE_FILES
        ${FILE_SOURCES}
        ${HEAD_SOURCES}
        ${TEST_SOURCES}
        ${LOG_SOURCES}
        ${BUFFER_SOURCES}
        ${RECORD_SOURCES}
        ${TX_SOURCES}
        ${METADATA_SOURCE}
#        ${QUERY_SOURCE}
        ${PLAN_SOURCE}
        ${PARSE_SOURCE}
      )

# 定义可执行文件
add_executable(DBMS_C_EXEC ${SOURCE_FILES})

# 指定头文件目录
target_include_directories(DBMS_C_EXEC PRIVATE
        file
        head
        test
        log
        buffer
        record
        tx
        query
        metadata
        plan
        index
        parse
        )
