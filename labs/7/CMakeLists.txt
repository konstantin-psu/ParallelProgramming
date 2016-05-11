cmake_minimum_required(VERSION 2.8)
project(assignment2_515)


set(CMAKE_C_COMPILER /usr/bin/mpicc)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -g")
set(CMAKE_VERBOSE_MAKEFILE on)

set(FILEIN_FILES  fileIn.c)
set(FILEOUT_FILES fileOut.c)
set(SIMPLE_FILES  simple.c)
set(RING_C_FILES  ring_c.c)
set(RING_FILES  ring.c)

add_executable(fileIn   ${FILEIN_FILES})
add_executable(fileOut  ${FILEOUT_FILES})
add_executable(simple   ${SIMPLE_FILES})
add_executable(ring_c   ${RING_C_FILES})
add_executable(ring     ${RING_FILES})

target_link_libraries(fileIn m)
target_link_libraries(fileOut m)
target_link_libraries(simple m)
target_link_libraries(ring m)
target_link_libraries(ring_c m)
