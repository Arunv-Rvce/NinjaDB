# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

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
CMAKE_COMMAND = /snap/clion/88/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/88/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/arun/CLionProjects/NinjaDB

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/arun/CLionProjects/NinjaDB/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/NinjaDB.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/NinjaDB.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/NinjaDB.dir/flags.make

CMakeFiles/NinjaDB.dir/main.c.o: CMakeFiles/NinjaDB.dir/flags.make
CMakeFiles/NinjaDB.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/arun/CLionProjects/NinjaDB/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/NinjaDB.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/NinjaDB.dir/main.c.o   -c /home/arun/CLionProjects/NinjaDB/main.c

CMakeFiles/NinjaDB.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/NinjaDB.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/arun/CLionProjects/NinjaDB/main.c > CMakeFiles/NinjaDB.dir/main.c.i

CMakeFiles/NinjaDB.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/NinjaDB.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/arun/CLionProjects/NinjaDB/main.c -o CMakeFiles/NinjaDB.dir/main.c.s

CMakeFiles/NinjaDB.dir/constants.c.o: CMakeFiles/NinjaDB.dir/flags.make
CMakeFiles/NinjaDB.dir/constants.c.o: ../constants.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/arun/CLionProjects/NinjaDB/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/NinjaDB.dir/constants.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/NinjaDB.dir/constants.c.o   -c /home/arun/CLionProjects/NinjaDB/constants.c

CMakeFiles/NinjaDB.dir/constants.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/NinjaDB.dir/constants.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/arun/CLionProjects/NinjaDB/constants.c > CMakeFiles/NinjaDB.dir/constants.c.i

CMakeFiles/NinjaDB.dir/constants.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/NinjaDB.dir/constants.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/arun/CLionProjects/NinjaDB/constants.c -o CMakeFiles/NinjaDB.dir/constants.c.s

CMakeFiles/NinjaDB.dir/insert.c.o: CMakeFiles/NinjaDB.dir/flags.make
CMakeFiles/NinjaDB.dir/insert.c.o: ../insert.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/arun/CLionProjects/NinjaDB/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/NinjaDB.dir/insert.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/NinjaDB.dir/insert.c.o   -c /home/arun/CLionProjects/NinjaDB/insert.c

CMakeFiles/NinjaDB.dir/insert.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/NinjaDB.dir/insert.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/arun/CLionProjects/NinjaDB/insert.c > CMakeFiles/NinjaDB.dir/insert.c.i

CMakeFiles/NinjaDB.dir/insert.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/NinjaDB.dir/insert.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/arun/CLionProjects/NinjaDB/insert.c -o CMakeFiles/NinjaDB.dir/insert.c.s

CMakeFiles/NinjaDB.dir/fileOperations.c.o: CMakeFiles/NinjaDB.dir/flags.make
CMakeFiles/NinjaDB.dir/fileOperations.c.o: ../fileOperations.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/arun/CLionProjects/NinjaDB/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/NinjaDB.dir/fileOperations.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/NinjaDB.dir/fileOperations.c.o   -c /home/arun/CLionProjects/NinjaDB/fileOperations.c

CMakeFiles/NinjaDB.dir/fileOperations.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/NinjaDB.dir/fileOperations.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/arun/CLionProjects/NinjaDB/fileOperations.c > CMakeFiles/NinjaDB.dir/fileOperations.c.i

CMakeFiles/NinjaDB.dir/fileOperations.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/NinjaDB.dir/fileOperations.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/arun/CLionProjects/NinjaDB/fileOperations.c -o CMakeFiles/NinjaDB.dir/fileOperations.c.s

CMakeFiles/NinjaDB.dir/db.c.o: CMakeFiles/NinjaDB.dir/flags.make
CMakeFiles/NinjaDB.dir/db.c.o: ../db.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/arun/CLionProjects/NinjaDB/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/NinjaDB.dir/db.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/NinjaDB.dir/db.c.o   -c /home/arun/CLionProjects/NinjaDB/db.c

CMakeFiles/NinjaDB.dir/db.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/NinjaDB.dir/db.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/arun/CLionProjects/NinjaDB/db.c > CMakeFiles/NinjaDB.dir/db.c.i

CMakeFiles/NinjaDB.dir/db.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/NinjaDB.dir/db.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/arun/CLionProjects/NinjaDB/db.c -o CMakeFiles/NinjaDB.dir/db.c.s

# Object files for target NinjaDB
NinjaDB_OBJECTS = \
"CMakeFiles/NinjaDB.dir/main.c.o" \
"CMakeFiles/NinjaDB.dir/constants.c.o" \
"CMakeFiles/NinjaDB.dir/insert.c.o" \
"CMakeFiles/NinjaDB.dir/fileOperations.c.o" \
"CMakeFiles/NinjaDB.dir/db.c.o"

# External object files for target NinjaDB
NinjaDB_EXTERNAL_OBJECTS =

NinjaDB: CMakeFiles/NinjaDB.dir/main.c.o
NinjaDB: CMakeFiles/NinjaDB.dir/constants.c.o
NinjaDB: CMakeFiles/NinjaDB.dir/insert.c.o
NinjaDB: CMakeFiles/NinjaDB.dir/fileOperations.c.o
NinjaDB: CMakeFiles/NinjaDB.dir/db.c.o
NinjaDB: CMakeFiles/NinjaDB.dir/build.make
NinjaDB: CMakeFiles/NinjaDB.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/arun/CLionProjects/NinjaDB/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking C executable NinjaDB"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/NinjaDB.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/NinjaDB.dir/build: NinjaDB

.PHONY : CMakeFiles/NinjaDB.dir/build

CMakeFiles/NinjaDB.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/NinjaDB.dir/cmake_clean.cmake
.PHONY : CMakeFiles/NinjaDB.dir/clean

CMakeFiles/NinjaDB.dir/depend:
	cd /home/arun/CLionProjects/NinjaDB/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/arun/CLionProjects/NinjaDB /home/arun/CLionProjects/NinjaDB /home/arun/CLionProjects/NinjaDB/cmake-build-debug /home/arun/CLionProjects/NinjaDB/cmake-build-debug /home/arun/CLionProjects/NinjaDB/cmake-build-debug/CMakeFiles/NinjaDB.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/NinjaDB.dir/depend

