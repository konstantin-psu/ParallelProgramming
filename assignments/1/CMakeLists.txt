cmake_minimum_required(VERSION 3.3)
project(1)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -pthread -g -DVERBOSE -D_LARGE_THREADS")

set(SOURCE_FILES qsort.c task-queue.c task-queue.h)

add_executable(qsort ${SOURCE_FILES})
