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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.18.4/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.18.4/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/zhang/Desktop/Research/Code/Autotuner/autotuner-developing/knowledge_base

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/zhang/Desktop/Research/Code/Autotuner/autotuner-developing/knowledge_base/build

# Include any dependencies generated for this target.
include CMakeFiles/load.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/load.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/load.dir/flags.make

CMakeFiles/load.dir/load.cpp.o: CMakeFiles/load.dir/flags.make
CMakeFiles/load.dir/load.cpp.o: ../load.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/zhang/Desktop/Research/Code/Autotuner/autotuner-developing/knowledge_base/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/load.dir/load.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/load.dir/load.cpp.o -c /Users/zhang/Desktop/Research/Code/Autotuner/autotuner-developing/knowledge_base/load.cpp

CMakeFiles/load.dir/load.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/load.dir/load.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/zhang/Desktop/Research/Code/Autotuner/autotuner-developing/knowledge_base/load.cpp > CMakeFiles/load.dir/load.cpp.i

CMakeFiles/load.dir/load.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/load.dir/load.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/zhang/Desktop/Research/Code/Autotuner/autotuner-developing/knowledge_base/load.cpp -o CMakeFiles/load.dir/load.cpp.s

CMakeFiles/load.dir/hardware.cpp.o: CMakeFiles/load.dir/flags.make
CMakeFiles/load.dir/hardware.cpp.o: ../hardware.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/zhang/Desktop/Research/Code/Autotuner/autotuner-developing/knowledge_base/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/load.dir/hardware.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/load.dir/hardware.cpp.o -c /Users/zhang/Desktop/Research/Code/Autotuner/autotuner-developing/knowledge_base/hardware.cpp

CMakeFiles/load.dir/hardware.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/load.dir/hardware.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/zhang/Desktop/Research/Code/Autotuner/autotuner-developing/knowledge_base/hardware.cpp > CMakeFiles/load.dir/hardware.cpp.i

CMakeFiles/load.dir/hardware.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/load.dir/hardware.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/zhang/Desktop/Research/Code/Autotuner/autotuner-developing/knowledge_base/hardware.cpp -o CMakeFiles/load.dir/hardware.cpp.s

CMakeFiles/load.dir/kernel.cpp.o: CMakeFiles/load.dir/flags.make
CMakeFiles/load.dir/kernel.cpp.o: ../kernel.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/zhang/Desktop/Research/Code/Autotuner/autotuner-developing/knowledge_base/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/load.dir/kernel.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/load.dir/kernel.cpp.o -c /Users/zhang/Desktop/Research/Code/Autotuner/autotuner-developing/knowledge_base/kernel.cpp

CMakeFiles/load.dir/kernel.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/load.dir/kernel.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/zhang/Desktop/Research/Code/Autotuner/autotuner-developing/knowledge_base/kernel.cpp > CMakeFiles/load.dir/kernel.cpp.i

CMakeFiles/load.dir/kernel.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/load.dir/kernel.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/zhang/Desktop/Research/Code/Autotuner/autotuner-developing/knowledge_base/kernel.cpp -o CMakeFiles/load.dir/kernel.cpp.s

CMakeFiles/load.dir/kernel_map.cpp.o: CMakeFiles/load.dir/flags.make
CMakeFiles/load.dir/kernel_map.cpp.o: ../kernel_map.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/zhang/Desktop/Research/Code/Autotuner/autotuner-developing/knowledge_base/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/load.dir/kernel_map.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/load.dir/kernel_map.cpp.o -c /Users/zhang/Desktop/Research/Code/Autotuner/autotuner-developing/knowledge_base/kernel_map.cpp

CMakeFiles/load.dir/kernel_map.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/load.dir/kernel_map.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/zhang/Desktop/Research/Code/Autotuner/autotuner-developing/knowledge_base/kernel_map.cpp > CMakeFiles/load.dir/kernel_map.cpp.i

CMakeFiles/load.dir/kernel_map.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/load.dir/kernel_map.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/zhang/Desktop/Research/Code/Autotuner/autotuner-developing/knowledge_base/kernel_map.cpp -o CMakeFiles/load.dir/kernel_map.cpp.s

CMakeFiles/load.dir/model.cpp.o: CMakeFiles/load.dir/flags.make
CMakeFiles/load.dir/model.cpp.o: ../model.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/zhang/Desktop/Research/Code/Autotuner/autotuner-developing/knowledge_base/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/load.dir/model.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/load.dir/model.cpp.o -c /Users/zhang/Desktop/Research/Code/Autotuner/autotuner-developing/knowledge_base/model.cpp

CMakeFiles/load.dir/model.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/load.dir/model.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/zhang/Desktop/Research/Code/Autotuner/autotuner-developing/knowledge_base/model.cpp > CMakeFiles/load.dir/model.cpp.i

CMakeFiles/load.dir/model.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/load.dir/model.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/zhang/Desktop/Research/Code/Autotuner/autotuner-developing/knowledge_base/model.cpp -o CMakeFiles/load.dir/model.cpp.s

