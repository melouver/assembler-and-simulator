cmake_minimum_required(VERSION 3.6)
project(asm_and_simu C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

set(SAS_SOURCE_FILES real_sas.c hashtbl.c customized_print_functions.c)
add_executable(asm ${SAS_SOURCE_FILES})

set(SIMU_SOURCE_FILES real_ssim.c)
add_executable(ssim ${SIMU_SOURCE_FILES})


