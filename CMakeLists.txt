cmake_minimum_required(VERSION 3.21)
project(memory_pool)

#设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

#添加源文件
file(GLOB_RECURSE SRC_FILES src/*.cc)
message(AUTHOR_WARNING ${SRC_FILES})

#创建共享库
add_library(${PROJECT_NAME} SHARED ${SRC_FILES})
# add_library(${PROJECT_NAME} SHARED "src/*.cc")

#指定头文件目录
target_include_directories(${PROJECT_NAME} PUBLIC include)