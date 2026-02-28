# 最小 CMake 版本
cmake_minimum_required(VERSION 3.15)

# 项目名称
project(NewDBMS VERSION 1.0.0 LANGUAGES C)

# 设置 C 标准
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 可执行文件输出路径
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# 查找 CMocka 库
set(CMOCKA_ROOT ${PROJECT_SOURCE_DIR}/external/cmocka-1.1.0)
include_directories(${CMOCKA_ROOT}/include)

# 根据操作系统找到并链接正确的 cmocka 库
if(WIN32)
    # 在 Windows 上，查找 cmocka 的 DLL 文件
    find_library(CMOCKA_LIBRARY NAMES cmocka.dll libcmocka.a
            PATHS ${CMOCKA_ROOT}/bin ${CMOCKA_ROOT}/lib
            NO_DEFAULT_PATH)
elseif(UNIX)
    # 在 Linux 和 macOS 上，查找 cmocka 的共享对象文件 (.so)
    find_library(CMOCKA_LIBRARY NAMES cmocka libcmocka.a libcmocka.so
            PATHS ${CMOCKA_ROOT}/lib ${CMOCKA_ROOT}/bin
            NO_DEFAULT_PATH)
endif()

# 如果没有找到 cmocka 库，则报错
if(NOT CMOCKA_LIBRARY)
    message(FATAL_ERROR "Could not find cmocka library. Please check your external directory and ensure the correct library file is present for your OS.\n\nExpected locations:\n  Windows: ${CMOCKA_ROOT}/bin or ${CMOCKA_ROOT}/lib\n  Linux/Unix: ${CMOCKA_ROOT}/lib or ${CMOCKA_ROOT}/bin")
endif()

# 设置文件夹路径
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
        ${DB_SOURCES}

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
        ${DB_DIR}
        ${TEST_DIR}
)

# 主程序
add_executable(NewDBMS main.c)
target_link_libraries(NewDBMS core)

# 测试目标宏
macro(add_cmocka_test TEST_NAME SOURCE_FILE)
    add_executable(${TEST_NAME} ${SOURCE_FILE})
    target_link_libraries(${TEST_NAME} core ${CMOCKA_LIBRARY})
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endmacro()

# 现有测试
# File 模块测试
add_cmocka_test(BlockIDTest ${TEST_DIR}/File/BlockIDTest.c)
add_cmocka_test(PageTest ${TEST_DIR}/File/PageTest.c)
add_cmocka_test(FileManagerTest ${TEST_DIR}/File/FileManagerTest.c)

# Lib 模块测试
add_cmocka_test(ByteBufferTest ${TEST_DIR}/Lib/ByteBufferTest.c)
add_cmocka_test(CStringTest ${TEST_DIR}/Lib/CStringTest.c)
add_cmocka_test(RBTTest ${TEST_DIR}/Lib/RBTTest.c)
add_cmocka_test(CMapTest ${TEST_DIR}/Lib/CMapTest.c)
add_cmocka_test(CVectorTest ${TEST_DIR}/Lib/CVectorTest.c)
add_cmocka_test(ListTest ${TEST_DIR}/Lib/ListTest.c)
add_cmocka_test(StreamTokenizerTest ${TEST_DIR}/Lib/StreamTokenizerTest.c)
add_cmocka_test(TrieTest ${TEST_DIR}/Lib/TrieTest.c)

# Log 模块测试
add_cmocka_test(LogTest ${TEST_DIR}/Log/LogTest.c)

# Buffer 模块测试
add_cmocka_test(BufferTest ${TEST_DIR}/buffer/BufferTest.c)
add_cmocka_test(BufferManagerTest ${TEST_DIR}/buffer/BufferManagerTest.c)
add_cmocka_test(BufferLRUPolicy ${TEST_DIR}/buffer/LRU/LRUPolicyTest.c)

# TX 模块测试
add_cmocka_test(TranstionTest ${TEST_DIR}/tx/TranstionTest.c)
add_cmocka_test(LockTest ${TEST_DIR}/tx/LockTest.c)

