#include <stdio.h>
#include <string.h>
#include "stack.h"


int main() {
    short G[8] = {1,2,3,4,5,6,7,8};
    unsigned char MEM[100];
    unsigned char* ptr = MEM;
    unsigned int i = 3;
    unsigned int* PC = &i;
    memcpy(ptr, G, sizeof(short)*8);
    ptr += sizeof(short)*8;
    memcpy(ptr, PC, sizeof(unsigned int));
    ptr += sizeof(unsigned int);

    i = 0;
    for (int i = 0; i < 8; ++i) {
        G[i] = 0;
    }
    ptr -= sizeof(unsigned int);
    memcpy(PC, ptr, sizeof(unsigned int));

    ptr -= sizeof(short) * 8;
    memcpy(G, ptr, sizeof(short) * 8);




    return 0;
}