cmake_minimum_required(VERSION 3.10)

# 项目名称和版本
project(ballistic VERSION 1.0 LANGUAGES CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 定义目标库名称
set(TARGET_NAME ballistic)

# 添加定义
add_definitions(-DballisticLibrary)
add_definitions(-DQT_DEPRECATED_WARNINGS)

# 源文件
set(SOURCES
        ./source/ballisticTypeDef.cpp
        ./source/ballisticWeatherMsg.cpp
        ./source/ballistic_GP.cpp
        ./source/ballistic_cannon.cpp
        ./source/ballistic_tank.cpp
        ./source/baseClass.cpp
)

# 头文件
set(HEADERS
        ./source/ballisticTypeDef.h
        ./source/ballisticWeatherMsg.h
        ./source/ballistic_GP.h
        ./source/ballistic_cannon.h
        ./source/ballistic_tank.h
        ./source/baseClass.h
        ./source/ammoData.h
)

# 包含路径
include_directories(
        ../utils
        ../utils/boost
        ../utils/matrix
        ../utils/spdlog
        ../Ammolib/source
)

# 收集 source 目录下的所有 .cpp 文件
file(GLOB SOURCES "source/*.cpp")
# 定义动态库和静态库目标
add_library(${TARGET_NAME}_shared SHARED ${SOURCES})
add_library(${TARGET_NAME}_static STATIC ${SOURCES})

# 如果未定义 PROJECT_BUILD_DIR，则将其设置为默认的构建输出目录
if(NOT DEFINED PROJECT_BUILD_DIR)
    set(PROJECT_BUILD_DIR "${CMAKE_SOURCE_DIR}/../build")
endif()

# 设置输出目录
set_target_properties(${TARGET_NAME}_shared ${TARGET_NAME}_static PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BUILD_DIR}"
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BUILD_DIR}"
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BUILD_DIR}"
)

# 链接其他库
target_link_directories(${TARGET_NAME}_shared PRIVATE ../build)
target_link_directories(${TARGET_NAME}_static PRIVATE ../build)
target_link_libraries(${TARGET_NAME}_shared ammo_shared)
target_link_libraries(${TARGET_NAME}_static ammo_static)

