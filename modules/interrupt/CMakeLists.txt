cmake_minimum_required(VERSION 3.10)
include(${CMAKE_CURRENT_LIST_DIR}/../module.cmake)
include_guard(GLOBAL)
fhb_include_module(common)
fhb_include_module(c906)
fhb_include_module(riscv64)
fhb_include_module(printf)
fhb_include_module(timer)
project(interrupt)
enable_language( C CXX ASM )
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(TARGET interrupt)
set(interrupt_INCLUDE_DIRS 
    "${CMAKE_CURRENT_LIST_DIR}/include"
)



add_library(${TARGET} 
    "${CMAKE_CURRENT_LIST_DIR}/src/interrupt.c"
    "${CMAKE_CURRENT_LIST_DIR}/src/interrupt-trampoline.S"
    "${CMAKE_CURRENT_LIST_DIR}/src/plic.c"
    "${CMAKE_CURRENT_LIST_DIR}/src/clint.c"
) 

# set_target_properties(${TARGET}  PROPERTIES PREFIX "")
# set_target_properties(${TARGET}  PROPERTIES OUTPUT_NAME ${TARGET})
# set_target_properties(${TARGET}  PROPERTIES SUFFIX ".a")
target_include_directories(${TARGET} PUBLIC 
    ${interrupt_INCLUDE_DIRS}
)
target_link_libraries(${TARGET} PUBLIC
    common
    c906 
    riscv64 
    printf
    timer 
)


