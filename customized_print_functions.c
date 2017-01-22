//
// Created by melouver on 1/22/17.
//

#include <stdio.h>
#include "customized_print_functions.h"

void byte_print(int *index, const char* format, void* arg, FILE* pfOut) {
    if (*index == 0) {
        fprintf(pfOut, "0x");
    }
    if (arg != NULL) {
        fprintf(pfOut, format, *(int*)arg);
    }else {
        fprintf(pfOut, format);
    }
    if (*index == 3) {
        fprintf(pfOut, "\n");
    }
    *index = (*index + 1) % 4;
}