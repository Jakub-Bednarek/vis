# CMAKE generated file: DO NOT EDIT!
# Generated by "NMake Makefiles" Generator, CMake Version 3.20

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

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

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE
NULL=nul
!ENDIF
SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2021.1.1\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2021.1.1\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "C:\Users\BlackFlage\OneDrive - Politechnika Wroclawska\C++\visual"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "C:\Users\BlackFlage\OneDrive - Politechnika Wroclawska\C++\visual\cmake-build-debug"

# Include any dependencies generated for this target.
include ..\components\vendor\ImGui\CMakeFiles\ImGui.dir\depend.make
# Include the progress variables for this target.
include ..\components\vendor\ImGui\CMakeFiles\ImGui.dir\progress.make

# Include the compile flags for this target's objects.
include ..\components\vendor\ImGui\CMakeFiles\ImGui.dir\flags.make

..\components\vendor\ImGui\CMakeFiles\ImGui.dir\ImGui\imgui.cpp.obj: ..\components\vendor\ImGui\CMakeFiles\ImGui.dir\flags.make
..\components\vendor\ImGui\CMakeFiles\ImGui.dir\ImGui\imgui.cpp.obj: ..\components\vendor\ImGui\ImGui\imgui.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="C:\Users\BlackFlage\OneDrive - Politechnika Wroclawska\C++\visual\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object ../components/vendor/ImGui/CMakeFiles/ImGui.dir/ImGui/imgui.cpp.obj"
	cd C:\Users\BLACKF~1\ONEDRI~1\C__~1\visual\COMPON~1\vendor\ImGui
	C:\PROGRA~1\LLVM\bin\clang.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\ImGui.dir\ImGui\imgui.cpp.obj -c "C:\Users\BlackFlage\OneDrive - Politechnika Wroclawska\C++\visual\components\vendor\ImGui\ImGui\imgui.cpp"
	cd C:\Users\BLACKF~1\ONEDRI~1\C__~1\visual\CMAKE-~1

..\components\vendor\ImGui\CMakeFiles\ImGui.dir\ImGui\imgui.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ImGui.dir/ImGui/imgui.cpp.i"
	$(CMAKE_COMMAND) -E cmake_unimplemented_variable CMAKE_CXX_CREATE_PREPROCESSED_SOURCE

..\components\vendor\ImGui\CMakeFiles\ImGui.dir\ImGui\imgui.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ImGui.dir/ImGui/imgui.cpp.s"
	$(CMAKE_COMMAND) -E cmake_unimplemented_variable CMAKE_CXX_CREATE_ASSEMBLY_SOURCE

..\components\vendor\ImGui\CMakeFiles\ImGui.dir\ImGui\imgui_demo.cpp.obj: ..\components\vendor\ImGui\CMakeFiles\ImGui.dir\flags.make
..\components\vendor\ImGui\CMakeFiles\ImGui.dir\ImGui\imgui_demo.cpp.obj: ..\components\vendor\ImGui\ImGui\imgui_demo.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="C:\Users\BlackFlage\OneDrive - Politechnika Wroclawska\C++\visual\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object ../components/vendor/ImGui/CMakeFiles/ImGui.dir/ImGui/imgui_demo.cpp.obj"
	cd C:\Users\BLACKF~1\ONEDRI~1\C__~1\visual\COMPON~1\vendor\ImGui
	C:\PROGRA~1\LLVM\bin\clang.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\ImGui.dir\ImGui\imgui_demo.cpp.obj -c "C:\Users\BlackFlage\OneDrive - Politechnika Wroclawska\C++\visual\components\vendor\ImGui\ImGui\imgui_demo.cpp"
	cd C:\Users\BLACKF~1\ONEDRI~1\C__~1\visual\CMAKE-~1

..\components\vendor\ImGui\CMakeFiles\ImGui.dir\ImGui\imgui_demo.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ImGui.dir/ImGui/imgui_demo.cpp.i"
	$(CMAKE_COMMAND) -E cmake_unimplemented_variable CMAKE_CXX_CREATE_PREPROCESSED_SOURCE

