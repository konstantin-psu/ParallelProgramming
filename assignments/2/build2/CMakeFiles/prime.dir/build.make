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
include CMakeFiles/prime.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/prime.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/prime.dir/flags.make

CMakeFiles/prime.dir/src/prime.c.o: CMakeFiles/prime.dir/flags.make
CMakeFiles/prime.dir/src/prime.c.o: ../src/prime.c
	$(CMAKE_COMMAND) -E cmake_progress_report /u/konstan2/github/ParallelProgramming/assignments/2/build2/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/prime.dir/src/prime.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/prime.dir/src/prime.c.o   -c /u/konstan2/github/ParallelProgramming/assignments/2/src/prime.c

CMakeFiles/prime.dir/src/prime.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/prime.dir/src/prime.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /u/konstan2/github/ParallelProgramming/assignments/2/src/prime.c > CMakeFiles/prime.dir/src/prime.c.i

CMakeFiles/prime.dir/src/prime.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/prime.dir/src/prime.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /u/konstan2/github/ParallelProgramming/assignments/2/src/prime.c -o CMakeFiles/prime.dir/src/prime.c.s

CMakeFiles/prime.dir/src/prime.c.o.requires:
.PHONY : CMakeFiles/prime.dir/src/prime.c.o.requires

CMakeFiles/prime.dir/src/prime.c.o.provides: CMakeFiles/prime.dir/src/prime.c.o.requires
	$(MAKE) -f CMakeFiles/prime.dir/build.make CMakeFiles/prime.dir/src/prime.c.o.provides.build
.PHONY : CMakeFiles/prime.dir/src/prime.c.o.provides

CMakeFiles/prime.dir/src/prime.c.o.provides.build: CMakeFiles/prime.dir/src/prime.c.o

# Object files for target prime
prime_OBJECTS = \
"CMakeFiles/prime.dir/src/prime.c.o"

# External object files for target prime
prime_EXTERNAL_OBJECTS =

prime: CMakeFiles/prime.dir/src/prime.c.o
prime: CMakeFiles/prime.dir/build.make
prime: CMakeFiles/prime.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable prime"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/prime.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/prime.dir/build: prime
.PHONY : CMakeFiles/prime.dir/build

CMakeFiles/prime.dir/requires: CMakeFiles/prime.dir/src/prime.c.o.requires
.PHONY : CMakeFiles/prime.dir/requires

CMakeFiles/prime.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/prime.dir/cmake_clean.cmake
.PHONY : CMakeFiles/prime.dir/clean

CMakeFiles/prime.dir/depend:
	cd /u/konstan2/github/ParallelProgramming/assignments/2/build2 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /u/konstan2/github/ParallelProgramming/assignments/2 /u/konstan2/github/ParallelProgramming/assignments/2 /u/konstan2/github/ParallelProgramming/assignments/2/build2 /u/konstan2/github/ParallelProgramming/assignments/2/build2 /u/konstan2/github/ParallelProgramming/assignments/2/build2/CMakeFiles/prime.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/prime.dir/depend

