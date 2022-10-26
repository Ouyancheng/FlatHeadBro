cmake_minimum_required(VERSION 3.10)
project(FlatHeadBro)



# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# NOTE: do not use include, otherwise the linker scripts will be messed up! 
add_subdirectory(boot0-spl)
add_subdirectory(boot0)
add_subdirectory(secondary/bootloadertest)
add_subdirectory(secondary/interrupttest)
add_subdirectory(secondary/supervisorinterrupttest)
add_subdirectory(secondary/mmutest)
add_subdirectory(secondary/steppermotor)


if (CMAKE_C_COMPILER_ID STREQUAL "Clang" AND CMAKE_C_COMPILER_VERSION VERSION_LESS "15")
    set(CMAKE_C_FLAGS                   "${CMAKE_C_FLAGS} -mno-relax ")
    set(CMAKE_C_FLAGS_RELEASE           "${CMAKE_C_FLAGS_RELEASE} -mno-relax ")
    set(CMAKE_CXX_FLAGS                 "${CMAKE_CXX_FLAGS} -mno-relax")
    set(CMAKE_CXX_FLAGS_RELEASE         "${CMAKE_CXX_FLAGS_RELEASE} -mno-relax")
    set(CMAKE_ASM_FLAGS                 "${CMAKE_ASM_FLAGS} -mno-relax") 
endif()
# message(STATUS "${CMAKE_C_COMPILER_ID}-${CMAKE_C_COMPILER_VERSION}, ${CMAKE_C_FLAGS} ${CMAKE_ASM_FLAGS}")
