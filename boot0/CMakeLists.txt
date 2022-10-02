cmake_minimum_required(VERSION 3.10)

include_guard(GLOBAL)

include(${CMAKE_CURRENT_LIST_DIR}/../modules/module.cmake)
fhb_include_module(common)
fhb_include_module(c906)
fhb_include_module(ccu)
fhb_include_module(dram)
fhb_include_module(gpio)
fhb_include_module(timer)
fhb_include_module(uart)
fhb_include_module(uartgetcode)

project(boot0)

enable_language( C ASM )

set(TARGET boot0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(LINKER_SCRIPT "${CMAKE_CURRENT_LIST_DIR}/boot0.ld")
include(${PROJECT_ROOT}/cmake/linker.cmake)


set(TARGET_INCLUDE_DIRS ${TARGET}_INCLUDE_DIRS)

set(TARGET_INCLUDE_DIRS
    "${CMAKE_CURRENT_LIST_DIR}/include"
)

add_executable(${TARGET}.elf 
    entry.S 
    boot0-header.c
    main.c
) 

target_include_directories(${TARGET}.elf PUBLIC 
    ${TARGET_INCLUDE_DIRS}
)
target_link_libraries(${TARGET}.elf PUBLIC ccu)
target_link_libraries(${TARGET}.elf PUBLIC common)
target_link_libraries(${TARGET}.elf PUBLIC c906)
target_link_libraries(${TARGET}.elf PUBLIC dram)
target_link_libraries(${TARGET}.elf PUBLIC gpio)
target_link_libraries(${TARGET}.elf PUBLIC timer)
target_link_libraries(${TARGET}.elf PUBLIC uart)
target_link_libraries(${TARGET}.elf PUBLIC uartgetcode)

add_custom_command(TARGET ${TARGET}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TARGET}.elf" -O binary "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TARGET}.bin"
    COMMENT "Invoking: objcopy")

add_custom_command(TARGET ${TARGET}.elf POST_BUILD
    COMMAND ${CMAKE_OBJDUMP} -D "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TARGET}.elf"  > "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TARGET}.list"
    COMMENT "Invoking: objdump")

add_custom_command(TARGET ${TARGET}.elf POST_BUILD
    COMMAND python3 ${PROJECT_ROOT}/scripts/gen_checksum.py "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TARGET}.bin" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TARGET}lite-sdcard.bin" 
    COMMENT "Filling in Checksum")










