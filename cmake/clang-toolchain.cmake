set(CMAKE_SYSTEM_NAME               Linux)
set(CMAKE_SYSTEM_PROCESSOR          riscv64)

set(CMAKE_TRY_COMPILE_TARGET_TYPE   STATIC_LIBRARY)
set(BAREMETAL_RV64_TOOLCHAIN_PATH "/Users/ouyancheng/homebrew/opt/llvm/bin/")
set(CMAKE_EXECUTABLE_SUFFIX "")
set(CMAKE_AR                        "${BAREMETAL_RV64_TOOLCHAIN_PATH}llvm-ar${CMAKE_EXECUTABLE_SUFFIX}")
set(CMAKE_ASM_COMPILER              "${BAREMETAL_RV64_TOOLCHAIN_PATH}clang${CMAKE_EXECUTABLE_SUFFIX}")
set(CMAKE_C_COMPILER                "${BAREMETAL_RV64_TOOLCHAIN_PATH}clang${CMAKE_EXECUTABLE_SUFFIX}")
set(CMAKE_CXX_COMPILER              "${BAREMETAL_RV64_TOOLCHAIN_PATH}clang++${CMAKE_EXECUTABLE_SUFFIX}")
### WTF lld has a bug?!! Why is it placing the string pool between the header and the entry?????!!!!!
set(CMAKE_LINKER                    "${BAREMETAL_RV64_TOOLCHAIN_PATH}ld.lld${CMAKE_EXECUTABLE_SUFFIX}")
# set(CMAKE_LINKER "/Volumes/Data/workspaces/xuantie-gcc/bin/riscv64-unknown-elf-ld")
set(CMAKE_OBJCOPY                   "${BAREMETAL_RV64_TOOLCHAIN_PATH}llvm-objcopy${CMAKE_EXECUTABLE_SUFFIX}")
set(CMAKE_RANLIB                    "${BAREMETAL_RV64_TOOLCHAIN_PATH}llvm-ranlib${CMAKE_EXECUTABLE_SUFFIX}"  )
set(CMAKE_SIZE                      "${BAREMETAL_RV64_TOOLCHAIN_PATH}llvm-size${CMAKE_EXECUTABLE_SUFFIX}"  )
set(CMAKE_STRIP                     "${BAREMETAL_RV64_TOOLCHAIN_PATH}llvm-strip${CMAKE_EXECUTABLE_SUFFIX}"  )
set(CMAKE_OBJDUMP                   "${BAREMETAL_RV64_TOOLCHAIN_PATH}llvm-objdump${CMAKE_EXECUTABLE_SUFFIX}")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(PROJECT_ROOT "${CMAKE_CURRENT_LIST_DIR}/..")
set(CPU_TYPE "")
set(TARGET_TRIPLET "riscv64-unknown-none-elf") 
set(TARGET_ARCH "rv64gvxthead")
set(USING_LLVM_TOOLCHAIN ON)
# -nostartfiles  in C_FLAGS + ASM_FLAGS? 
# -menable-experimental-extensions 
# set(CODE_MODEL "medany")  # use auipc instruction rd = pc + (imm20 << 12)
set(CODE_MODEL "medlow")  # use lui instruction to take the upper 20 bits of an immediate rd = (imm20 << 12) 
set(C_FLAGS_COMMON                  "-fPIC --target=${TARGET_TRIPLET} -mabi=lp64d -mcmodel=${CODE_MODEL} -mno-relax -fno-stack-protector -Wall -nostdlib -ffreestanding -Werror -Wno-unused-function -Wno-unused-variable")
set(CMAKE_C_FLAGS                   "-Og ${C_FLAGS_COMMON} -std=gnu11 -Wno-pointer-sign ")
set(CMAKE_C_FLAGS_RELEASE           "-Os -DNDEBUG ${C_FLAGS_COMMON} -std=gnu11 -Wno-pointer-sign ")
set(CMAKE_CXX_FLAGS                 "-Og ${C_FLAGS_COMMON} -std=gnu++17 -fno-exceptions -fno-unwind-tables -fno-rtti")
set(CMAKE_CXX_FLAGS_RELEASE         "-Os -DNDEBUG ${C_FLAGS_COMMON} -std=gnu++17 -fno-exceptions -fno-unwind-tables -fno-rtti")
set(CMAKE_ASM_FLAGS                 "-fPIC --target=${TARGET_TRIPLET} -mabi=lp64d -mcmodel=${CODE_MODEL} -nostdlib -ffreestanding -Wa,--fatal-warnings -mno-relax")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

