//
// Created by melouver on 1/22/17.
//

#include <stdio.h>
#include "customized_print_functions.h"

void print_whether_arg_exists(FILE* pfOut, const char *format, void *arg) {
    if (arg != NULL) {
        fprintf(pfOut, format, *(int*)arg);
    }else {
        fprintf(pfOut, format);
    }
}

void byte_print(int *index, const char* format, void* arg, FILE* pfOut) {
    if (*index == 0) {
        fprintf(pfOut, "0x");
    }
    print_whether_arg_exists(pfOut, format, arg);
    if (*index == 3) {
        fprintf(pfOut, "\n");
    }
    *index = (*index + 1) % 4;
}