# Record 模块测试
add_cmocka_test(RIDTest ${TEST_DIR}/record/RIDTest.c)
add_cmocka_test(SchemaTest ${TEST_DIR}/record/SchemaTest.c)
add_cmocka_test(LayoutTest ${TEST_DIR}/record/LayoutTest.c)
add_cmocka_test(RecordTest ${TEST_DIR}/record/RecordTest.c)

# Query 模块测试
add_cmocka_test(ConstantTest ${TEST_DIR}/query/ConstantTest.c)
add_cmocka_test(ExpressionTest ${TEST_DIR}/query/ExpressionTest.c)
add_cmocka_test(TermTest ${TEST_DIR}/query/TermTest.c)
add_cmocka_test(PredicateTest ${TEST_DIR}/query/PredicateTest.c)

# Plan 模块测试
add_cmocka_test(PlanTest ${TEST_DIR}/plan/PlanTest.c)
add_cmocka_test(PlannerTest ${TEST_DIR}/plan/PlannerTest.c)
add_cmocka_test(BasicQueryPlannerTest ${TEST_DIR}/plan/BasicQueryPlannerTest.c)
add_cmocka_test(BasicUpdatePlannerTest ${TEST_DIR}/plan/BasicUpdatePlannerTest.c)
add_cmocka_test(TablePlanTest ${TEST_DIR}/plan/TablePlanTest.c)
add_cmocka_test(SelectPlanTest ${TEST_DIR}/plan/SelectPlanTest.c)
add_cmocka_test(ProjectPlanTest ${TEST_DIR}/plan/ProjectPlanTest.c)
add_cmocka_test(ProductPlanTest ${TEST_DIR}/plan/ProductPlanTest.c)

# Parse 模块测试
add_cmocka_test(ParserTest ${TEST_DIR}/parse/ParserTest.c)
add_cmocka_test(LexerTest ${TEST_DIR}/parse/LexerTest.c)
add_cmocka_test(QueryDataTest ${TEST_DIR}/parse/QueryDataTest.c)
add_cmocka_test(DeleteDataTest ${TEST_DIR}/parse/DeleteDataTest.c)
add_cmocka_test(InsertDataTest ${TEST_DIR}/parse/InsertDataTest.c)
add_cmocka_test(ModifyDataTest ${TEST_DIR}/parse/ModifyDataTest.c)
add_cmocka_test(CreateViewDataTest ${TEST_DIR}/parse/CreateViewDataTest.c)
add_cmocka_test(CreateTableDataTest ${TEST_DIR}/parse/CreateTableDataTest.c)
add_cmocka_test(CreateIndexDataTest ${TEST_DIR}/parse/CreateIndexDataTest.c)
add_cmocka_test(PredParserTest ${TEST_DIR}/parse/PredParserTest.c)

# Metadata 模块测试
add_cmocka_test(MetadataManagerTest ${TEST_DIR}/metadata/MetadataManagerTest.c)
add_cmocka_test(TableManagerTest ${TEST_DIR}/metadata/TableManagerTest.c)
add_cmocka_test(ViewManagerTest ${TEST_DIR}/metadata/ViewManagerTest.c)
add_cmocka_test(IndexManagerTest ${TEST_DIR}/metadata/IndexManagerTest.c)
add_cmocka_test(StatManagerTest ${TEST_DIR}/metadata/StatManagerTest.c)
add_cmocka_test(StatInfoTest ${TEST_DIR}/metadata/StatInfoTest.c)
add_cmocka_test(IndexInfoTest ${TEST_DIR}/metadata/IndexInfoTest.c)

# Index 模块测试
add_cmocka_test(HashIndexTest ${TEST_DIR}/index/HashIndexTest.c)

# DBMS 模块测试
add_cmocka_test(DBMSTest ${TEST_DIR}/DBMSTest.c)

# 安装目标
install(TARGETS NewDBMS
        RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin
)

# 启用测试
enable_testing()
