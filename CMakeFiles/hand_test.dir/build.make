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
CMAKE_SOURCE_DIR = /home/astronomax/cdcl-sat-solver

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/astronomax/cdcl-sat-solver

# Include any dependencies generated for this target.
include CMakeFiles/hand_test.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/hand_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/hand_test.dir/flags.make

CMakeFiles/hand_test.dir/src/Entities.cpp.o: CMakeFiles/hand_test.dir/flags.make
CMakeFiles/hand_test.dir/src/Entities.cpp.o: src/Entities.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/astronomax/cdcl-sat-solver/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/hand_test.dir/src/Entities.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/hand_test.dir/src/Entities.cpp.o -c /home/astronomax/cdcl-sat-solver/src/Entities.cpp

CMakeFiles/hand_test.dir/src/Entities.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/hand_test.dir/src/Entities.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/astronomax/cdcl-sat-solver/src/Entities.cpp > CMakeFiles/hand_test.dir/src/Entities.cpp.i

CMakeFiles/hand_test.dir/src/Entities.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/hand_test.dir/src/Entities.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/astronomax/cdcl-sat-solver/src/Entities.cpp -o CMakeFiles/hand_test.dir/src/Entities.cpp.s

CMakeFiles/hand_test.dir/src/CDCL.cpp.o: CMakeFiles/hand_test.dir/flags.make
CMakeFiles/hand_test.dir/src/CDCL.cpp.o: src/CDCL.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/astronomax/cdcl-sat-solver/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/hand_test.dir/src/CDCL.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/hand_test.dir/src/CDCL.cpp.o -c /home/astronomax/cdcl-sat-solver/src/CDCL.cpp

CMakeFiles/hand_test.dir/src/CDCL.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/hand_test.dir/src/CDCL.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/astronomax/cdcl-sat-solver/src/CDCL.cpp > CMakeFiles/hand_test.dir/src/CDCL.cpp.i

CMakeFiles/hand_test.dir/src/CDCL.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/hand_test.dir/src/CDCL.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/astronomax/cdcl-sat-solver/src/CDCL.cpp -o CMakeFiles/hand_test.dir/src/CDCL.cpp.s

CMakeFiles/hand_test.dir/src/submain.cpp.o: CMakeFiles/hand_test.dir/flags.make
CMakeFiles/hand_test.dir/src/submain.cpp.o: src/submain.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/astronomax/cdcl-sat-solver/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/hand_test.dir/src/submain.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/hand_test.dir/src/submain.cpp.o -c /home/astronomax/cdcl-sat-solver/src/submain.cpp

CMakeFiles/hand_test.dir/src/submain.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/hand_test.dir/src/submain.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/astronomax/cdcl-sat-solver/src/submain.cpp > CMakeFiles/hand_test.dir/src/submain.cpp.i

CMakeFiles/hand_test.dir/src/submain.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/hand_test.dir/src/submain.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/astronomax/cdcl-sat-solver/src/submain.cpp -o CMakeFiles/hand_test.dir/src/submain.cpp.s

# Object files for target hand_test
hand_test_OBJECTS = \
"CMakeFiles/hand_test.dir/src/Entities.cpp.o" \
"CMakeFiles/hand_test.dir/src/CDCL.cpp.o" \
"CMakeFiles/hand_test.dir/src/submain.cpp.o"

# External object files for target hand_test
hand_test_EXTERNAL_OBJECTS =

hand_test: CMakeFiles/hand_test.dir/src/Entities.cpp.o
hand_test: CMakeFiles/hand_test.dir/src/CDCL.cpp.o
hand_test: CMakeFiles/hand_test.dir/src/submain.cpp.o
hand_test: CMakeFiles/hand_test.dir/build.make
hand_test: CMakeFiles/hand_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/astronomax/cdcl-sat-solver/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable hand_test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/hand_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/hand_test.dir/build: hand_test

.PHONY : CMakeFiles/hand_test.dir/build

CMakeFiles/hand_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/hand_test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/hand_test.dir/clean

CMakeFiles/hand_test.dir/depend:
	cd /home/astronomax/cdcl-sat-solver && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/astronomax/cdcl-sat-solver /home/astronomax/cdcl-sat-solver /home/astronomax/cdcl-sat-solver /home/astronomax/cdcl-sat-solver /home/astronomax/cdcl-sat-solver/CMakeFiles/hand_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/hand_test.dir/depend

