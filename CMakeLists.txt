cmake_minimum_required(VERSION 3.10)

# 项目名称
project(BallisticSimulator LANGUAGES CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 定义统一的构建输出目录
set(PROJECT_BUILD_DIR "${CMAKE_SOURCE_DIR}/build")

# 添加选项，用于选择编译动态库或静态库
option(BUILD_SHARED_LIBS "Build shared libraries instead of static ones" ON)

# 添加子工程
add_subdirectory(Ammolib)
add_subdirectory(Ballisticlib)

# 设置依赖关系
add_dependencies(ballistic_shared ammo_shared)
add_dependencies(ballistic_static ammo_static)