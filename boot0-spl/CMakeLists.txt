project(boot0)

cmake_minimum_required(VERSION 3.9)
set(TARGET boot0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include(${PROJECT_ROOT}/cmake/common.cmake)
add_subdirectory(src/libdram)

file(GLOB BOOT0_C_FILES src/*.c)
file(GLOB BOOT0_S_FILES src/*.S)

add_executable(${TARGET}.elf ${BOOT0_C_FILES} ${BOOT0_S_FILES})

target_include_directories(${TARGET}.elf PUBLIC "${PROJECT_ROOT}/include")

target_link_libraries(${TARGET}.elf PUBLIC libdram)

add_custom_command(TARGET ${TARGET}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} ${TARGET}.elf -O binary ${TARGET}.bin
    COMMENT "Invoking: objcopy")

add_custom_command(TARGET ${TARGET}.elf POST_BUILD
    COMMAND ${CMAKE_OBJDUMP} -D ${TARGET}.elf  > ${TARGET}.list
    COMMENT "Invoking: objdump")

    add_custom_command(TARGET ${TARGET}.elf POST_BUILD
    COMMAND python3 ../scripts/gen_checksum.py boot0.bin boot0-sdcard.bin 
    COMMENT "Filling in Checksum")

