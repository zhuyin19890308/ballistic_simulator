cmake_minimum_required(VERSION 3.10)
project(ammo LANGUAGES CXX)

set(TARGET_NAME ammo)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

## 添加宏定义
#add_definitions(-DBSL_BULLET_DATA_LIBRARY_QT_LIBRARY)
#add_definitions(-DQT_DEPRECATED_WARNINGS)
## add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x060000)

# 收集 source 目录下的所有 .cpp 文件
file(GLOB SOURCES "source/*.cpp")


# 创建动态库和静态库
add_library(${TARGET_NAME}_shared SHARED ${SOURCES})
add_library(${TARGET_NAME}_static STATIC ${SOURCES})

# 如果未定义 PROJECT_BUILD_DIR，则将其设置为默认的构建输出目录
if(NOT DEFINED PROJECT_BUILD_DIR)
    set(PROJECT_BUILD_DIR "${CMAKE_SOURCE_DIR}/../build")
endif()

# 设置输出目录
foreach(target ${TARGET_NAME}_shared ${TARGET_NAME}_static)
    set_target_properties(${target} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BUILD_DIR}"
        LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BUILD_DIR}"
        RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BUILD_DIR}"
    )
    target_include_directories(${target} PRIVATE
        ${CMAKE_SOURCE_DIR}/source
        ${CMAKE_SOURCE_DIR}/../utils/matrix
    )
endforeach()
