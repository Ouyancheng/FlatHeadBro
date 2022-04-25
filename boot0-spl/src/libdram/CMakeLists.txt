project(libdram)

cmake_minimum_required(VERSION 3.9)
set(TARGET libdram)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include(${PROJECT_ROOT}/cmake/common.cmake)


add_library(libdram dram-init.c mctl_hal.S memcpy.S memset.S) 

set_target_properties(libdram PROPERTIES PREFIX "")
set_target_properties(libdram PROPERTIES OUTPUT_NAME "libdram")
set_target_properties(libdram PROPERTIES SUFFIX ".a")

target_include_directories(libdram PUBLIC ${PROJECT_ROOT}/include)



