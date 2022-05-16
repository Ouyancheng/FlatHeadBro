cmake_minimum_required(VERSION 3.10)
include(${CMAKE_CURRENT_LIST_DIR}/../module.cmake)
include_guard(GLOBAL)
fhb_include_module(uart)

project(dram)
enable_language( C CXX ASM )
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(TARGET dram)
set(dram_INCLUDE_DIRS
    "${CMAKE_CURRENT_LIST_DIR}/include"
)



add_library(${TARGET}
    "${CMAKE_CURRENT_LIST_DIR}/src/dram-init.c"
    "${CMAKE_CURRENT_LIST_DIR}/src/mctl_hal.S"
    "${CMAKE_CURRENT_LIST_DIR}/src/memcpy.S"
    "${CMAKE_CURRENT_LIST_DIR}/src/memset.S"
    "${CMAKE_CURRENT_LIST_DIR}/src/tinyprintf.c"
) 

# set_target_properties(${TARGET} PROPERTIES PREFIX "lib")
# set_target_properties(${TARGET} PROPERTIES OUTPUT_NAME "dram")
# set_target_properties(${TARGET} PROPERTIES SUFFIX ".a")

target_include_directories(dram PUBLIC 
    ${dram_INCLUDE_DIRS}
)

target_link_libraries(${TARGET} PUBLIC
    uart
)


