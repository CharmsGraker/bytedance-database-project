# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


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
CMAKE_SOURCE_DIR = /home/nekonoyume/codeDev/GithubProject/bytedance-database-project

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/nekonoyume/codeDev/GithubProject/bytedance-database-project

# Include any dependencies generated for this target.
include test/lab2/CMakeFiles/column_sum_test.dir/depend.make

# Include the progress variables for this target.
include test/lab2/CMakeFiles/column_sum_test.dir/progress.make

# Include the compile flags for this target's objects.
include test/lab2/CMakeFiles/column_sum_test.dir/flags.make

test/lab2/CMakeFiles/column_sum_test.dir/column_sum_test.cc.o: test/lab2/CMakeFiles/column_sum_test.dir/flags.make
test/lab2/CMakeFiles/column_sum_test.dir/column_sum_test.cc.o: test/lab2/column_sum_test.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nekonoyume/codeDev/GithubProject/bytedance-database-project/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/lab2/CMakeFiles/column_sum_test.dir/column_sum_test.cc.o"
	cd /home/nekonoyume/codeDev/GithubProject/bytedance-database-project/test/lab2 && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/column_sum_test.dir/column_sum_test.cc.o -c /home/nekonoyume/codeDev/GithubProject/bytedance-database-project/test/lab2/column_sum_test.cc

test/lab2/CMakeFiles/column_sum_test.dir/column_sum_test.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/column_sum_test.dir/column_sum_test.cc.i"
	cd /home/nekonoyume/codeDev/GithubProject/bytedance-database-project/test/lab2 && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nekonoyume/codeDev/GithubProject/bytedance-database-project/test/lab2/column_sum_test.cc > CMakeFiles/column_sum_test.dir/column_sum_test.cc.i

test/lab2/CMakeFiles/column_sum_test.dir/column_sum_test.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/column_sum_test.dir/column_sum_test.cc.s"
	cd /home/nekonoyume/codeDev/GithubProject/bytedance-database-project/test/lab2 && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nekonoyume/codeDev/GithubProject/bytedance-database-project/test/lab2/column_sum_test.cc -o CMakeFiles/column_sum_test.dir/column_sum_test.cc.s

test/lab2/CMakeFiles/column_sum_test.dir/column_sum_test.cc.o.requires:

.PHONY : test/lab2/CMakeFiles/column_sum_test.dir/column_sum_test.cc.o.requires

test/lab2/CMakeFiles/column_sum_test.dir/column_sum_test.cc.o.provides: test/lab2/CMakeFiles/column_sum_test.dir/column_sum_test.cc.o.requires
	$(MAKE) -f test/lab2/CMakeFiles/column_sum_test.dir/build.make test/lab2/CMakeFiles/column_sum_test.dir/column_sum_test.cc.o.provides.build
.PHONY : test/lab2/CMakeFiles/column_sum_test.dir/column_sum_test.cc.o.provides

test/lab2/CMakeFiles/column_sum_test.dir/column_sum_test.cc.o.provides.build: test/lab2/CMakeFiles/column_sum_test.dir/column_sum_test.cc.o


# Object files for target column_sum_test
column_sum_test_OBJECTS = \
"CMakeFiles/column_sum_test.dir/column_sum_test.cc.o"

# External object files for target column_sum_test
column_sum_test_EXTERNAL_OBJECTS =

test/lab2/column_sum_test: test/lab2/CMakeFiles/column_sum_test.dir/column_sum_test.cc.o
test/lab2/column_sum_test: test/lab2/CMakeFiles/column_sum_test.dir/build.make
test/lab2/column_sum_test: /usr/local/lib/libgtest_main.a
test/lab2/column_sum_test: /usr/local/lib/libgtest.a
test/lab2/column_sum_test: src/libdatabase_project_lib.so
test/lab2/column_sum_test: test/lab2/CMakeFiles/column_sum_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/nekonoyume/codeDev/GithubProject/bytedance-database-project/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable column_sum_test"
	cd /home/nekonoyume/codeDev/GithubProject/bytedance-database-project/test/lab2 && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/column_sum_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/lab2/CMakeFiles/column_sum_test.dir/build: test/lab2/column_sum_test

.PHONY : test/lab2/CMakeFiles/column_sum_test.dir/build

test/lab2/CMakeFiles/column_sum_test.dir/requires: test/lab2/CMakeFiles/column_sum_test.dir/column_sum_test.cc.o.requires

.PHONY : test/lab2/CMakeFiles/column_sum_test.dir/requires

test/lab2/CMakeFiles/column_sum_test.dir/clean:
	cd /home/nekonoyume/codeDev/GithubProject/bytedance-database-project/test/lab2 && $(CMAKE_COMMAND) -P CMakeFiles/column_sum_test.dir/cmake_clean.cmake
.PHONY : test/lab2/CMakeFiles/column_sum_test.dir/clean

test/lab2/CMakeFiles/column_sum_test.dir/depend:
	cd /home/nekonoyume/codeDev/GithubProject/bytedance-database-project && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/nekonoyume/codeDev/GithubProject/bytedance-database-project /home/nekonoyume/codeDev/GithubProject/bytedance-database-project/test/lab2 /home/nekonoyume/codeDev/GithubProject/bytedance-database-project /home/nekonoyume/codeDev/GithubProject/bytedance-database-project/test/lab2 /home/nekonoyume/codeDev/GithubProject/bytedance-database-project/test/lab2/CMakeFiles/column_sum_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/lab2/CMakeFiles/column_sum_test.dir/depend

