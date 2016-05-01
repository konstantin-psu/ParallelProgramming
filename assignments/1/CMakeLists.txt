cmake_minimum_required(VERSION 2.8)
project(assignment1_515)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -pthread -g -DVERBOSE -D_LARGE_THREADS")

set(QSOURCE_FILES src/qsort-pthd.c src/task-queue.c src/task-queue.h)
set(PSOURCE_FILES src/prodcons-pthd.c src/task-queue.c src/task-queue.h)

set(QSSOURCE_FILES origin/qsort.c    origin/task-queue.c origin/task-queue.h)
set(PSSOURCE_FILES origin/prodcons.c origin/task-queue.c origin/task-queue.h)

add_executable(qsort-pthd ${QSOURCE_FILES})
add_executable(prodcons-pthd ${PSOURCE_FILES})

add_executable(qsort ${QSSOURCE_FILES})
