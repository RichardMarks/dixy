cmake_minimum_required(VERSION 3.16)
project(dixy_cpp)
set(CMAKE_CXX_STANDARD 17)
add_executable(dixy dixy-cli-main.cpp dixy-assembler.cpp dixy-linker.cpp dixy-emulator.cpp)
