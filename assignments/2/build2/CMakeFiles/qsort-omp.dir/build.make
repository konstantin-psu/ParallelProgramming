# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Produce verbose output by default.
VERBOSE = 1

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /u/konstan2/github/ParallelProgramming/assignments/2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /u/konstan2/github/ParallelProgramming/assignments/2/build2

# Include any dependencies generated for this target.
include CMakeFiles/qsort-omp.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/qsort-omp.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/qsort-omp.dir/flags.make

CMakeFiles/qsort-omp.dir/src/qsort-omp.c.o: CMakeFiles/qsort-omp.dir/flags.make
CMakeFiles/qsort-omp.dir/src/qsort-omp.c.o: ../src/qsort-omp.c
	$(CMAKE_COMMAND) -E cmake_progress_report /u/konstan2/github/ParallelProgramming/assignments/2/build2/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/qsort-omp.dir/src/qsort-omp.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/qsort-omp.dir/src/qsort-omp.c.o   -c /u/konstan2/github/ParallelProgramming/assignments/2/src/qsort-omp.c

CMakeFiles/qsort-omp.dir/src/qsort-omp.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/qsort-omp.dir/src/qsort-omp.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /u/konstan2/github/ParallelProgramming/assignments/2/src/qsort-omp.c > CMakeFiles/qsort-omp.dir/src/qsort-omp.c.i

CMakeFiles/qsort-omp.dir/src/qsort-omp.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/qsort-omp.dir/src/qsort-omp.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /u/konstan2/github/ParallelProgramming/assignments/2/src/qsort-omp.c -o CMakeFiles/qsort-omp.dir/src/qsort-omp.c.s

CMakeFiles/qsort-omp.dir/src/qsort-omp.c.o.requires:
.PHONY : CMakeFiles/qsort-omp.dir/src/qsort-omp.c.o.requires

CMakeFiles/qsort-omp.dir/src/qsort-omp.c.o.provides: CMakeFiles/qsort-omp.dir/src/qsort-omp.c.o.requires
	$(MAKE) -f CMakeFiles/qsort-omp.dir/build.make CMakeFiles/qsort-omp.dir/src/qsort-omp.c.o.provides.build
.PHONY : CMakeFiles/qsort-omp.dir/src/qsort-omp.c.o.provides

CMakeFiles/qsort-omp.dir/src/qsort-omp.c.o.provides.build: CMakeFiles/qsort-omp.dir/src/qsort-omp.c.o

# Object files for target qsort-omp
qsort__omp_OBJECTS = \
"CMakeFiles/qsort-omp.dir/src/qsort-omp.c.o"

# External object files for target qsort-omp
qsort__omp_EXTERNAL_OBJECTS =

qsort-omp: CMakeFiles/qsort-omp.dir/src/qsort-omp.c.o
qsort-omp: CMakeFiles/qsort-omp.dir/build.make
qsort-omp: CMakeFiles/qsort-omp.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable qsort-omp"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/qsort-omp.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/qsort-omp.dir/build: qsort-omp
.PHONY : CMakeFiles/qsort-omp.dir/build

CMakeFiles/qsort-omp.dir/requires: CMakeFiles/qsort-omp.dir/src/qsort-omp.c.o.requires
.PHONY : CMakeFiles/qsort-omp.dir/requires

CMakeFiles/qsort-omp.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/qsort-omp.dir/cmake_clean.cmake
.PHONY : CMakeFiles/qsort-omp.dir/clean

CMakeFiles/qsort-omp.dir/depend:
	cd /u/konstan2/github/ParallelProgramming/assignments/2/build2 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /u/konstan2/github/ParallelProgramming/assignments/2 /u/konstan2/github/ParallelProgramming/assignments/2 /u/konstan2/github/ParallelProgramming/assignments/2/build2 /u/konstan2/github/ParallelProgramming/assignments/2/build2 /u/konstan2/github/ParallelProgramming/assignments/2/build2/CMakeFiles/qsort-omp.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/qsort-omp.dir/depend

