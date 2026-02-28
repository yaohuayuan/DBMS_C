# external 文件夹

该文件夹包含数据库系统的外部依赖库。

## 内容

- **cmocka-1.1.0/**: 单元测试框架cmocka的安装包和头文件
  - **include/**: cmocka的头文件
    - **cmocka.h**: cmocka的主头文件
    - **cmocka_pbc.h**: cmocka的属性基类测试头文件
    - **cmocka_version.h**: cmocka的版本信息头文件
  - **lib/**: cmocka的库文件
    - **cmake/**: cmake配置文件
    - **pkgconfig/**: pkgconfig配置文件
    - **libcmocka.dll.a**: cmocka的静态库文件

## 用途

- 存储和管理外部依赖库
- 提供单元测试框架支持
- 包含编译和链接所需的头文件和库文件
- 确保项目的依赖项可以方便地访问和使用