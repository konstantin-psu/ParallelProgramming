cmake_minimum_required(VERSION 2.8)
project(assignment2_515)


set(CMAKE_C_COMPILER /usr/bin/mpicc)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -g")
set(CMAKE_VERBOSE_MAKEFILE on)

set(FILEIN_FILES  fileIn.c)
set(FILEOUT_FILES fileOut.c)
set(SIMPLE_FILES  simple.c)

add_executable(fileIn   ${FILEIN_FILES})
add_executable(fileOut  ${FILEOUT_FILES})
add_executable(simple   ${SIMPLE_FILES})

target_link_libraries(fileIn m)
target_link_libraries(fileOut m)
target_link_libraries(simple m)

