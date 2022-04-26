cmake_minimum_required(VERSION 3.10)
include_guard()
if (NOT DEFINED FHB_MODULE_PATH)
    set(FHB_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/..")
endif()
include(${FHB_MODULE_PATH}/common/CMakeLists.txt)
include(${FHB_MODULE_PATH}/uart/CMakeLists.txt)
project(uartgetcode)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(TARGET uartgetcode)
set(uartgetcode_INCLUDE_DIRS 
    "${CMAKE_CURRENT_LIST_DIR}/include"
)
set(TARGET_INCLUDE_DIRS
    ${TARGET_INCLUDE_DIRS}
    ${uartgetcode_INCLUDE_DIRS}
)
add_library(${TARGET} 
    "${CMAKE_CURRENT_LIST_DIR}/src/uartgetcode.c"
) 

# set_target_properties(${TARGET}  PROPERTIES PREFIX "")
# set_target_properties(${TARGET}  PROPERTIES OUTPUT_NAME ${TARGET})
# set_target_properties(${TARGET}  PROPERTIES SUFFIX ".a")

target_include_directories(${TARGET} PRIVATE 
    ${TARGET_INCLUDE_DIRS}
)



