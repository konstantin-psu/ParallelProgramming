cmake_minimum_required(VERSION 3.2)
project(qsort_pthd)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -pthread -g -DVERBOSE -D_LARGE_THREADS")

set(SOURCE_FILES src/qsort.c src/task-queue.c src/task-queue.h)

add_executable(qsort ${SOURCE_FILES})
