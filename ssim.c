//
// Created by melouver on 1/13/17.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define REG0 ((IR >> 24) & 0x0F)
#define REG1 ((IR >> 20) & 0x0F)
#define REG2 ((IR >> 16) & 0x0F)
#define IMMEDIATE (IR & 0xFFFF)
#define ADDRESS (IR & 0xFFFFFF)
#define PORT (IR & 0xFF)
#define OPCODE ((IR >> 28) & 0x0F)

typedef struct _PROG_STATE_WORD {
    unsigned short overflow_flg: 1;
    unsigned short compare_flg: 1;
    unsigned short reserve : 14;
}PROG_STATE_WORD;

unsigned char* MEM;
unsigned int* PC;
short GR[8];
PROG_STATE_WORD PSW;

unsigned int IR;

int HLT();
int JMP();
int CJMP();
int OJMP();
int LOAD();
int STORE();
int LOADI();
int NOP();
int ADD();
int SUB();
int IN();
int OUT();
int EQU();
int LT();
int LTE();
int NOT();

int main(int argc, char* argv[]) {
    unsigned int instruction;
    unsigned long mem_size;
    int (*ops[])() =  {HLT, JMP, CJMP, OJMP, LOAD, STORE, LOADI,
                       NOP, ADD, SUB, IN, OUT, EQU, LT, LTE, NOT};

    FILE *pfIn;
    int ret, n;

    ret = 1;

    if (argc < 3) {
        printf("ERROR: no enough command line arguments!\n");
        exit(-1);
    }

    n = sscanf(argv[1], "%li", &mem_size);
    if (n < 1) {
        printf("ERROR: argument %s is an invalid number! \n", argv[1]);
        exit(-1);
    }

    /*
     * set aside memory
     */

    if ((MEM = (unsigned char *) malloc(sizeof(char) * mem_size)) == NULL) {
        printf("ERROR: failed to allocate memory !\n");
        exit(-1);
    }

    PC = (unsigned int *) MEM;
    if ((pfIn = fopen(argv[2], "r")) == NULL) {
        printf("ERROR: cannot open file %s for reading!\n", argv[2]);
        exit(-1);
    }
    /*
     * copy instructions into memory
     */
    fscanf(pfIn, "%i", &instruction);
    while (!feof(pfIn)) {
        memcpy(PC, &instruction, sizeof(instruction));
        PC++;
        fscanf(pfIn, "%i", &instruction);
    }
    fclose(pfIn);


    PC = (unsigned int *)MEM;
    printf("begin while loop!\n");
    while (ret) {
        //printf("in loop\n");
        IR = *PC;
        //printf("IR IS %x\n", IR);
        PC++;
        ret = (*ops[OPCODE])();
    }

    free(MEM);

    return 0;
}


int JMP() {
    //printf("ADDRESS IS %u", ADDRESS);
    PC = (unsigned int*)(MEM + ADDRESS);
    //printf("PC NOW POINTS TO %u", *PC);

    return 1;
}

int CJMP() {
    if (PSW.compare_flg)
        PC = (unsigned int*)(MEM + ADDRESS);
    return 1;
}

int OJMP() {
    if (PSW.overflow_flg)
        PC = (unsigned int*)(MEM + ADDRESS);
    return 1;
}

int LOAD() {
    //TODO: change to *(short*)(MEM + ADDRESS)
    GR[REG0] = (short )(* (unsigned int*)(MEM + ADDRESS));
    return 1;
}

int STORE() {
    *(short *)(MEM + ADDRESS) = GR[REG0];
    return 1;
}

int LOADI() {
    GR[REG0] = (short)IMMEDIATE;
    return 1;
}

int NOP() {
    return 1;
}

int ADD() {
    GR[REG0] = GR[REG1] + GR[REG2];
    if (GR[REG2] >= 0) {
        PSW.overflow_flg = GR[REG0] < GR[REG1] ? 1 : 0;
    }else if (GR[REG2] < 0) {
        PSW.overflow_flg = GR[REG0] > GR[REG1] ? 1 : 0;
    }
    return 1;
}

int SUB() {
    GR[REG0] = GR[REG1] - GR[REG2];

    if (GR[REG2] >= 0) {
        PSW.overflow_flg = GR[REG0] > GR[REG1] ? 1 : 0;
    }else if (GR[REG2] < 0) {
        PSW.overflow_flg = GR[REG0] > GR[REG1] ? 0 : 1;
    }

    return 1;
}

int IN() {
    char ch;
    ch = getchar();
    GR[REG0] = (short)(ch & 0x00FF);
    //printf("GOT %u\n", GR[REG0]);
    return 1;
}

int OUT() {
    putchar((char)GR[REG0]);
    //printf("WRITE %u\n", GR[REG0]);
    return 1;
}

int EQU() {
    PSW.compare_flg = (GR[REG0] == GR[REG1]);
    return 1;
}

int LT() {
    PSW.compare_flg = (GR[REG0] < GR[REG1]);
    return 1;
}

int LTE() {
    PSW.compare_flg = (GR[REG0] <= GR[REG1]);
    return 1;
}

int NOT() {
    PSW.compare_flg = !PSW.compare_flg;
    return 1;
}

int HLT() {
    return 0;
}












