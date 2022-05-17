cmake_minimum_required(VERSION 3.10)
include(${CMAKE_CURRENT_LIST_DIR}/../module.cmake)
include_guard(GLOBAL)
fhb_include_module(common)
fhb_include_module(gpio)
fhb_include_module(c906)
project(uart)
enable_language( C CXX ASM )
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(TARGET uart)
set(uart_INCLUDE_DIRS 
    "${CMAKE_CURRENT_LIST_DIR}/include"
)


add_library(${TARGET} 
    "${CMAKE_CURRENT_LIST_DIR}/src/uart.c"
) 

# set_target_properties(${TARGET}  PROPERTIES PREFIX "")
# set_target_properties(${TARGET}  PROPERTIES OUTPUT_NAME ${TARGET})
# set_target_properties(${TARGET}  PROPERTIES SUFFIX ".a")
target_include_directories(${TARGET} PUBLIC 
    ${uart_INCLUDE_DIRS}
)
target_link_libraries(${TARGET} PUBLIC
    common  
    gpio 
    c906 
)


