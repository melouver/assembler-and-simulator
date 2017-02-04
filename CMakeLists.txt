cmake_minimum_required(VERSION 3.6)
project(asm_and_simu C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

set(SOURCE_FILES  real_ssim.c)
add_executable(asm_and_simu ${SOURCE_FILES})