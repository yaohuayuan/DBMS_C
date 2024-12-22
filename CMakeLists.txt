# 设置最低要求的 CMake 版本
cmake_minimum_required(VERSION 3.24)

# 项目名称及版本号
project(DBMS_C VERSION 1.0.0 LANGUAGES C)

# 设置 C 标准
set(CMAKE_C_STANDARD 11)

# 可执行文件输出路径
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# 定义模块化源文件
set(SOURCES
        # File 模块
        file/BlockId.c
        file/Page.c
        file/FileManager.c

        # Head 模块
        head/ByteBuffer.c
        head/Error.c
        head/map.c
        head/List.c
        head/Trie.c
        head/StreamTokenizer.c

        # Log 模块
        log/LogIterator.c
        log/LogManager.c

        # Buffer 模块
        buffer/Buffer.c
        buffer/BufferManager.c

        # Record 模块
        record/Schema.c
        record/Layout.c
        record/RID.c
        record/RecordPage.c
        record/TableScan.c

        # Transaction 模块
        tx/BufferList.c
        tx/concurrency/LockTable.c
        tx/concurrency/ConcurrencyManager.c
        tx/recovery/LogRecord.c
        tx/recovery/RecoveryManager.c
        tx/Transaction.c

        # Metadata 模块
        metadata/TableManager.c
        metadata/ViewManager.c
        metadata/StatInfo.c
        metadata/StartManager.c
        metadata/IndexInfo.c
        metadata/IndexManager.c
        metadata/MetadataManager.c

        # Query 模块
        query/Constant.c
        query/Expression.c
        query/Scan.c
        query/UpdateScan.c
        query/ProjectScan.c
        query/ProductScan.c
        query/Term.c
        query/Predicate.c

        # Plan 模块
        plan/Plan.c
        plan/TablePlan.c
        plan/Planner.c
        plan/BasicQueryPlanner.c

        # Parse 模块
        parse/Lexer.c
        parse/Parser.c
        parse/CreateIndexData.c
        parse/CreateTableData.c
        parse/DeleteData.c
        parse/QueryData.c
        parse/PredParser.c
        parse/ModifyData.c
        parse/InsertData.c


        # Index 模块
        index/HashIndex.c

        # Server 模块
        server/SimpleDB.c


        # 额外 Plan 模块
        plan/BasicUpdatePlanner.c
        plan/BetterQueryPlanner.c
        plan/OptimizedProductPlan.c
        plan/ProductPlan.c
        plan/ProjectPlan.c
        plan/SelectPlan.c
        parse/CreateViewData.c parse/CreateViewData.h query/SelectScan.h query/SelectScan.c

        # Test 模块
#        test/test.c
        main.c
        tx/TransactionManager.c tx/TransactionManager.h)

# 定义头文件目录
set(INCLUDE_DIRS
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
        server
        )

# 定义可执行文件
add_executable(DBMS_C_EXEC ${SOURCES})

# 指定头文件目录
target_include_directories(DBMS_C_EXEC PRIVATE ${INCLUDE_DIRS})
