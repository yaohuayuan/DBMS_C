# 最低 CMake 版本
cmake_minimum_required(VERSION 3.15)

# 项目名称
project(NewDBMS VERSION 1.0.0 LANGUAGES C)

# C 语言标准
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Avoid CMake's Windows-GNU Unix Makefiles temporary objects.a link archive.
# In this environment those intermediate archives can be locked briefly and
# make relinks fail even though compilation succeeds.
if(WIN32 AND CMAKE_C_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_C_LINK_EXECUTABLE
            "<CMAKE_C_COMPILER> <FLAGS> <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> -Wl,--out-implib,<TARGET_IMPLIB> ${CMAKE_GNULD_IMAGE_VERSION} <LINK_LIBRARIES>")
endif()

# 可执行文件输出目录，统一输出到 bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin)

# ========== CMocka 配置 ==========
set(CMOCKA_ROOT ${PROJECT_SOURCE_DIR}/external/cmocka-1.1.0)
set(HAVE_CMOCKA FALSE)

# 检查头文件
if(EXISTS ${CMOCKA_ROOT}/include/cmocka.h)
    include_directories(${CMOCKA_ROOT}/include)
    message(STATUS "Found CMocka headers: ${CMOCKA_ROOT}/include")
else()
    message(WARNING "CMocka headers not found at ${CMOCKA_ROOT}/include")
endif()

# 设置 CMocka 库和 DLL 路径
set(CMOCKA_LIBRARY ${CMOCKA_ROOT}/lib/libcmocka.dll.a)
set(CMOCKA_DLL ${CMOCKA_ROOT}/bin/cmocka.dll)

# 验证 CMocka 文件是否存在
if(EXISTS ${CMOCKA_LIBRARY} AND EXISTS ${CMOCKA_DLL})
    set(HAVE_CMOCKA TRUE)
    message(STATUS "Found CMocka library: ${CMOCKA_LIBRARY}")
    message(STATUS "Found CMocka DLL: ${CMOCKA_DLL}")
else()
    set(HAVE_CMOCKA FALSE)
    if(NOT EXISTS ${CMOCKA_LIBRARY})
        message(WARNING "CMocka library (.dll.a) not found at ${CMOCKA_LIBRARY}")
    endif()
    if(NOT EXISTS ${CMOCKA_DLL})
        message(WARNING "CMocka DLL not found at ${CMOCKA_DLL}")
    endif()
endif()

# ========== 目录设置 ==========
set(LIB_DIR ${PROJECT_SOURCE_DIR}/Lib)
set(FILE_DIR ${PROJECT_SOURCE_DIR}/File)
set(TEST_DIR ${PROJECT_SOURCE_DIR}/test)
set(LOG_DIR ${PROJECT_SOURCE_DIR}/Log)
set(BUFFER_DIR ${PROJECT_SOURCE_DIR}/buffer)
set(TX_DIR ${PROJECT_SOURCE_DIR}/tx)
set(DB_DIR ${PROJECT_SOURCE_DIR}/DBMS)
set(RECORD_DIR ${PROJECT_SOURCE_DIR}/record)
set(METADATA_DIR ${PROJECT_SOURCE_DIR}/metadata)
set(QUERY_DIR ${PROJECT_SOURCE_DIR}/query)
set(PLAN_DIR ${PROJECT_SOURCE_DIR}/plan)
set(PARSE_DIR ${PROJECT_SOURCE_DIR}/parse)
set(INDEX_DIR ${PROJECT_SOURCE_DIR}/index)
set(ERROR_DIR ${PROJECT_SOURCE_DIR}/error)
set(TRACE_DIR ${PROJECT_SOURCE_DIR}/trace)

# ========== 源文件 ==========
set(LIB_SOURCES
        ${LIB_DIR}/ByteBuffer.c
        ${LIB_DIR}/CString.c
        ${LIB_DIR}/CVector.c
        ${LIB_DIR}/RBT.c
        ${LIB_DIR}/CMap.c
        ${LIB_DIR}/Error.c
        ${ERROR_DIR}/DBError.c
        ${LIB_DIR}/map.c
        ${LIB_DIR}/Trie.c
        ${LIB_DIR}/StreamTokenizer.c
        ${LIB_DIR}/CList.c
)

set(FILE_SOURCES
        ${FILE_DIR}/BlockID.c
        ${FILE_DIR}/Page.c
        ${FILE_DIR}/FileManager.c
)

