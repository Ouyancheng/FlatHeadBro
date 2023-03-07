# FlatHeadBro

This is an experiment to run baremetal programs on Lichee RV or D1 Nezha development boards. 

The boot0 program is loaded from the SD card, and it will further load secondary program from UART, instead of downloading using FEL. 

## boot0-spl 

This is the first program to run after powering on and going through the BROM phrase. 

### Build requirement: 

- cmake 
- riscv64-unknown-elf-gcc or clang (with riscv64 target support) 
- objcopy and objdump supporting riscv64 elf (usually shipped with riscv64-unknown-elf-gcc or LLVM)
- python3 

### Build instructions: 

#### Using riscv64 gcc 

If using riscv64-unknown-elf-gcc, **modify `cmake/gcc-toolchain.cmake` to specify your path to riscv64-unknown-elf-gcc and the binary utilities**. 
```
mkdir build 
cd build 
cmake .. --toolchain ../cmake/gcc-toolchain.cmake
# for older cmake versions, you may want: cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchain.cmake 
make 
```
If you are using T-head's own gcc, you can use `toolchain.cmake` as the toolchain file, that might take advantage of T-head's instruction extension. 
#### Using clang 
If using clang, **modify `cmake/clang-toolchain.cmake` to specify your path to clang and the binary utilities** (Note: if you are using macOS, Apple's clang shipped with Command Line Tools will NOT work, please install the LLVM toolchain via homebrew: `brew install llvm`, and make sure your clang has the riscv64 target: `${YOUR_PATH_TO_CLANG}/clang --print-targets`). 
```
mkdir build 
cd build 
cmake .. --toolchain ../cmake/clang-toolchain.cmake
# for older cmake versions, you may want: cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake/clang-toolchain.cmake 
make 
```

### Running the boot0-spl 

After your build, in the build/bin directory you will get a binary file `boot0-sdcard.bin`, and that's the file that will be flashed to the SD card. 

```
# In the build directory 
sudo dd if=bin/boot0-sdcard.bin of=/dev/<YOUR_SD_CARD_DEVICE> bs=4096 seek=2
sync 
```

For macOS, you may want to unmount the disk before dd-ing: 
```
# In the build directory 
diskutil unmountDisk <YOUR_SD_CARD_DEVICE> 
sudo dd if=bin/boot0-sdcard.bin of=/dev/<YOUR_SD_CARD_DEVICE> bs=4096 seek=2
sync 
diskutil unmountDisk <YOUR_SD_CARD_DEVICE> 
``` 

**There's also a simplified version of the boot0 bootloader `bin/boot0lite-sdcard.bin` (source code in folder `boot0`), you may use it to replace `boot0-sdcard.bin`.**

Then insert your SD card to the Lichee RV and power it on (could be done via GPIO pins), the UART will print the following, use `screen /dev/cu.SLAB_USBtoUART 115200` to monitor UART, of course with a USB to UART dongle (NOTE: this message is now suppressed -- it will print `dram init result (dram size): 0x200MB` indicating the board has 512MB RAM. If you still want to see the DRAM init message, set `DRAM_DEBUG` to 1 in `modules/dram/src/tinyprintf.c`): 
```
DRAM only have internal ZQ!!
get_pmu_exist() = 4294967295
ddr_efuse_type: 0x0
[AUTO DEBUG] single rank and full DQ!
ddr_efuse_type: 0x0
[AUTO DEBUG] rank 0 row = 15 
[AUTO DEBUG] rank 0 bank = 8 
[AUTO DEBUG] rank 0 page size = 2 KB 
DRAM BOOT DRIVE INFO: %s
DRAM CLK = 792 MHz
DRAM Type = 3 (2:DDR2,3:DDR3)
DRAMC ZQ value: 0x7b7bfb
DRAM ODT value: 0x42.
ddr_efuse_type: 0x0
DRAM SIZE =512 M
DRAM simple test OK.

```
~~And then it will echo anything you type into the UART terminal.~~ 
~~If you connect an LED to GPIO PE16, you will also see the LED blinking each time you type a character.~~ 
Then you would be able to use `uart-uploader/my-install.py` to upload the binary file through UART. 
```
python3 uart-uploader/my-install.py <YOUR_BINARY_FILE>
```
After uploading it will handle the UART communication between host and the board. 

## Modules 

Invidividual modules in the modules directory could be built as library -- just use the corresponding CMakeLists.txt. 
Notice that if you build one module as a library, all other modules it depends on will be built! 

e.g., if you want to build the uart module and all of its dependencies: 
```
mkdir build 
cd build 
cmake ../modules/uart --toolchain ../cmake/toolchain.cmake 
make 
```
You will see `libcommon.a`, `libgpio.a`, `libuart.a` in the build/lib directory, and as you can see the uart module depends on the common and gpio modules. 

~~The option `FHB_MODULE_USE_INCLUDE` is to tell CMake to use include for importing dependency instead of add_subdirectory. Using include will list all of the dependencies at the top-level build folder, instead of hiding in one of the subdirectories. However, using include to import the module will cause CMake generate a chain of directories from / to your module path in the build directory for each dependency...~~ 


## Adding a new secondary program 

You can add another new secondary program by adding a folder in under the `secondary` directory. 
e.g., let's make a secondary program called `hello`. 

You would make a directory called `hello` under the `secondary` folder. 

Then copy `secondary/mmutest/linker.ld` and `secondary/mmutest/start.S` to `secondary/hello`. 

Create a file called `CMakeLists.txt` in `secondary/hello`. 
Write the following: 

```
cmake_minimum_required(VERSION 3.10)
include_guard(GLOBAL)
include(${CMAKE_CURRENT_LIST_DIR}/../../modules/module.cmake) # for fhb_include_module 
fhb_include_module(common)    # includes the common module 
fhb_include_module(gpio)      # includes the gpio module 
fhb_include_module(uart)      # includes the uart module 
fhb_include_module(printf)    # includes the printf module 
# includes the kernelsdk module, this module includes common,uart,printf, etc. so technically we don't need to include the modules above 
fhb_include_module(kernelsdk) 
project(hello)                # your project name 

enable_language( C CXX ASM )  # the allowed languages 
set(TARGET hello)             # this determines your executable name 

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(LINKER_SCRIPT ${CMAKE_CURRENT_LIST_DIR}/linker.ld)  # use the newly copied linker.ld 
# specifies the linker command -- by default, cmake uses the compiler as the linker. However, we need our own dedicated linker in the toolchain file 
include(${PROJECT_ROOT}/cmake/linker.cmake)  
add_executable(${TARGET}.elf 
    "${CMAKE_CURRENT_LIST_DIR}/main.c"    # the main.c file, you will need to write them 
    "${CMAKE_CURRENT_LIST_DIR}/start.S"   # the boilerplate code, initializes the program and calls main 
    # if you have more .c .S .cpp files for the hello program, add them here 
) 
# add your included modules here so that cmake knows to link them 
target_link_libraries(${TARGET}.elf PUBLIC 
    common
    gpio
    uart
    printf 
    kernelsdk
)
# copy TARGET.elf to TARGET.bin in raw binary 
add_custom_command(TARGET ${TARGET}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TARGET}.elf" -O binary "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TARGET}.bin"
    COMMENT "Invoking: objcopy")
# execute objdump on the TARGET.elf 
add_custom_command(TARGET ${TARGET}.elf POST_BUILD
    COMMAND ${CMAKE_OBJDUMP} -D "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TARGET}.elf"  > "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TARGET}.list"
    COMMENT "Invoking: objdump")
```

Then add a file `secondary/hello/main.c`: 
```
#include "uart.h" 
#include "printf.h" 
#include "kernelsdk.h"
void main(void) {
    uart_init(0, 1);  // initializes uart0 and sets the gpio 
    printf("hello world!\n");  // prints hello world 
}
```

Lastly, modify the project root cmake file: `FlatHeadBro/CMakeLists.txt`: 
```
cmake_minimum_required(VERSION 3.10)
project(FlatHeadBro)

add_subdirectory(boot0-spl)
add_subdirectory(secondary/bootloadertest)
add_subdirectory(secondary/interrupttest)
add_subdirectory(secondary/supervisorinterrupttest)
add_subdirectory(secondary/mmutest)
# add your newly-added hello secondary program 
add_subdirectory(secondary/hello) 
```

Now you can go back to the build folder and type `make` again -- `cmake` will automatically rerun and after compilation, `build/bin` will contain your `hello.bin` (raw binary file), `hello.list` (objdump output), `hello.elf` (elf file) and `hello.elf.map` (memory map). 

Now use `uartuploader/my-install.py` to upload `hello.bin` to your board: 
```
# in build/ folder 
python3 ../uartuploader/my-install.py bin/hello.bin 
```
And it will print the hello message. 

## References 
- https://github.com/xboot/xfel
- https://github.com/smaeul/sun20i_d1_spl
- https://github.com/bigmagic123/d1-nezha-baremeta 
- https://mp.weixin.qq.com/s/MdrtuX3s1aUf0tdivXfpKQ
- https://whycan.com/t_6440.html 
- [Nezha description] https://linux-sunxi.org/Allwinner_Nezha#U-Boot
- [Lichee RV description] https://linux-sunxi.org/Sipeed_Lichee_RV 
- [D1 description from Fedora] https://fedoraproject.org/wiki/Architectures/RISC-V/Allwinner 
- [Boot SD card layout] https://linux-sunxi.org/Bootable_SD_card 
- [Description of the BROM for Allwinner ARM processors] https://linux-sunxi.org/BROM 
- [The boot0spl header format] https://linux-sunxi.org/EGON 
- [The FEL USB protocol] https://github.com/hno/Allwinner-Info/blob/master/FEL-usb/USB-protocol.txt 
- [The Display Engine 2 register guide] https://linux-sunxi.org/DE2_Register_Guide 
- [RISC-V assembly programming guide] https://github.com/riscv-non-isa/riscv-asm-manual/blob/master/riscv-asm.md 
- [RISC-V Non-ISA specification] https://github.com/riscv-non-isa 
- [RISC-V instructions] https://msyksphinz-self.github.io/riscv-isadoc/html/rvi.html 
- [A brief justification of RISC-V's calling convention] https://inst.eecs.berkeley.edu/~cs61c/resources/RISCV_Calling_Convention.pdf 















