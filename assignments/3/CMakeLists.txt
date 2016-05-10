cmake_minimum_required(VERSION 2.8)
project(assignment2_515)


set(CMAKE_C_COMPILER /usr/bin/mpicc)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -g")
set(CMAKE_VERBOSE_MAKEFILE on)

set(QSOURCE_FILES src/extsort.c)
set(DATAGEN_FILES src/datagen.c)
set(QSECSOURCE_FILES src/qsort.c)
set(VERIFY_FILES src/verify.c)

add_executable(extsort ${QSOURCE_FILES})
add_executable(datagen ${DATAGEN_FILES})
add_executable(verify ${VERIFY_FILES})
add_executable(qsort ${QSECSOURCE_FILES})

target_link_libraries(extsort m)
target_link_libraries(datagen m)
target_link_libraries(verify m)
target_link_libraries(qsort m)

