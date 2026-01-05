cmake_minimum_required(VERSION 4.0)
project(Algoritmer)

set(CMAKE_CXX_STANDARD 20)

set (SOURCE_FILES
        BinaryTree.cpp
        CuckooHashing.cpp
#        exam_jan3_25.cpp
        reexam_feb27_25.cpp
)

include_directories(.)

add_executable(Algoritmer ${SOURCE_FILES})
