if (NOT DEFINED FHB_MODULE_USE_INCLUDE)
    set(FHB_MODULE_USE_INCLUDE OFF)
endif()
set(FHB_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})
function(fhb_include_module module_dir)
### we don't need this anymore with specifying the bin and lib folders 
    # if (FHB_MODULE_USE_INCLUDE)
    #     include("${FHB_MODULE_PATH}/${module_dir}/CMakeLists.txt")
    # else()
    #     add_subdirectory("${FHB_MODULE_PATH}/${module_dir}" "${module_dir}")
    # endif()
    add_subdirectory("${FHB_MODULE_PATH}/${module_dir}" "${module_dir}")
endfunction()






