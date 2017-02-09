set(util_source_files
        customized_print_functions.c
        customized_print_functions.h)

add_library(util ${util_source_files})

target_link_libraries(util hashtbl)
