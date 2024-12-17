cmake_minimum_required(VERSION 3.10)
project(D2D)

# 尋找 Qt5 Widgets 模組
find_package(Qt5 REQUIRED COMPONENTS Widgets)

# 指定你的源文件
set(SOURCES main.cpp src/showQT.cpp src/algorithm.cpp)

# 添加頭文件目錄
include_directories(${CMAKE_SOURCE_DIR}/src)

# 添加可執行檔案
add_executable(D2D ${SOURCES})

# 連結 Qt5 Widgets 庫
target_link_libraries(D2D Qt5::Widgets)

