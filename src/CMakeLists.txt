cmake_minimum_required(VERSION 3.6)
project(asm_and_simu C)

add_subdirectory(hashtbl)
add_subdirectory(util)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -lm")

set(asm_src_files real_sas.c)
add_executable(asm ${asm_src_files})
target_link_libraries(asm util)

set(ssim_src_files real_ssim.c)
add_executable(ssim ${ssim_src_files})



