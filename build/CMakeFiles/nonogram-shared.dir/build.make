# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.28

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = C:\Users\mecty\AppData\Local\Programs\CLion\bin\cmake\win\x64\bin\cmake.exe

# The command to remove a file.
RM = C:\Users\mecty\AppData\Local\Programs\CLion\bin\cmake\win\x64\bin\cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\PROJET_C\nonogram_C\src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\PROJET_C\nonogram_C\build

# Include any dependencies generated for this target.
include CMakeFiles/nonogram-shared.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/nonogram-shared.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/nonogram-shared.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/nonogram-shared.dir/flags.make

CMakeFiles/nonogram-shared.dir/nonogram.c.obj: CMakeFiles/nonogram-shared.dir/flags.make
CMakeFiles/nonogram-shared.dir/nonogram.c.obj: D:/PROJET_C/nonogram_C/src/nonogram.c
CMakeFiles/nonogram-shared.dir/nonogram.c.obj: CMakeFiles/nonogram-shared.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=D:\PROJET_C\nonogram_C\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/nonogram-shared.dir/nonogram.c.obj"
	C:\Users\mecty\AppData\Local\Programs\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/nonogram-shared.dir/nonogram.c.obj -MF CMakeFiles\nonogram-shared.dir\nonogram.c.obj.d -o CMakeFiles\nonogram-shared.dir\nonogram.c.obj -c D:\PROJET_C\nonogram_C\src\nonogram.c

CMakeFiles/nonogram-shared.dir/nonogram.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/nonogram-shared.dir/nonogram.c.i"
	C:\Users\mecty\AppData\Local\Programs\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E D:\PROJET_C\nonogram_C\src\nonogram.c > CMakeFiles\nonogram-shared.dir\nonogram.c.i

CMakeFiles/nonogram-shared.dir/nonogram.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/nonogram-shared.dir/nonogram.c.s"
	C:\Users\mecty\AppData\Local\Programs\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S D:\PROJET_C\nonogram_C\src\nonogram.c -o CMakeFiles\nonogram-shared.dir\nonogram.c.s

CMakeFiles/nonogram-shared.dir/cJSON.c.obj: CMakeFiles/nonogram-shared.dir/flags.make
CMakeFiles/nonogram-shared.dir/cJSON.c.obj: D:/PROJET_C/nonogram_C/src/cJSON.c
CMakeFiles/nonogram-shared.dir/cJSON.c.obj: CMakeFiles/nonogram-shared.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=D:\PROJET_C\nonogram_C\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/nonogram-shared.dir/cJSON.c.obj"
	C:\Users\mecty\AppData\Local\Programs\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/nonogram-shared.dir/cJSON.c.obj -MF CMakeFiles\nonogram-shared.dir\cJSON.c.obj.d -o CMakeFiles\nonogram-shared.dir\cJSON.c.obj -c D:\PROJET_C\nonogram_C\src\cJSON.c

CMakeFiles/nonogram-shared.dir/cJSON.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/nonogram-shared.dir/cJSON.c.i"
	C:\Users\mecty\AppData\Local\Programs\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E D:\PROJET_C\nonogram_C\src\cJSON.c > CMakeFiles\nonogram-shared.dir\cJSON.c.i

CMakeFiles/nonogram-shared.dir/cJSON.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/nonogram-shared.dir/cJSON.c.s"
	C:\Users\mecty\AppData\Local\Programs\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S D:\PROJET_C\nonogram_C\src\cJSON.c -o CMakeFiles\nonogram-shared.dir\cJSON.c.s

CMakeFiles/nonogram-shared.dir/pnmio.c.obj: CMakeFiles/nonogram-shared.dir/flags.make
CMakeFiles/nonogram-shared.dir/pnmio.c.obj: D:/PROJET_C/nonogram_C/src/pnmio.c
CMakeFiles/nonogram-shared.dir/pnmio.c.obj: CMakeFiles/nonogram-shared.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=D:\PROJET_C\nonogram_C\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/nonogram-shared.dir/pnmio.c.obj"
	C:\Users\mecty\AppData\Local\Programs\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/nonogram-shared.dir/pnmio.c.obj -MF CMakeFiles\nonogram-shared.dir\pnmio.c.obj.d -o CMakeFiles\nonogram-shared.dir\pnmio.c.obj -c D:\PROJET_C\nonogram_C\src\pnmio.c

CMakeFiles/nonogram-shared.dir/pnmio.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/nonogram-shared.dir/pnmio.c.i"
	C:\Users\mecty\AppData\Local\Programs\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E D:\PROJET_C\nonogram_C\src\pnmio.c > CMakeFiles\nonogram-shared.dir\pnmio.c.i

CMakeFiles/nonogram-shared.dir/pnmio.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/nonogram-shared.dir/pnmio.c.s"
	C:\Users\mecty\AppData\Local\Programs\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S D:\PROJET_C\nonogram_C\src\pnmio.c -o CMakeFiles\nonogram-shared.dir\pnmio.c.s

# Object files for target nonogram-shared
nonogram__shared_OBJECTS = \
"CMakeFiles/nonogram-shared.dir/nonogram.c.obj" \
"CMakeFiles/nonogram-shared.dir/cJSON.c.obj" \
"CMakeFiles/nonogram-shared.dir/pnmio.c.obj"

# External object files for target nonogram-shared
nonogram__shared_EXTERNAL_OBJECTS =

libnonogram.dll: CMakeFiles/nonogram-shared.dir/nonogram.c.obj
libnonogram.dll: CMakeFiles/nonogram-shared.dir/cJSON.c.obj
libnonogram.dll: CMakeFiles/nonogram-shared.dir/pnmio.c.obj
libnonogram.dll: CMakeFiles/nonogram-shared.dir/build.make
libnonogram.dll: CMakeFiles/nonogram-shared.dir/linkLibs.rsp
libnonogram.dll: CMakeFiles/nonogram-shared.dir/objects1.rsp
libnonogram.dll: CMakeFiles/nonogram-shared.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=D:\PROJET_C\nonogram_C\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C shared library libnonogram.dll"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\nonogram-shared.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/nonogram-shared.dir/build: libnonogram.dll
.PHONY : CMakeFiles/nonogram-shared.dir/build

CMakeFiles/nonogram-shared.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\nonogram-shared.dir\cmake_clean.cmake
.PHONY : CMakeFiles/nonogram-shared.dir/clean

CMakeFiles/nonogram-shared.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\PROJET_C\nonogram_C\src D:\PROJET_C\nonogram_C\src D:\PROJET_C\nonogram_C\build D:\PROJET_C\nonogram_C\build D:\PROJET_C\nonogram_C\build\CMakeFiles\nonogram-shared.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/nonogram-shared.dir/depend

