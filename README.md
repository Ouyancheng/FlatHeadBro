# FlatHeadBro

This is an experiment to run baremetal programs on Lichee RV or D1 Nezha development boards. 

The program is loaded from the SD card, instead of downloading via FEL. 

## boot0-spl 

This is the first program to run after powering on and going through the BROM phrase. 

### Build requirement: 

- cmake 
- riscv64-unknown-elf-gcc or clang (with riscv64 target support) 
- python3 

### Build instructions: 

#### Using riscv64 gcc 

If using riscv64-unknown-elf-gcc, modify `boot0-spl/cmake/toolchain.cmake` to specify your path to riscv64-unknown-elf-gcc. 
```
cd boot0-spl 
mkdir build 
cd build 
cmake .. --toolchain ../cmake/toolchain.cmake
make 
```

If using clang, modify `boot0-spl/cmake/clang-toolchain.cmake` to specify your path to clang (Note: if you are using macOS, Apple's clang shipped with Command Line Tools will NOT work, please install the LLVM toolchain via homebrew: `brew install llvm`, and make sure your clang has the riscv64 target: `${YOUR_PATH_TO_CLANG}/clang --print-targets`). 
```
cd boot0-spl 
mkdir build 
cd build 
cmake .. --toolchain ../cmake/clang-toolchain.cmake
make 
```

### Running the boot0-spl 

After your build, in the build directory you will get a binary file `boot0-sdcard.bin`, and that's the file that will be flashed to the SD card. 

```
# In the build directory 
sudo dd if=boot0-sdcard.bin of=/dev/<YOUR_SD_CARD_DEVICE> bs=4096 seek=2
sync 
```

For macOS, you may want to unmount the disk before dd-ing: 
```
# In the build directory 
diskutil unmountDisk <YOUR_SD_CARD_DEVICE> 
sudo dd if=boot0-sdcard.bin of=/dev/<YOUR_SD_CARD_DEVICE> bs=4096 seek=2
sync 
diskutil unmountDisk <YOUR_SD_CARD_DEVICE> 
``` 

Then insert your SD card to the Lichee RV and power it on (could be done via GPIO pins), the UART will print the following, use `screen /dev/cu.SLAB_USBtoUART 115200` to monitor UART, of course with a USB to UART dongle: 
```
Hello World!!!
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

And then it will echo anything you type into the UART terminal. 
If you connect an LED to GPIO PE16, you will also see the LED blinking each time you type a character to the UART terminal. 




