set(LOG_SOURCES
        ${LOG_DIR}/LogManager.c
)

set(BUFFER_SOURCES
        ${BUFFER_DIR}/Buffer.c
        ${BUFFER_DIR}/BufferManager.c
        ${BUFFER_DIR}/LRU/LRUCore.c
        ${BUFFER_DIR}/LRU/LRUPolicy.c
)

set(TX_SOURCES
        ${TX_DIR}/BufferList.c
        ${TX_DIR}/concurrency/LockTable.c
        ${TX_DIR}/concurrency/ConcurrencyManager.c
        ${TX_DIR}/recovery/LogRecord.c
        ${TX_DIR}/recovery/RecoveryManager.c
        ${TX_DIR}/Transaction.c
        ${TX_DIR}/TransactionManager.c
        ${TX_DIR}/concurrency/DeadlockDetector.c
)

set(RECORD_SOURCES
        ${RECORD_DIR}/Schema.c
        ${RECORD_DIR}/Layout.c
        ${RECORD_DIR}/RID.c
        ${RECORD_DIR}/RecordPage.c
        ${RECORD_DIR}/TableScan.c
)

set(METADATA_SOURCES
        ${METADATA_DIR}/TableManager.c
        ${METADATA_DIR}/ViewManager.c
        ${METADATA_DIR}/StatInfo.c
        ${METADATA_DIR}/StatManager.c
        ${METADATA_DIR}/IndexInfo.c
        ${METADATA_DIR}/IndexManager.c
        ${METADATA_DIR}/MetadataManager.c
)

set(QUERY_SOURCES
        ${QUERY_DIR}/Constant.c
        ${QUERY_DIR}/Expression.c
        ${QUERY_DIR}/Scan.c
        ${QUERY_DIR}/UpdateScan.c
        ${QUERY_DIR}/ProjectScan.c
        ${QUERY_DIR}/ProductScan.c
        ${QUERY_DIR}/Term.c
        ${QUERY_DIR}/Predicate.c
        ${QUERY_DIR}/SelectScan.c
)

set(PLAN_SOURCES
        ${PLAN_DIR}/Plan.c
        ${PLAN_DIR}/TablePlan.c
        ${PLAN_DIR}/Planner.c
        ${PLAN_DIR}/BasicQueryPlanner.c
        ${PLAN_DIR}/BasicUpdatePlanner.c
        ${PLAN_DIR}/BetterQueryPlanner.c
        ${PLAN_DIR}/OptimizedProductPlan.c
        ${PLAN_DIR}/ProductPlan.c
        ${PLAN_DIR}/ProjectPlan.c
        ${PLAN_DIR}/SelectPlan.c
)

set(PARSE_SOURCES
        ${PARSE_DIR}/Lexer.c
        ${PARSE_DIR}/Parser.c
        ${PARSE_DIR}/CreateIndexData.c
        ${PARSE_DIR}/CreateTableData.c
        ${PARSE_DIR}/DeleteData.c
        ${PARSE_DIR}/QueryData.c
        ${PARSE_DIR}/PredParser.c
        ${PARSE_DIR}/ModifyData.c
        ${PARSE_DIR}/InsertData.c
        ${PARSE_DIR}/CreateViewData.c
)

set(INDEX_SOURCES
        ${INDEX_DIR}/HashIndex.c
)

set(DB_SOURCES
        ${DB_DIR}/DBMS.c
)

set(TRACE_SOURCES
        ${TRACE_DIR}/DBTrace.c
)

# ========== 核心库 ==========
add_library(core STATIC
        ${LIB_SOURCES}
        ${FILE_SOURCES}
        ${LOG_SOURCES}
        ${BUFFER_SOURCES}
        ${TX_SOURCES}
        ${RECORD_SOURCES}
        ${METADATA_SOURCES}
        ${QUERY_SOURCES}
        ${PLAN_SOURCES}
        ${PARSE_SOURCES}
        ${INDEX_SOURCES}
        ${DB_SOURCES}
        ${TRACE_SOURCES}
)

target_include_directories(core PUBLIC
        ${LIB_DIR}
        ${FILE_DIR}
        ${LOG_DIR}
        ${BUFFER_DIR}
        ${TX_DIR}
        ${TX_DIR}/concurrency
        ${TX_DIR}/recovery
        ${RECORD_DIR}
        ${METADATA_DIR}
        ${QUERY_DIR}
        ${PLAN_DIR}
        ${PARSE_DIR}
        ${INDEX_DIR}
        ${DB_DIR}
        ${ERROR_DIR}
        ${TRACE_DIR}
        ${TEST_DIR}
)

