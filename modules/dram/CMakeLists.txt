cmake_minimum_required(VERSION 3.10)
include_guard()
if (NOT DEFINED FHB_MODULE_PATH)
    set(FHB_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/..")
endif()
include(${FHB_MODULE_PATH}/uart/CMakeLists.txt)

project(dram)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(TARGET dram)
set(dram_INCLUDE_DIRS
    "${CMAKE_CURRENT_LIST_DIR}/include"
)
set(TARGET_INCLUDE_DIRS
    ${TARGET_INCLUDE_DIRS}
    ${dram_INCLUDE_DIRS}
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

target_include_directories(dram PRIVATE 
    ${TARGET_INCLUDE_DIRS}
)



