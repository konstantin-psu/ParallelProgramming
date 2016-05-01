cmake_minimum_required(VERSION 2.8)
project(assignment2_515)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -g")
set(CMAKE_VERBOSE_MAKEFILE on)
find_package(OpenMP)
if (OPENMP_FOUND)
#    set (C_F "${OpenMP_C_FLAGS}")
#    set (CXX_F "${OpenMP_CXX_FLAGS}")
     set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
     set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

set(QSOURCE_FILES src/qsort-omp.c)
set(PSOURCE_FILES src/prime-omp.c)
set(QSECSOURCE_FILES src/qsort.c)
set(PSECSOURCE_FILES src/prime.c)

add_executable(qsort-omp ${QSOURCE_FILES})
add_executable(prime-omp ${PSOURCE_FILES})

#set_target_properties(qsort-omp PROPERTIES COMPILE_FLAGS "${C_F}")
#set_target_properties(prime-omp PROPERTIES COMPILE_FLAGS "${C_F})


add_executable(qsort ${QSECSOURCE_FILES})
add_executable(prime ${PSECSOURCE_FILES})

target_link_libraries(prime m)
target_link_libraries(prime-omp m)

