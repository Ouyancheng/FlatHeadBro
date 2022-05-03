# FlatHeadBro

This is an experiment to run baremetal programs on Lichee RV or D1 Nezha development boards. 

The boot0 program is loaded from the SD card, and it will further load secondary program from UART, instead of downloading using FEL. 

## boot0-spl 

This is the first program to run after powering on and going through the BROM phrase. 

### Build requirement: 

- cmake 
- riscv64-unknown-elf-gcc or clang (with riscv64 target support) 
- python3 

### Build instructions: 

#### Using riscv64 gcc 

If using riscv64-unknown-elf-gcc, modify `cmake/toolchain.cmake` to specify your path to riscv64-unknown-elf-gcc. 
```
mkdir build 
cd build 
cmake ../boot0-spl --toolchain ../cmake/toolchain.cmake
make 
```
#### Using clang 
If using clang, modify `cmake/clang-toolchain.cmake` to specify your path to clang (Note: if you are using macOS, Apple's clang shipped with Command Line Tools will NOT work, please install the LLVM toolchain via homebrew: `brew install llvm`, and make sure your clang has the riscv64 target: `${YOUR_PATH_TO_CLANG}/clang --print-targets`). 
```
mkdir build 
cd build 
cmake ../boot0-spl --toolchain ../cmake/clang-toolchain.cmake
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

## References 
- https://github.com/xboot/xfel
- https://github.com/smaeul/sun20i_d1_spl
- https://github.com/bigmagic123/d1-nezha-baremeta 
- https://mp.weixin.qq.com/s/MdrtuX3s1aUf0tdivXfpKQ
- https://whycan.com/t_6440.html 
- https://linux-sunxi.org/Allwinner_Nezha#U-Boot




















