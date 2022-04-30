cmake_minimum_required(VERSION 3.10)
project(FlatHeadBro)
if (NOT DEFINED FHB_MODULE_USE_INCLUDE)
    set(FHB_MODULE_USE_INCLUDE OFF)
endif()
if (FHB_MODULE_USE_INCLUDE)
    include(boot0-spl/CMakeLists.txt)
    include(secondary/bootloadertest/CMakeLists.txt)
else()
    add_subdirectory(boot0-spl)
    add_subdirectory(secondary/bootloadertest)
endif()



