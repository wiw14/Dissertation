# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

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
CMAKE_COMMAND = /cygdrive/c/Users/wmw13/AppData/Local/JetBrains/CLion2020.3/cygwin_cmake/bin/cmake.exe

# The command to remove a file.
RM = /cygdrive/c/Users/wmw13/AppData/Local/JetBrains/CLion2020.3/cygwin_cmake/bin/cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/cygdrive/c/Users/wmw13/Documents/GitHub/Dissertation/AntColonyOptimisation -- Example"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/cygdrive/c/Users/wmw13/Documents/GitHub/Dissertation/AntColonyOptimisation -- Example/cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/AntColonyOptimisation.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/AntColonyOptimisation.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/AntColonyOptimisation.dir/flags.make

CMakeFiles/AntColonyOptimisation.dir/main.cpp.o: CMakeFiles/AntColonyOptimisation.dir/flags.make
CMakeFiles/AntColonyOptimisation.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/cygdrive/c/Users/wmw13/Documents/GitHub/Dissertation/AntColonyOptimisation -- Example/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/AntColonyOptimisation.dir/main.cpp.o"
	/usr/bin/c++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/AntColonyOptimisation.dir/main.cpp.o -c "/cygdrive/c/Users/wmw13/Documents/GitHub/Dissertation/AntColonyOptimisation -- Example/main.cpp"

CMakeFiles/AntColonyOptimisation.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/AntColonyOptimisation.dir/main.cpp.i"
	/usr/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/cygdrive/c/Users/wmw13/Documents/GitHub/Dissertation/AntColonyOptimisation -- Example/main.cpp" > CMakeFiles/AntColonyOptimisation.dir/main.cpp.i

CMakeFiles/AntColonyOptimisation.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/AntColonyOptimisation.dir/main.cpp.s"
	/usr/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/cygdrive/c/Users/wmw13/Documents/GitHub/Dissertation/AntColonyOptimisation -- Example/main.cpp" -o CMakeFiles/AntColonyOptimisation.dir/main.cpp.s

CMakeFiles/AntColonyOptimisation.dir/ACO.cpp.o: CMakeFiles/AntColonyOptimisation.dir/flags.make
CMakeFiles/AntColonyOptimisation.dir/ACO.cpp.o: ../ACO.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/cygdrive/c/Users/wmw13/Documents/GitHub/Dissertation/AntColonyOptimisation -- Example/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/AntColonyOptimisation.dir/ACO.cpp.o"
	/usr/bin/c++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/AntColonyOptimisation.dir/ACO.cpp.o -c "/cygdrive/c/Users/wmw13/Documents/GitHub/Dissertation/AntColonyOptimisation -- Example/ACO.cpp"

CMakeFiles/AntColonyOptimisation.dir/ACO.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/AntColonyOptimisation.dir/ACO.cpp.i"
	/usr/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/cygdrive/c/Users/wmw13/Documents/GitHub/Dissertation/AntColonyOptimisation -- Example/ACO.cpp" > CMakeFiles/AntColonyOptimisation.dir/ACO.cpp.i

CMakeFiles/AntColonyOptimisation.dir/ACO.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/AntColonyOptimisation.dir/ACO.cpp.s"
	/usr/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/cygdrive/c/Users/wmw13/Documents/GitHub/Dissertation/AntColonyOptimisation -- Example/ACO.cpp" -o CMakeFiles/AntColonyOptimisation.dir/ACO.cpp.s

CMakeFiles/AntColonyOptimisation.dir/Randoms.cpp.o: CMakeFiles/AntColonyOptimisation.dir/flags.make
CMakeFiles/AntColonyOptimisation.dir/Randoms.cpp.o: ../Randoms.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/cygdrive/c/Users/wmw13/Documents/GitHub/Dissertation/AntColonyOptimisation -- Example/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/AntColonyOptimisation.dir/Randoms.cpp.o"
	/usr/bin/c++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/AntColonyOptimisation.dir/Randoms.cpp.o -c "/cygdrive/c/Users/wmw13/Documents/GitHub/Dissertation/AntColonyOptimisation -- Example/Randoms.cpp"

CMakeFiles/AntColonyOptimisation.dir/Randoms.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/AntColonyOptimisation.dir/Randoms.cpp.i"
	/usr/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/cygdrive/c/Users/wmw13/Documents/GitHub/Dissertation/AntColonyOptimisation -- Example/Randoms.cpp" > CMakeFiles/AntColonyOptimisation.dir/Randoms.cpp.i

CMakeFiles/AntColonyOptimisation.dir/Randoms.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/AntColonyOptimisation.dir/Randoms.cpp.s"
	/usr/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/cygdrive/c/Users/wmw13/Documents/GitHub/Dissertation/AntColonyOptimisation -- Example/Randoms.cpp" -o CMakeFiles/AntColonyOptimisation.dir/Randoms.cpp.s

# Object files for target AntColonyOptimisation
AntColonyOptimisation_OBJECTS = \
"CMakeFiles/AntColonyOptimisation.dir/main.cpp.o" \
"CMakeFiles/AntColonyOptimisation.dir/ACO.cpp.o" \
"CMakeFiles/AntColonyOptimisation.dir/Randoms.cpp.o"

# External object files for target AntColonyOptimisation
AntColonyOptimisation_EXTERNAL_OBJECTS =

AntColonyOptimisation.exe: CMakeFiles/AntColonyOptimisation.dir/main.cpp.o
AntColonyOptimisation.exe: CMakeFiles/AntColonyOptimisation.dir/ACO.cpp.o
AntColonyOptimisation.exe: CMakeFiles/AntColonyOptimisation.dir/Randoms.cpp.o
AntColonyOptimisation.exe: CMakeFiles/AntColonyOptimisation.dir/build.make
AntColonyOptimisation.exe: CMakeFiles/AntColonyOptimisation.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/cygdrive/c/Users/wmw13/Documents/GitHub/Dissertation/AntColonyOptimisation -- Example/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable AntColonyOptimisation.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/AntColonyOptimisation.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/AntColonyOptimisation.dir/build: AntColonyOptimisation.exe

.PHONY : CMakeFiles/AntColonyOptimisation.dir/build

CMakeFiles/AntColonyOptimisation.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/AntColonyOptimisation.dir/cmake_clean.cmake
.PHONY : CMakeFiles/AntColonyOptimisation.dir/clean

CMakeFiles/AntColonyOptimisation.dir/depend:
	cd "/cygdrive/c/Users/wmw13/Documents/GitHub/Dissertation/AntColonyOptimisation -- Example/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/cygdrive/c/Users/wmw13/Documents/GitHub/Dissertation/AntColonyOptimisation -- Example" "/cygdrive/c/Users/wmw13/Documents/GitHub/Dissertation/AntColonyOptimisation -- Example" "/cygdrive/c/Users/wmw13/Documents/GitHub/Dissertation/AntColonyOptimisation -- Example/cmake-build-debug" "/cygdrive/c/Users/wmw13/Documents/GitHub/Dissertation/AntColonyOptimisation -- Example/cmake-build-debug" "/cygdrive/c/Users/wmw13/Documents/GitHub/Dissertation/AntColonyOptimisation -- Example/cmake-build-debug/CMakeFiles/AntColonyOptimisation.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/AntColonyOptimisation.dir/depend

