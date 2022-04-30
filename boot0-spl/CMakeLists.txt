cmake_minimum_required(VERSION 3.10)
# NOTE: the PROJECT_ROOT variable is set after the first project directive, so we couldn't use it here 
# set(FHB_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../modules")
# add_subdirectory(${FHB_MODULE_PATH}/ccu/ ccu)
# add_subdirectory(${FHB_MODULE_PATH}/common/ common)
# add_subdirectory(${FHB_MODULE_PATH}/dram/ dram)
# add_subdirectory(${FHB_MODULE_PATH}/gpio/ gpio)
# add_subdirectory(${FHB_MODULE_PATH}/timer/ timer)
# add_subdirectory(${FHB_MODULE_PATH}/uart/ uart)
# add_subdirectory(${FHB_MODULE_PATH}/uartgetcode/ uartgetcode)
include_guard(GLOBAL)
include(${CMAKE_CURRENT_LIST_DIR}/../modules/module.cmake)
fhb_include_module(common)
fhb_include_module(ccu)
fhb_include_module(dram)
fhb_include_module(gpio)
fhb_include_module(timer)
fhb_include_module(uart)
fhb_include_module(uartgetcode)
project(boot0main)
enable_language( C CXX ASM )
set(TARGET boot0main)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if (NOT USING_LLVM_TOOLCHAIN)
    set(LINKER_SCRIPT "${CMAKE_CURRENT_LIST_DIR}/boot0.ld")
else()
    set(LINKER_SCRIPT "${CMAKE_CURRENT_LIST_DIR}/boot0-llvm.ld")
endif()
message(STATUS "linker script = ${LINKER_SCRIPT}")

include(${PROJECT_ROOT}/cmake/linker.cmake)
message(STATUS "linker project root = ${PROJECT_ROOT}")
set(boot0main_INCLUDE_DIRS
    "${CMAKE_CURRENT_LIST_DIR}/include"
)
set(TARGET_INCLUDE_DIRS
    ${TARGET_INCLUDE_DIRS}
    ${boot0main_INCLUDE_DIRS}
)
# file(GLOB BOOT0_C_FILES src/*.c)
# file(GLOB BOOT0_S_FILES src/*.S)

add_executable(${TARGET}.elf 
    "${CMAKE_CURRENT_LIST_DIR}/boot0-entry.S"
    "${CMAKE_CURRENT_LIST_DIR}/boot0-main.c"
    "${CMAKE_CURRENT_LIST_DIR}/boot0-header.c"
) 

target_include_directories(${TARGET}.elf PUBLIC 
    ${TARGET_INCLUDE_DIRS}
)


target_link_libraries(${TARGET}.elf PUBLIC ccu)
target_link_libraries(${TARGET}.elf PUBLIC common)
target_link_libraries(${TARGET}.elf PUBLIC dram)
target_link_libraries(${TARGET}.elf PUBLIC gpio)
target_link_libraries(${TARGET}.elf PUBLIC timer)
target_link_libraries(${TARGET}.elf PUBLIC uart)
target_link_libraries(${TARGET}.elf PUBLIC uartgetcode)

add_custom_command(TARGET ${TARGET}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} ${TARGET}.elf -O binary ${TARGET}.bin
    COMMENT "Invoking: objcopy")

add_custom_command(TARGET ${TARGET}.elf POST_BUILD
    COMMAND ${CMAKE_OBJDUMP} -D ${TARGET}.elf  > ${TARGET}.list
    COMMENT "Invoking: objdump")

add_custom_command(TARGET ${TARGET}.elf POST_BUILD
    COMMAND python3 ${PROJECT_ROOT}/scripts/gen_checksum.py boot0main.bin boot0-sdcard.bin 
    COMMENT "Filling in Checksum")

