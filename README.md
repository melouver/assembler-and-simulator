# Usage
You can get sas and ssim either from my provided binary or build it from source.

# Requirements

- CMake

- GCC

No specific requirements on the version

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

这是学校的某次课程设计，参照自CMU15xxx某个课程的实验(该课程似乎讲的是系统编程，不过现在已经不开设了)。我们需要实现一个汇编器来将汇编代码转换成可被模拟器识别的机器码。然后在我们实现的模拟器中，读取指令，翻译指令，执行指令，三步走，最终的测试结果取决于能否正确给出八皇后的汇编实现的运行结果。这个汇编指令集是一个比较简单的，足以完成我们此次试验的试验品。
