cmake_minimum_required(VERSION 3.15)
project(NewDBMS VERSION 1.0.0 LANGUAGES C)

# 设置 C 标准
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 可执行文件输出路径
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# 查找 CMocka 包
set(CMOCKA_ROOT ${PROJECT_SOURCE_DIR}/external/cmocka-1.1.0)
find_package(cmocka REQUIRED
        PATHS ${CMOCKA_ROOT}
        PATH_SUFFIXES lib/cmake/cmocka
        NO_DEFAULT_PATH
)

# 设置文件夹路径
set(LIB_DIR ${PROJECT_SOURCE_DIR}/Lib)
set(FILE_DIR ${PROJECT_SOURCE_DIR}/File)
set(TEST_DIR ${PROJECT_SOURCE_DIR}/test)
set(LOG_DIR ${PROJECT_SOURCE_DIR}/Log)
set(BUFFER_DIR ${PROJECT_SOURCE_DIR}/buffer)
set(TX_DIR ${PROJECT_SOURCE_DIR}/tx)
set(DB_DIR ${PROJECT_SOURCE_DIR}/DBMS)

# 新增模块路径（来自第一个项目）
set(RECORD_DIR ${PROJECT_SOURCE_DIR}/record)
set(METADATA_DIR ${PROJECT_SOURCE_DIR}/metadata)
set(QUERY_DIR ${PROJECT_SOURCE_DIR}/query)
set(PLAN_DIR ${PROJECT_SOURCE_DIR}/plan)
set(PARSE_DIR ${PROJECT_SOURCE_DIR}/parse)
set(INDEX_DIR ${PROJECT_SOURCE_DIR}/index)
set(SERVER_DIR ${PROJECT_SOURCE_DIR}/server)

# 获取源文件
set(LIB_SOURCES
        ${LIB_DIR}/ByteBuffer.c
        ${LIB_DIR}/CString.c
        ${LIB_DIR}/CVector.c
        ${LIB_DIR}/RBT.c
        ${LIB_DIR}/CMap.c
        ${LIB_DIR}/rwlock.h
        ${LIB_DIR}/Error.c
        ${LIB_DIR}/map.c
        ${LIB_DIR}/Trie.c
        ${LIB_DIR}/StreamTokenizer.c
        ${LIB_DIR}/List.c
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
)

set(TX_SOURCES
        ${TX_DIR}/BufferList.c
        ${TX_DIR}/concurrency/LockTable.c
        ${TX_DIR}/concurrency/ConcurrencyManager.c
        ${TX_DIR}/recovery/LogRecord.c
        ${TX_DIR}/recovery/RecoveryManager.c
        ${TX_DIR}/Transaction.c
        tx/TransactionManager.c
        tx/concurrency/DeadlockDetector.c
        tx/concurrency/DeadlockDetector.h
)

# 新增模块源文件（来自第一个项目）
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

# 添加核心库
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
        ${SERVER_SOURCES}
        ${DB_SOURCES}
        main.c
)

# 包含目录
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
        ${SERVER_DIR}
        ${DB_DIR}
        ${TEST_DIR}
)

# 主程序
add_executable(NewDBMS main.c)
target_link_libraries(NewDBMS core)

# 测试目标宏
macro(add_cmocka_test TEST_NAME SOURCE_FILE)
    add_executable(${TEST_NAME} ${SOURCE_FILE})
    target_link_libraries(${TEST_NAME} core cmocka::cmocka)
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

    # 复制 CMocka DLL 到测试目录
    add_custom_command(TARGET ${TEST_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMOCKA_ROOT}/bin/cmocka.dll
            $<TARGET_FILE_DIR:${TEST_NAME}>
    )
endmacro()

# 现有测试
#add_cmocka_test(BlockIDTest ${TEST_DIR}/File/BlockIDTest.c)
#add_cmocka_test(ByteBufferTest ${TEST_DIR}/Lib/ByteBufferTest.c)
#add_cmocka_test(CStringTest ${TEST_DIR}/Lib/CStringTest.c)
#add_cmocka_test(PageTest ${TEST_DIR}/File/PageTest.c)
#add_cmocka_test(RBTTest ${TEST_DIR}/Lib/RBTTest.c)
#add_cmocka_test(CMapTest ${TEST_DIR}/Lib/CMapTest.c)
#add_cmocka_test(Test ${TEST_DIR}/Test.c)
#add_cmocka_test(FileManagerTest ${TEST_DIR}/File/FileManagerTest.c)
#add_cmocka_test(LogTest ${TEST_DIR}/Log/LogTest.c)
#add_cmocka_test(CVectorTest ${TEST_DIR}/Lib/CVectorTest.c)
#add_cmocka_test(BufferTest ${TEST_DIR}/buffer/BufferTest.c)
#add_cmocka_test(BufferManagerTest ${TEST_DIR}/buffer/BufferManagerTest.c)
#add_cmocka_test(TranstionTest ${TEST_DIR}/tx/TranstionTest.c)
add_cmocka_test(BufferLRU ${TEST_DIR}/buffer/BufferLRU.c)
add_cmocka_test(test_bench_tps ${TEST_DIR}/test_bench_tps.c)
add_cmocka_test(test1GB ${TEST_DIR}/testLog1GB.c)
add_cmocka_test(LockTest ${TEST_DIR}/tx/LockTest.c)
# 安装目标
install(TARGETS NewDBMS
        RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin
)

# 启用测试
enable_testing()