# ========== 主程序 ==========
add_executable(NewDBMS main.c)
target_link_libraries(NewDBMS core)

# ========== 测试宏 ==========
macro(add_cmocka_test TEST_NAME SOURCE_FILE)
    if(HAVE_CMOCKA)
        add_executable(${TEST_NAME} ${SOURCE_FILE})
        target_link_libraries(${TEST_NAME} PRIVATE core ${CMOCKA_LIBRARY})

        # Windows 构建后复制 CMocka DLL 到测试程序输出目录
        if(WIN32 AND CMOCKA_DLL)
            add_custom_command(TARGET ${TEST_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${CMOCKA_DLL} $<TARGET_FILE_DIR:${TEST_NAME}>
                    COMMENT "Copying ${CMOCKA_DLL} to $<TARGET_FILE_DIR:${TEST_NAME}>"
            )
        endif()

        add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    endif()
endmacro()

# ========== 测试注册 ==========
if(HAVE_CMOCKA)
    message(STATUS "Registering tests...")

    # File 模块测试
    add_cmocka_test(FileManagerTest ${TEST_DIR}/File/FileManagerTest.c)
    add_cmocka_test(FileManagerBasicTest ${TEST_DIR}/File/FileManagerBasicTest.c)

    # Lib 模块测试
    add_cmocka_test(CStringTest ${TEST_DIR}/Lib/CStringTest.c)
    add_cmocka_test(CListTest ${TEST_DIR}/Lib/CListTest.c)
    add_cmocka_test(LibBasicTest ${TEST_DIR}/Lib/LibBasicTest.c)

    # Error 模块测试
    add_cmocka_test(DBErrorTest ${TEST_DIR}/error/DBErrorTest.c)

    # Log 模块测试
    add_cmocka_test(LogTest ${TEST_DIR}/Log/LogTest.c)
    add_cmocka_test(LogManagerBasicTest ${TEST_DIR}/Log/LogManagerBasicTest.c)

    # Buffer 模块测试
    add_cmocka_test(BufferManagerTest ${TEST_DIR}/buffer/BufferManagerTest.c)
    add_cmocka_test(BufferManagerBasicTest ${TEST_DIR}/buffer/BufferManagerBasicTest.c)

    # TX 模块测试
    add_cmocka_test(TranstionTest ${TEST_DIR}/tx/TranstionTest.c)
    add_cmocka_test(TransactionBasicTest ${TEST_DIR}/tx/TransactionBasicTest.c)
    add_cmocka_test(ConcurrencyBasicTest ${TEST_DIR}/tx/ConcurrencyBasicTest.c)
    add_cmocka_test(RecoveryBasicTest ${TEST_DIR}/tx/RecoveryBasicTest.c)

    # Record 模块测试
    add_cmocka_test(RecordTest ${TEST_DIR}/record/RecordTest.c)
    add_cmocka_test(RecordBasicTest ${TEST_DIR}/record/RecordBasicTest.c)

    # Query 模块测试
    add_cmocka_test(ExpressionTest ${TEST_DIR}/query/ExpressionTest.c)

    # Plan 模块测试
    add_cmocka_test(PlanTest ${TEST_DIR}/plan/PlanTest.c)
    add_cmocka_test(PlanScanBasicTest ${TEST_DIR}/plan/PlanScanBasicTest.c)

    # Parse 模块测试
    add_cmocka_test(ParserTest ${TEST_DIR}/parse/ParserTest.c)
    add_cmocka_test(ParseBasicTest ${TEST_DIR}/parse/ParseBasicTest.c)

    # Metadata 模块测试
    add_cmocka_test(MetadataManagerTest ${TEST_DIR}/metadata/MetadataManagerTest.c)
    add_cmocka_test(MetadataBasicTest ${TEST_DIR}/metadata/MetadataBasicTest.c)

    # Index 模块测试
    add_cmocka_test(HashIndexTest ${TEST_DIR}/index/HashIndexTest.c)

    enable_testing()
else()
    message(STATUS "Tests disabled - CMocka not available")
endif()

# ========== 安装 ==========
install(TARGETS NewDBMS
        RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin
)
