cmake_minimum_required(VERSION 3.24)

# 设置项目名称和语言
project(NewDBMS C)

# 设置 C 标准
set(CMAKE_C_STANDARD 11)

# 设置源文件和头文件目录
# 将所有源文件列出
file(GLOB SOURCES
        Lib/CString.c
        File/BlockID.c
        Lib/CVector.c
        test.c
        )

# 将所有头文件目录列出
include_directories(
        Lib
        File
)

# 创建可执行文件
add_executable(NewDBMS ${SOURCES} File/Page.c File/Page.h Lib/ByteBuffer.c Lib/ByteBuffer.h)


