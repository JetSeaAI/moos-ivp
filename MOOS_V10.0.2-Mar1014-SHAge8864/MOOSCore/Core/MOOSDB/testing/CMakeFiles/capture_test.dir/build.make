# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_COMMAND = /opt/local/bin/cmake

# The command to remove a file.
RM = /opt/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /opt/local/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/mikerb/Research/moos-ivp/trunk/MOOS/MOOSCoreMar2413

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/mikerb/Research/moos-ivp/trunk/MOOS/MOOSCoreMar2413

# Include any dependencies generated for this target.
include Core/MOOSDB/testing/CMakeFiles/capture_test.dir/depend.make

# Include the progress variables for this target.
include Core/MOOSDB/testing/CMakeFiles/capture_test.dir/progress.make

# Include the compile flags for this target's objects.
include Core/MOOSDB/testing/CMakeFiles/capture_test.dir/flags.make

Core/MOOSDB/testing/CMakeFiles/capture_test.dir/CaptureTest.cpp.o: Core/MOOSDB/testing/CMakeFiles/capture_test.dir/flags.make
Core/MOOSDB/testing/CMakeFiles/capture_test.dir/CaptureTest.cpp.o: Core/MOOSDB/testing/CaptureTest.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/mikerb/Research/moos-ivp/trunk/MOOS/MOOSCoreMar2413/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object Core/MOOSDB/testing/CMakeFiles/capture_test.dir/CaptureTest.cpp.o"
	cd /Users/mikerb/Research/moos-ivp/trunk/MOOS/MOOSCoreMar2413/Core/MOOSDB/testing && /usr/bin/g++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/capture_test.dir/CaptureTest.cpp.o -c /Users/mikerb/Research/moos-ivp/trunk/MOOS/MOOSCoreMar2413/Core/MOOSDB/testing/CaptureTest.cpp

Core/MOOSDB/testing/CMakeFiles/capture_test.dir/CaptureTest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/capture_test.dir/CaptureTest.cpp.i"
	cd /Users/mikerb/Research/moos-ivp/trunk/MOOS/MOOSCoreMar2413/Core/MOOSDB/testing && /usr/bin/g++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/mikerb/Research/moos-ivp/trunk/MOOS/MOOSCoreMar2413/Core/MOOSDB/testing/CaptureTest.cpp > CMakeFiles/capture_test.dir/CaptureTest.cpp.i

Core/MOOSDB/testing/CMakeFiles/capture_test.dir/CaptureTest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/capture_test.dir/CaptureTest.cpp.s"
	cd /Users/mikerb/Research/moos-ivp/trunk/MOOS/MOOSCoreMar2413/Core/MOOSDB/testing && /usr/bin/g++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/mikerb/Research/moos-ivp/trunk/MOOS/MOOSCoreMar2413/Core/MOOSDB/testing/CaptureTest.cpp -o CMakeFiles/capture_test.dir/CaptureTest.cpp.s

Core/MOOSDB/testing/CMakeFiles/capture_test.dir/CaptureTest.cpp.o.requires:
.PHONY : Core/MOOSDB/testing/CMakeFiles/capture_test.dir/CaptureTest.cpp.o.requires

Core/MOOSDB/testing/CMakeFiles/capture_test.dir/CaptureTest.cpp.o.provides: Core/MOOSDB/testing/CMakeFiles/capture_test.dir/CaptureTest.cpp.o.requires
	$(MAKE) -f Core/MOOSDB/testing/CMakeFiles/capture_test.dir/build.make Core/MOOSDB/testing/CMakeFiles/capture_test.dir/CaptureTest.cpp.o.provides.build
.PHONY : Core/MOOSDB/testing/CMakeFiles/capture_test.dir/CaptureTest.cpp.o.provides

Core/MOOSDB/testing/CMakeFiles/capture_test.dir/CaptureTest.cpp.o.provides.build: Core/MOOSDB/testing/CMakeFiles/capture_test.dir/CaptureTest.cpp.o

# Object files for target capture_test
capture_test_OBJECTS = \
"CMakeFiles/capture_test.dir/CaptureTest.cpp.o"

# External object files for target capture_test
capture_test_EXTERNAL_OBJECTS =

/Users/mikerb/Research/moos-ivp/trunk/bin/tests/capture_test: Core/MOOSDB/testing/CMakeFiles/capture_test.dir/CaptureTest.cpp.o
/Users/mikerb/Research/moos-ivp/trunk/bin/tests/capture_test: Core/MOOSDB/testing/CMakeFiles/capture_test.dir/build.make
/Users/mikerb/Research/moos-ivp/trunk/bin/tests/capture_test: lib/libMOOS.a
/Users/mikerb/Research/moos-ivp/trunk/bin/tests/capture_test: Core/MOOSDB/testing/CMakeFiles/capture_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable /Users/mikerb/Research/moos-ivp/trunk/bin/tests/capture_test"
	cd /Users/mikerb/Research/moos-ivp/trunk/MOOS/MOOSCoreMar2413/Core/MOOSDB/testing && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/capture_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Core/MOOSDB/testing/CMakeFiles/capture_test.dir/build: /Users/mikerb/Research/moos-ivp/trunk/bin/tests/capture_test
.PHONY : Core/MOOSDB/testing/CMakeFiles/capture_test.dir/build

Core/MOOSDB/testing/CMakeFiles/capture_test.dir/requires: Core/MOOSDB/testing/CMakeFiles/capture_test.dir/CaptureTest.cpp.o.requires
.PHONY : Core/MOOSDB/testing/CMakeFiles/capture_test.dir/requires

Core/MOOSDB/testing/CMakeFiles/capture_test.dir/clean:
	cd /Users/mikerb/Research/moos-ivp/trunk/MOOS/MOOSCoreMar2413/Core/MOOSDB/testing && $(CMAKE_COMMAND) -P CMakeFiles/capture_test.dir/cmake_clean.cmake
.PHONY : Core/MOOSDB/testing/CMakeFiles/capture_test.dir/clean

Core/MOOSDB/testing/CMakeFiles/capture_test.dir/depend:
	cd /Users/mikerb/Research/moos-ivp/trunk/MOOS/MOOSCoreMar2413 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/mikerb/Research/moos-ivp/trunk/MOOS/MOOSCoreMar2413 /Users/mikerb/Research/moos-ivp/trunk/MOOS/MOOSCoreMar2413/Core/MOOSDB/testing /Users/mikerb/Research/moos-ivp/trunk/MOOS/MOOSCoreMar2413 /Users/mikerb/Research/moos-ivp/trunk/MOOS/MOOSCoreMar2413/Core/MOOSDB/testing /Users/mikerb/Research/moos-ivp/trunk/MOOS/MOOSCoreMar2413/Core/MOOSDB/testing/CMakeFiles/capture_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Core/MOOSDB/testing/CMakeFiles/capture_test.dir/depend