..\components\vendor\ImGui\CMakeFiles\ImGui.dir\ImGui\imgui_demo.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ImGui.dir/ImGui/imgui_demo.cpp.s"
	$(CMAKE_COMMAND) -E cmake_unimplemented_variable CMAKE_CXX_CREATE_ASSEMBLY_SOURCE

..\components\vendor\ImGui\CMakeFiles\ImGui.dir\ImGui\imgui_draw.cpp.obj: ..\components\vendor\ImGui\CMakeFiles\ImGui.dir\flags.make
..\components\vendor\ImGui\CMakeFiles\ImGui.dir\ImGui\imgui_draw.cpp.obj: ..\components\vendor\ImGui\ImGui\imgui_draw.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="C:\Users\BlackFlage\OneDrive - Politechnika Wroclawska\C++\visual\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object ../components/vendor/ImGui/CMakeFiles/ImGui.dir/ImGui/imgui_draw.cpp.obj"
	cd C:\Users\BLACKF~1\ONEDRI~1\C__~1\visual\COMPON~1\vendor\ImGui
	C:\PROGRA~1\LLVM\bin\clang.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\ImGui.dir\ImGui\imgui_draw.cpp.obj -c "C:\Users\BlackFlage\OneDrive - Politechnika Wroclawska\C++\visual\components\vendor\ImGui\ImGui\imgui_draw.cpp"
	cd C:\Users\BLACKF~1\ONEDRI~1\C__~1\visual\CMAKE-~1

..\components\vendor\ImGui\CMakeFiles\ImGui.dir\ImGui\imgui_draw.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ImGui.dir/ImGui/imgui_draw.cpp.i"
	$(CMAKE_COMMAND) -E cmake_unimplemented_variable CMAKE_CXX_CREATE_PREPROCESSED_SOURCE

..\components\vendor\ImGui\CMakeFiles\ImGui.dir\ImGui\imgui_draw.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ImGui.dir/ImGui/imgui_draw.cpp.s"
	$(CMAKE_COMMAND) -E cmake_unimplemented_variable CMAKE_CXX_CREATE_ASSEMBLY_SOURCE

..\components\vendor\ImGui\CMakeFiles\ImGui.dir\ImGui\imgui_tables.cpp.obj: ..\components\vendor\ImGui\CMakeFiles\ImGui.dir\flags.make
..\components\vendor\ImGui\CMakeFiles\ImGui.dir\ImGui\imgui_tables.cpp.obj: ..\components\vendor\ImGui\ImGui\imgui_tables.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="C:\Users\BlackFlage\OneDrive - Politechnika Wroclawska\C++\visual\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object ../components/vendor/ImGui/CMakeFiles/ImGui.dir/ImGui/imgui_tables.cpp.obj"
	cd C:\Users\BLACKF~1\ONEDRI~1\C__~1\visual\COMPON~1\vendor\ImGui
	C:\PROGRA~1\LLVM\bin\clang.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\ImGui.dir\ImGui\imgui_tables.cpp.obj -c "C:\Users\BlackFlage\OneDrive - Politechnika Wroclawska\C++\visual\components\vendor\ImGui\ImGui\imgui_tables.cpp"
	cd C:\Users\BLACKF~1\ONEDRI~1\C__~1\visual\CMAKE-~1

..\components\vendor\ImGui\CMakeFiles\ImGui.dir\ImGui\imgui_tables.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ImGui.dir/ImGui/imgui_tables.cpp.i"
	$(CMAKE_COMMAND) -E cmake_unimplemented_variable CMAKE_CXX_CREATE_PREPROCESSED_SOURCE

..\components\vendor\ImGui\CMakeFiles\ImGui.dir\ImGui\imgui_tables.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ImGui.dir/ImGui/imgui_tables.cpp.s"
	$(CMAKE_COMMAND) -E cmake_unimplemented_variable CMAKE_CXX_CREATE_ASSEMBLY_SOURCE

