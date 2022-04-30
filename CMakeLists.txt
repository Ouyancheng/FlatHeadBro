cmake_minimum_required(VERSION 3.10)
project(FlatHeadBro)



set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# NOTE: do not use include, otherwise the linker scripts with be messed up! 
add_subdirectory(boot0-spl)
add_subdirectory(secondary/bootloadertest)