CMakeFiles/load.dir/step.cpp.o: CMakeFiles/load.dir/flags.make
CMakeFiles/load.dir/step.cpp.o: ../step.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/zhang/Desktop/Research/Code/Autotuner/autotuner-developing/knowledge_base/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/load.dir/step.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/load.dir/step.cpp.o -c /Users/zhang/Desktop/Research/Code/Autotuner/autotuner-developing/knowledge_base/step.cpp

CMakeFiles/load.dir/step.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/load.dir/step.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/zhang/Desktop/Research/Code/Autotuner/autotuner-developing/knowledge_base/step.cpp > CMakeFiles/load.dir/step.cpp.i

CMakeFiles/load.dir/step.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/load.dir/step.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/zhang/Desktop/Research/Code/Autotuner/autotuner-developing/knowledge_base/step.cpp -o CMakeFiles/load.dir/step.cpp.s

CMakeFiles/load.dir/src/Model.cpp.o: CMakeFiles/load.dir/flags.make
CMakeFiles/load.dir/src/Model.cpp.o: ../src/Model.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/zhang/Desktop/Research/Code/Autotuner/autotuner-developing/knowledge_base/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/load.dir/src/Model.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/load.dir/src/Model.cpp.o -c /Users/zhang/Desktop/Research/Code/Autotuner/autotuner-developing/knowledge_base/src/Model.cpp

CMakeFiles/load.dir/src/Model.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/load.dir/src/Model.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/zhang/Desktop/Research/Code/Autotuner/autotuner-developing/knowledge_base/src/Model.cpp > CMakeFiles/load.dir/src/Model.cpp.i

CMakeFiles/load.dir/src/Model.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/load.dir/src/Model.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/zhang/Desktop/Research/Code/Autotuner/autotuner-developing/knowledge_base/src/Model.cpp -o CMakeFiles/load.dir/src/Model.cpp.s

CMakeFiles/load.dir/src/Tensor.cpp.o: CMakeFiles/load.dir/flags.make
CMakeFiles/load.dir/src/Tensor.cpp.o: ../src/Tensor.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/zhang/Desktop/Research/Code/Autotuner/autotuner-developing/knowledge_base/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/load.dir/src/Tensor.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/load.dir/src/Tensor.cpp.o -c /Users/zhang/Desktop/Research/Code/Autotuner/autotuner-developing/knowledge_base/src/Tensor.cpp

CMakeFiles/load.dir/src/Tensor.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/load.dir/src/Tensor.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/zhang/Desktop/Research/Code/Autotuner/autotuner-developing/knowledge_base/src/Tensor.cpp > CMakeFiles/load.dir/src/Tensor.cpp.i

CMakeFiles/load.dir/src/Tensor.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/load.dir/src/Tensor.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/zhang/Desktop/Research/Code/Autotuner/autotuner-developing/knowledge_base/src/Tensor.cpp -o CMakeFiles/load.dir/src/Tensor.cpp.s

# Object files for target load
load_OBJECTS = \
"CMakeFiles/load.dir/load.cpp.o" \
"CMakeFiles/load.dir/hardware.cpp.o" \
"CMakeFiles/load.dir/kernel.cpp.o" \
"CMakeFiles/load.dir/kernel_map.cpp.o" \
"CMakeFiles/load.dir/model.cpp.o" \
"CMakeFiles/load.dir/step.cpp.o" \
"CMakeFiles/load.dir/src/Model.cpp.o" \
"CMakeFiles/load.dir/src/Tensor.cpp.o"

# External object files for target load
load_EXTERNAL_OBJECTS =

load: CMakeFiles/load.dir/load.cpp.o
load: CMakeFiles/load.dir/hardware.cpp.o
load: CMakeFiles/load.dir/kernel.cpp.o
load: CMakeFiles/load.dir/kernel_map.cpp.o
load: CMakeFiles/load.dir/model.cpp.o
load: CMakeFiles/load.dir/step.cpp.o
load: CMakeFiles/load.dir/src/Model.cpp.o
load: CMakeFiles/load.dir/src/Tensor.cpp.o
load: CMakeFiles/load.dir/build.make
load: /usr/local/lib/libtensorflow.dylib
load: CMakeFiles/load.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/zhang/Desktop/Research/Code/Autotuner/autotuner-developing/knowledge_base/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Linking CXX executable load"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/load.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/load.dir/build: load

.PHONY : CMakeFiles/load.dir/build

CMakeFiles/load.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/load.dir/cmake_clean.cmake
.PHONY : CMakeFiles/load.dir/clean

CMakeFiles/load.dir/depend:
	cd /Users/zhang/Desktop/Research/Code/Autotuner/autotuner-developing/knowledge_base/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/zhang/Desktop/Research/Code/Autotuner/autotuner-developing/knowledge_base /Users/zhang/Desktop/Research/Code/Autotuner/autotuner-developing/knowledge_base /Users/zhang/Desktop/Research/Code/Autotuner/autotuner-developing/knowledge_base/build /Users/zhang/Desktop/Research/Code/Autotuner/autotuner-developing/knowledge_base/build /Users/zhang/Desktop/Research/Code/Autotuner/autotuner-developing/knowledge_base/build/CMakeFiles/load.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/load.dir/depend

