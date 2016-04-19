cmake_minimum_required(VERSION 3.2)
project(qsort_pthd)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -pthread -g -DVERBOSE -D_LARGE_THREADS")

set(QSOURCE_FILES src/qsort-pthd.c src/task-queue.c src/task-queue.h)
set(PSOURCE_FILES src/prodcons-pthd.c src/task-queue.c src/task-queue.h)

add_executable(qsort-pthd ${QSOURCE_FILES})
add_executable(prodcons-pthd ${PSOURCE_FILES})
