set(CMAKE_SYSTEM_NAME               Linux)
set(CMAKE_SYSTEM_PROCESSOR          riscv64)

set(CMAKE_TRY_COMPILE_TARGET_TYPE   STATIC_LIBRARY)
set(BAREMETAL_RV64_TOOLCHAIN_PATH "/Volumes/Data/workspaces/xuantie-gcc/bin/")
set(CMAKE_EXECUTABLE_SUFFIX "")
set(CMAKE_AR                        "${BAREMETAL_RV64_TOOLCHAIN_PATH}riscv64-unknown-elf-ar${CMAKE_EXECUTABLE_SUFFIX}")
set(CMAKE_ASM_COMPILER              "${BAREMETAL_RV64_TOOLCHAIN_PATH}riscv64-unknown-elf-gcc${CMAKE_EXECUTABLE_SUFFIX}")
set(CMAKE_C_COMPILER                "${BAREMETAL_RV64_TOOLCHAIN_PATH}riscv64-unknown-elf-gcc${CMAKE_EXECUTABLE_SUFFIX}")
set(CMAKE_CXX_COMPILER              "${BAREMETAL_RV64_TOOLCHAIN_PATH}riscv64-unknown-elf-g++${CMAKE_EXECUTABLE_SUFFIX}")
set(CMAKE_LINKER                    "${BAREMETAL_RV64_TOOLCHAIN_PATH}riscv64-unknown-elf-ld${CMAKE_EXECUTABLE_SUFFIX}")
set(CMAKE_OBJCOPY                   "${BAREMETAL_RV64_TOOLCHAIN_PATH}riscv64-unknown-elf-objcopy${CMAKE_EXECUTABLE_SUFFIX}")
set(CMAKE_RANLIB                    "${BAREMETAL_RV64_TOOLCHAIN_PATH}riscv64-unknown-elf-ranlib${CMAKE_EXECUTABLE_SUFFIX}"  )
set(CMAKE_SIZE                      "${BAREMETAL_RV64_TOOLCHAIN_PATH}riscv64-unknown-elf-size${CMAKE_EXECUTABLE_SUFFIX}"  )
set(CMAKE_STRIP                     "${BAREMETAL_RV64_TOOLCHAIN_PATH}riscv64-unknown-elf-strip${CMAKE_EXECUTABLE_SUFFIX}"  )
set(CMAKE_OBJDUMP                   "${BAREMETAL_RV64_TOOLCHAIN_PATH}riscv64-unknown-elf-objdump${CMAKE_EXECUTABLE_SUFFIX}")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(PROJECT_ROOT "${CMAKE_CURRENT_LIST_DIR}/..")
set(CPU_TYPE "")
set(TARGET_TRIPLET "riscv64-unknown-none-elf") 
set(TARGET_ARCH "")
# -nostartfiles  in C_FLAGS + ASM_FLAGS? 
set(C_FLAGS_COMMON                  "-march=rv64gvxthead -mabi=lp64d -mcmodel=medany -fno-stack-protector -Wall -nostdlib -ffreestanding -Werror -Wno-unused-function -Wno-unused-variable")
set(CMAKE_C_FLAGS                   "-Og ${C_FLAGS_COMMON} -std=gnu11 -Wno-pointer-sign ")
set(CMAKE_C_FLAGS_RELEASE           "-Os -DNDEBUG ${C_FLAGS_COMMON} -std=gnu11 -Wno-pointer-sign ")
set(CMAKE_CXX_FLAGS                 "-Og ${C_FLAGS_COMMON} -std=gnu++17 -fno-exceptions -fno-unwind-tables -fno-rtti")
set(CMAKE_CXX_FLAGS_RELEASE         "-Os -DNDEBUG ${C_FLAGS_COMMON} -std=gnu++17 -fno-exceptions -fno-unwind-tables -fno-rtti")
set(CMAKE_ASM_FLAGS                 "-nostdlib -ffreestanding -Wa,--fatal-warnings ")