..\components\vendor\ImGui\CMakeFiles\ImGui.dir\ImGui\imgui_widgets.cpp.obj: ..\components\vendor\ImGui\CMakeFiles\ImGui.dir\flags.make
..\components\vendor\ImGui\CMakeFiles\ImGui.dir\ImGui\imgui_widgets.cpp.obj: ..\components\vendor\ImGui\ImGui\imgui_widgets.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="C:\Users\BlackFlage\OneDrive - Politechnika Wroclawska\C++\visual\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object ../components/vendor/ImGui/CMakeFiles/ImGui.dir/ImGui/imgui_widgets.cpp.obj"
	cd C:\Users\BLACKF~1\ONEDRI~1\C__~1\visual\COMPON~1\vendor\ImGui
	C:\PROGRA~1\LLVM\bin\clang.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\ImGui.dir\ImGui\imgui_widgets.cpp.obj -c "C:\Users\BlackFlage\OneDrive - Politechnika Wroclawska\C++\visual\components\vendor\ImGui\ImGui\imgui_widgets.cpp"
	cd C:\Users\BLACKF~1\ONEDRI~1\C__~1\visual\CMAKE-~1

..\components\vendor\ImGui\CMakeFiles\ImGui.dir\ImGui\imgui_widgets.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ImGui.dir/ImGui/imgui_widgets.cpp.i"
	$(CMAKE_COMMAND) -E cmake_unimplemented_variable CMAKE_CXX_CREATE_PREPROCESSED_SOURCE

..\components\vendor\ImGui\CMakeFiles\ImGui.dir\ImGui\imgui_widgets.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ImGui.dir/ImGui/imgui_widgets.cpp.s"
	$(CMAKE_COMMAND) -E cmake_unimplemented_variable CMAKE_CXX_CREATE_ASSEMBLY_SOURCE

..\components\vendor\ImGui\CMakeFiles\ImGui.dir\ImGui\imgui_impl_opengl3.cpp.obj: ..\components\vendor\ImGui\CMakeFiles\ImGui.dir\flags.make
..\components\vendor\ImGui\CMakeFiles\ImGui.dir\ImGui\imgui_impl_opengl3.cpp.obj: ..\components\vendor\ImGui\ImGui\imgui_impl_opengl3.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="C:\Users\BlackFlage\OneDrive - Politechnika Wroclawska\C++\visual\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object ../components/vendor/ImGui/CMakeFiles/ImGui.dir/ImGui/imgui_impl_opengl3.cpp.obj"
	cd C:\Users\BLACKF~1\ONEDRI~1\C__~1\visual\COMPON~1\vendor\ImGui
	C:\PROGRA~1\LLVM\bin\clang.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\ImGui.dir\ImGui\imgui_impl_opengl3.cpp.obj -c "C:\Users\BlackFlage\OneDrive - Politechnika Wroclawska\C++\visual\components\vendor\ImGui\ImGui\imgui_impl_opengl3.cpp"
	cd C:\Users\BLACKF~1\ONEDRI~1\C__~1\visual\CMAKE-~1

..\components\vendor\ImGui\CMakeFiles\ImGui.dir\ImGui\imgui_impl_opengl3.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ImGui.dir/ImGui/imgui_impl_opengl3.cpp.i"
	$(CMAKE_COMMAND) -E cmake_unimplemented_variable CMAKE_CXX_CREATE_PREPROCESSED_SOURCE

..\components\vendor\ImGui\CMakeFiles\ImGui.dir\ImGui\imgui_impl_opengl3.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ImGui.dir/ImGui/imgui_impl_opengl3.cpp.s"
	$(CMAKE_COMMAND) -E cmake_unimplemented_variable CMAKE_CXX_CREATE_ASSEMBLY_SOURCE

# Object files for target ImGui
ImGui_OBJECTS = \
"CMakeFiles\ImGui.dir\ImGui\imgui.cpp.obj" \
"CMakeFiles\ImGui.dir\ImGui\imgui_demo.cpp.obj" \
"CMakeFiles\ImGui.dir\ImGui\imgui_draw.cpp.obj" \
"CMakeFiles\ImGui.dir\ImGui\imgui_tables.cpp.obj" \
"CMakeFiles\ImGui.dir\ImGui\imgui_widgets.cpp.obj" \
"CMakeFiles\ImGui.dir\ImGui\imgui_impl_opengl3.cpp.obj"

