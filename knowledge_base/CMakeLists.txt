cmake_minimum_required(VERSION 3.10)
project(main)

find_library(TENSORFLOW_LIB tensorflow HINT $ENV{HOME}/libtensorflow/lib)

set(CMAKE_CXX_STANDARD 17)
add_executable(save save.cpp hardware.cpp kernel.cpp kernel_map.cpp model.cpp step.cpp src/Model.cpp src/Tensor.cpp)
add_executable(load load.cpp hardware.cpp kernel.cpp kernel_map.cpp model.cpp step.cpp src/Model.cpp src/Tensor.cpp)
target_include_directories(save PRIVATE include $ENV{HOME}/libtensorflow/include)
target_include_directories(load PRIVATE include $ENV{HOME}/libtensorflow/include)
target_link_libraries (save ${TENSORFLOW_LIB})
target_link_libraries (load ${TENSORFLOW_LIB})
include_directories(.)