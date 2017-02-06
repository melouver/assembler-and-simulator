#include <stdio.h>
#include <string.h>
#include "stack.h"

typedef struct _PROG_STATE_WORD {
    unsigned short overflow_flg: 1;
    unsigned short compare_flg: 1;
    unsigned short reserve: 14;
}PROG_STATE_WORD;

PROG_STATE_WORD PSW;

int main() {
    short a = 58;
    int b = a & 0x0F;

    printf("%c", a);

    return 0;
}