# External object files for target ImGui
ImGui_EXTERNAL_OBJECTS =

..\components\vendor\ImGui\ImGui.lib: ..\components\vendor\ImGui\CMakeFiles\ImGui.dir\ImGui\imgui.cpp.obj
..\components\vendor\ImGui\ImGui.lib: ..\components\vendor\ImGui\CMakeFiles\ImGui.dir\ImGui\imgui_demo.cpp.obj
..\components\vendor\ImGui\ImGui.lib: ..\components\vendor\ImGui\CMakeFiles\ImGui.dir\ImGui\imgui_draw.cpp.obj
..\components\vendor\ImGui\ImGui.lib: ..\components\vendor\ImGui\CMakeFiles\ImGui.dir\ImGui\imgui_tables.cpp.obj
..\components\vendor\ImGui\ImGui.lib: ..\components\vendor\ImGui\CMakeFiles\ImGui.dir\ImGui\imgui_widgets.cpp.obj
..\components\vendor\ImGui\ImGui.lib: ..\components\vendor\ImGui\CMakeFiles\ImGui.dir\ImGui\imgui_impl_opengl3.cpp.obj
..\components\vendor\ImGui\ImGui.lib: ..\components\vendor\ImGui\CMakeFiles\ImGui.dir\build.make
..\components\vendor\ImGui\ImGui.lib: ..\components\vendor\ImGui\CMakeFiles\ImGui.dir\link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="C:\Users\BlackFlage\OneDrive - Politechnika Wroclawska\C++\visual\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX static library ImGui.lib"
	cd C:\Users\BLACKF~1\ONEDRI~1\C__~1\visual\COMPON~1\vendor\ImGui
	$(CMAKE_COMMAND) -P CMakeFiles\ImGui.dir\cmake_clean_target.cmake
	cd C:\Users\BLACKF~1\ONEDRI~1\C__~1\visual\CMAKE-~1
	cd C:\Users\BLACKF~1\ONEDRI~1\C__~1\visual\COMPON~1\vendor\ImGui
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\ImGui.dir\link.txt --verbose=$(VERBOSE)
	cd C:\Users\BLACKF~1\ONEDRI~1\C__~1\visual\CMAKE-~1

# Rule to build all files generated by this target.
..\components\vendor\ImGui\CMakeFiles\ImGui.dir\build: ..\components\vendor\ImGui\ImGui.lib
.PHONY : ..\components\vendor\ImGui\CMakeFiles\ImGui.dir\build

..\components\vendor\ImGui\CMakeFiles\ImGui.dir\clean:
	cd C:\Users\BLACKF~1\ONEDRI~1\C__~1\visual\COMPON~1\vendor\ImGui
	$(CMAKE_COMMAND) -P CMakeFiles\ImGui.dir\cmake_clean.cmake
	cd C:\Users\BLACKF~1\ONEDRI~1\C__~1\visual\CMAKE-~1
.PHONY : ..\components\vendor\ImGui\CMakeFiles\ImGui.dir\clean

..\components\vendor\ImGui\CMakeFiles\ImGui.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" "C:\Users\BlackFlage\OneDrive - Politechnika Wroclawska\C++\visual" "C:\Users\BlackFlage\OneDrive - Politechnika Wroclawska\C++\visual\components\vendor\ImGui" "C:\Users\BlackFlage\OneDrive - Politechnika Wroclawska\C++\visual\cmake-build-debug" "C:\Users\BlackFlage\OneDrive - Politechnika Wroclawska\C++\visual\components\vendor\ImGui" "C:\Users\BlackFlage\OneDrive - Politechnika Wroclawska\C++\visual\components\vendor\ImGui\CMakeFiles\ImGui.dir\DependInfo.cmake" --color=$(COLOR)
.PHONY : ..\components\vendor\ImGui\CMakeFiles\ImGui.dir\depend

