cmake_minimum_required(VERSION 3.10)

include(${CMAKE_CURRENT_LIST_DIR}/../module.cmake)
include_guard(GLOBAL)
fhb_include_module(riscv64) 
project(c906)

enable_language( C CXX ASM )
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(TARGET c906)
set(c906_INCLUDE_DIRS 
    "${CMAKE_CURRENT_LIST_DIR}/include"
)


add_library(${TARGET} 
    "${CMAKE_CURRENT_LIST_DIR}/src/delay.c"
    "${CMAKE_CURRENT_LIST_DIR}/src/delay-asm.S"
)

# set_target_properties(${TARGET}  PROPERTIES PREFIX "")
# set_target_properties(${TARGET}  PROPERTIES OUTPUT_NAME ${TARGET})
# set_target_properties(${TARGET}  PROPERTIES SUFFIX ".a")


target_include_directories(${TARGET} PUBLIC 
    ${c906_INCLUDE_DIRS}
)
target_link_libraries(${TARGET} PUBLIC 
    riscv64 
)


