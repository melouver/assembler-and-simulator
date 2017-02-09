# Usage
You can get sas and ssim either from my provided binary or build it from source.

# Requirements
## CMake 
## GCC 

# Build
$ cd asm_and_simu
$ mkdir bin
$ cd bin
$ cmake -DCMAKE_BUILD_TYPE=DEBUG ../src
$ make
$ ./asm ../asm_src/queen.txt queen.dat
$ ./asm ../asm_src/src2 src2.dat
$ ./ssim queen.dat
$ ./ssim src2.dat
