cmake_minimum_required(VERSION 3.10)
include_guard()
if (NOT DEFINED FHB_MODULE_PATH)
    set(FHB_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/..")
endif()
project(gpio)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(TARGET gpio)
set(gpio_INCLUDE_DIRS 
    "${CMAKE_CURRENT_LIST_DIR}/include"
)
set(TARGET_INCLUDE_DIRS
    ${TARGET_INCLUDE_DIRS}
    ${gpio_INCLUDE_DIRS}
)
add_library(${TARGET} 
    "${CMAKE_CURRENT_LIST_DIR}/src/gpio.c"
) 

# set_target_properties(${TARGET}  PROPERTIES PREFIX "")
# set_target_properties(${TARGET}  PROPERTIES OUTPUT_NAME ${TARGET})
# set_target_properties(${TARGET}  PROPERTIES SUFFIX ".a")


target_include_directories(${TARGET} PUBLIC 
    ${TARGET_INCLUDE_DIRS}
)



