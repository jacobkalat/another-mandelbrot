# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/jake/CLionProjects/another-mandelbrot/testing/test_server_NET

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jake/CLionProjects/another-mandelbrot/testing/test_server_NET/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/test_server_NET.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/test_server_NET.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test_server_NET.dir/flags.make

CMakeFiles/test_server_NET.dir/main.c.o: CMakeFiles/test_server_NET.dir/flags.make
CMakeFiles/test_server_NET.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jake/CLionProjects/another-mandelbrot/testing/test_server_NET/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/test_server_NET.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/test_server_NET.dir/main.c.o -c /home/jake/CLionProjects/another-mandelbrot/testing/test_server_NET/main.c

CMakeFiles/test_server_NET.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/test_server_NET.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/jake/CLionProjects/another-mandelbrot/testing/test_server_NET/main.c > CMakeFiles/test_server_NET.dir/main.c.i

CMakeFiles/test_server_NET.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/test_server_NET.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/jake/CLionProjects/another-mandelbrot/testing/test_server_NET/main.c -o CMakeFiles/test_server_NET.dir/main.c.s

CMakeFiles/test_server_NET.dir/UDP_Routines.c.o: CMakeFiles/test_server_NET.dir/flags.make
CMakeFiles/test_server_NET.dir/UDP_Routines.c.o: ../UDP_Routines.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jake/CLionProjects/another-mandelbrot/testing/test_server_NET/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/test_server_NET.dir/UDP_Routines.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/test_server_NET.dir/UDP_Routines.c.o -c /home/jake/CLionProjects/another-mandelbrot/testing/test_server_NET/UDP_Routines.c

CMakeFiles/test_server_NET.dir/UDP_Routines.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/test_server_NET.dir/UDP_Routines.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/jake/CLionProjects/another-mandelbrot/testing/test_server_NET/UDP_Routines.c > CMakeFiles/test_server_NET.dir/UDP_Routines.c.i

CMakeFiles/test_server_NET.dir/UDP_Routines.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/test_server_NET.dir/UDP_Routines.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/jake/CLionProjects/another-mandelbrot/testing/test_server_NET/UDP_Routines.c -o CMakeFiles/test_server_NET.dir/UDP_Routines.c.s

# Object files for target test_server_NET
test_server_NET_OBJECTS = \
"CMakeFiles/test_server_NET.dir/main.c.o" \
"CMakeFiles/test_server_NET.dir/UDP_Routines.c.o"

# External object files for target test_server_NET
test_server_NET_EXTERNAL_OBJECTS =

test_server_NET: CMakeFiles/test_server_NET.dir/main.c.o
test_server_NET: CMakeFiles/test_server_NET.dir/UDP_Routines.c.o
test_server_NET: CMakeFiles/test_server_NET.dir/build.make
test_server_NET: CMakeFiles/test_server_NET.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jake/CLionProjects/another-mandelbrot/testing/test_server_NET/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable test_server_NET"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_server_NET.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test_server_NET.dir/build: test_server_NET

.PHONY : CMakeFiles/test_server_NET.dir/build

CMakeFiles/test_server_NET.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test_server_NET.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test_server_NET.dir/clean

CMakeFiles/test_server_NET.dir/depend:
	cd /home/jake/CLionProjects/another-mandelbrot/testing/test_server_NET/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jake/CLionProjects/another-mandelbrot/testing/test_server_NET /home/jake/CLionProjects/another-mandelbrot/testing/test_server_NET /home/jake/CLionProjects/another-mandelbrot/testing/test_server_NET/cmake-build-debug /home/jake/CLionProjects/another-mandelbrot/testing/test_server_NET/cmake-build-debug /home/jake/CLionProjects/another-mandelbrot/testing/test_server_NET/cmake-build-debug/CMakeFiles/test_server_NET.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/test_server_NET.dir/depend

