cmake_minimum_required(VERSION 3.2)
project(assignment2_515)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -g")
set(CMAKE_VERBOSE_MAKEFILE on)
find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

set(QSOURCE_FILES src/qsort-omp.c)
set(PSOURCE_FILES src/prime-omp.c)
set(QSECSOURCE_FILES src/qsort.c)
set(PSECSOURCE_FILES src/prime.c)

add_executable(qsort-omp ${QSOURCE_FILES})
add_executable(prime-omp ${PSOURCE_FILES})

target_link_libraries(qsort-omp m)
target_link_libraries(prime-omp m)
