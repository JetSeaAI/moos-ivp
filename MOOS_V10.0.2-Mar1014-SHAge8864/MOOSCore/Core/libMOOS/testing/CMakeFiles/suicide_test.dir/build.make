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
include Core/libMOOS/testing/CMakeFiles/suicide_test.dir/depend.make

# Include the progress variables for this target.
include Core/libMOOS/testing/CMakeFiles/suicide_test.dir/progress.make

# Include the compile flags for this target's objects.
include Core/libMOOS/testing/CMakeFiles/suicide_test.dir/flags.make

Core/libMOOS/testing/CMakeFiles/suicide_test.dir/SuicideTest.cpp.o: Core/libMOOS/testing/CMakeFiles/suicide_test.dir/flags.make
Core/libMOOS/testing/CMakeFiles/suicide_test.dir/SuicideTest.cpp.o: Core/libMOOS/testing/SuicideTest.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/mikerb/Research/moos-ivp/trunk/MOOS/MOOSCoreMar2413/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object Core/libMOOS/testing/CMakeFiles/suicide_test.dir/SuicideTest.cpp.o"
	cd /Users/mikerb/Research/moos-ivp/trunk/MOOS/MOOSCoreMar2413/Core/libMOOS/testing && /usr/bin/g++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/suicide_test.dir/SuicideTest.cpp.o -c /Users/mikerb/Research/moos-ivp/trunk/MOOS/MOOSCoreMar2413/Core/libMOOS/testing/SuicideTest.cpp

Core/libMOOS/testing/CMakeFiles/suicide_test.dir/SuicideTest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/suicide_test.dir/SuicideTest.cpp.i"
	cd /Users/mikerb/Research/moos-ivp/trunk/MOOS/MOOSCoreMar2413/Core/libMOOS/testing && /usr/bin/g++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/mikerb/Research/moos-ivp/trunk/MOOS/MOOSCoreMar2413/Core/libMOOS/testing/SuicideTest.cpp > CMakeFiles/suicide_test.dir/SuicideTest.cpp.i

Core/libMOOS/testing/CMakeFiles/suicide_test.dir/SuicideTest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/suicide_test.dir/SuicideTest.cpp.s"
	cd /Users/mikerb/Research/moos-ivp/trunk/MOOS/MOOSCoreMar2413/Core/libMOOS/testing && /usr/bin/g++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/mikerb/Research/moos-ivp/trunk/MOOS/MOOSCoreMar2413/Core/libMOOS/testing/SuicideTest.cpp -o CMakeFiles/suicide_test.dir/SuicideTest.cpp.s

Core/libMOOS/testing/CMakeFiles/suicide_test.dir/SuicideTest.cpp.o.requires:
.PHONY : Core/libMOOS/testing/CMakeFiles/suicide_test.dir/SuicideTest.cpp.o.requires

Core/libMOOS/testing/CMakeFiles/suicide_test.dir/SuicideTest.cpp.o.provides: Core/libMOOS/testing/CMakeFiles/suicide_test.dir/SuicideTest.cpp.o.requires
	$(MAKE) -f Core/libMOOS/testing/CMakeFiles/suicide_test.dir/build.make Core/libMOOS/testing/CMakeFiles/suicide_test.dir/SuicideTest.cpp.o.provides.build
.PHONY : Core/libMOOS/testing/CMakeFiles/suicide_test.dir/SuicideTest.cpp.o.provides

Core/libMOOS/testing/CMakeFiles/suicide_test.dir/SuicideTest.cpp.o.provides.build: Core/libMOOS/testing/CMakeFiles/suicide_test.dir/SuicideTest.cpp.o

# Object files for target suicide_test
suicide_test_OBJECTS = \
"CMakeFiles/suicide_test.dir/SuicideTest.cpp.o"

# External object files for target suicide_test
suicide_test_EXTERNAL_OBJECTS =

/Users/mikerb/Research/moos-ivp/trunk/bin/tests/lib/suicide_test: Core/libMOOS/testing/CMakeFiles/suicide_test.dir/SuicideTest.cpp.o
/Users/mikerb/Research/moos-ivp/trunk/bin/tests/lib/suicide_test: Core/libMOOS/testing/CMakeFiles/suicide_test.dir/build.make
/Users/mikerb/Research/moos-ivp/trunk/bin/tests/lib/suicide_test: lib/libMOOS.a
/Users/mikerb/Research/moos-ivp/trunk/bin/tests/lib/suicide_test: Core/libMOOS/testing/CMakeFiles/suicide_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable /Users/mikerb/Research/moos-ivp/trunk/bin/tests/lib/suicide_test"
	cd /Users/mikerb/Research/moos-ivp/trunk/MOOS/MOOSCoreMar2413/Core/libMOOS/testing && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/suicide_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Core/libMOOS/testing/CMakeFiles/suicide_test.dir/build: /Users/mikerb/Research/moos-ivp/trunk/bin/tests/lib/suicide_test
.PHONY : Core/libMOOS/testing/CMakeFiles/suicide_test.dir/build

Core/libMOOS/testing/CMakeFiles/suicide_test.dir/requires: Core/libMOOS/testing/CMakeFiles/suicide_test.dir/SuicideTest.cpp.o.requires
.PHONY : Core/libMOOS/testing/CMakeFiles/suicide_test.dir/requires

Core/libMOOS/testing/CMakeFiles/suicide_test.dir/clean:
	cd /Users/mikerb/Research/moos-ivp/trunk/MOOS/MOOSCoreMar2413/Core/libMOOS/testing && $(CMAKE_COMMAND) -P CMakeFiles/suicide_test.dir/cmake_clean.cmake
.PHONY : Core/libMOOS/testing/CMakeFiles/suicide_test.dir/clean

Core/libMOOS/testing/CMakeFiles/suicide_test.dir/depend:
	cd /Users/mikerb/Research/moos-ivp/trunk/MOOS/MOOSCoreMar2413 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/mikerb/Research/moos-ivp/trunk/MOOS/MOOSCoreMar2413 /Users/mikerb/Research/moos-ivp/trunk/MOOS/MOOSCoreMar2413/Core/libMOOS/testing /Users/mikerb/Research/moos-ivp/trunk/MOOS/MOOSCoreMar2413 /Users/mikerb/Research/moos-ivp/trunk/MOOS/MOOSCoreMar2413/Core/libMOOS/testing /Users/mikerb/Research/moos-ivp/trunk/MOOS/MOOSCoreMar2413/Core/libMOOS/testing/CMakeFiles/suicide_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Core/libMOOS/testing/CMakeFiles/suicide_test.dir/depend

