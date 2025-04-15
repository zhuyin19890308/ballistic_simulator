# 设置最低 CMake 版本
cmake_minimum_required(VERSION 3.10)

# 定义项目名称
project(TestProject LANGUAGES CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 设置动态库的路径
set(LIBRARY_OUTPUT_PATH "../build")
get_filename_component(ABS_LIBRARY_OUTPUT_PATH ${LIBRARY_OUTPUT_PATH} ABSOLUTE)
message(STATUS "Library output absolute path: ${ABS_LIBRARY_OUTPUT_PATH}")

# 包含路径
include_directories(
        ../utils
        ../utils/boost
        ../utils/matrix
        ../utils/spdlog
        ../Ammolib/source
)

# 添加包含目录
find_package(Boost REQUIRED)
        if (Boost_FOUND)
            include_directories(${Boost_INCLUDE_DIRS})
        endif()

# 查找源文件
file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/*.cpp)

# 添加可执行文件
add_executable(test_program ${SOURCES}
        ./include/ballisticTypeDef.h
        ./include/ballisticWeatherMsg.h
        ./include/ballistic_GP.h
        ./include/ballistic_cannon.h
        ./include/ballistic_tank.h
        ./include/baseClass.h
        ./include/ammoData.h
)

# 链接动态库
if (WIN32)
    target_link_libraries(test_program PRIVATE ${ABS_LIBRARY_OUTPUT_PATH}/libballistic_shared.dll)
elseif (APPLE)
    target_link_libraries(test_program PRIVATE ${ABS_LIBRARY_OUTPUT_PATH}/libballistic_shared.dylib)
else()
    target_link_libraries(test_program PRIVATE ${ABS_LIBRARY_OUTPUT_PATH}/libballistic_shared.so)
endif()

# 设置运行时库路径
set_target_properties(test_program PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
set_target_properties(test_program PROPERTIES
        INSTALL_RPATH "@loader_path/../../build"
)