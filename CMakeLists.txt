cmake_minimum_required(VERSION 2.8)

project(stereomatch)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

aux_source_directory(. SRC_LIST)

add_executable(stereomatch ${SRC_LIST})
target_link_libraries(stereomatch ${OpenCV_LIBS})
