# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_COMMAND = /snap/clion/58/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/58/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/tudor/Desktop/Retele/CLionProjects/server1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/tudor/Desktop/Retele/CLionProjects/server1/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/server1.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/server1.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/server1.dir/flags.make

CMakeFiles/server1.dir/main.cpp.o: CMakeFiles/server1.dir/flags.make
CMakeFiles/server1.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tudor/Desktop/Retele/CLionProjects/server1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/server1.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/server1.dir/main.cpp.o -c /home/tudor/Desktop/Retele/CLionProjects/server1/main.cpp

CMakeFiles/server1.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/server1.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tudor/Desktop/Retele/CLionProjects/server1/main.cpp > CMakeFiles/server1.dir/main.cpp.i

CMakeFiles/server1.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/server1.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tudor/Desktop/Retele/CLionProjects/server1/main.cpp -o CMakeFiles/server1.dir/main.cpp.s

# Object files for target server1
server1_OBJECTS = \
"CMakeFiles/server1.dir/main.cpp.o"

# External object files for target server1
server1_EXTERNAL_OBJECTS =

server1: CMakeFiles/server1.dir/main.cpp.o
server1: CMakeFiles/server1.dir/build.make
server1: CMakeFiles/server1.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/tudor/Desktop/Retele/CLionProjects/server1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable server1"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/server1.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/server1.dir/build: server1

.PHONY : CMakeFiles/server1.dir/build

CMakeFiles/server1.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/server1.dir/cmake_clean.cmake
.PHONY : CMakeFiles/server1.dir/clean

CMakeFiles/server1.dir/depend:
	cd /home/tudor/Desktop/Retele/CLionProjects/server1/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tudor/Desktop/Retele/CLionProjects/server1 /home/tudor/Desktop/Retele/CLionProjects/server1 /home/tudor/Desktop/Retele/CLionProjects/server1/cmake-build-debug /home/tudor/Desktop/Retele/CLionProjects/server1/cmake-build-debug /home/tudor/Desktop/Retele/CLionProjects/server1/cmake-build-debug/CMakeFiles/server1.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/server1.dir/depend

