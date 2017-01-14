//
// Created by melouver on 1/13/17.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
unsigned long* PC;
short GR[8];
PROG_STATE_WORD PSW;

unsigned long IR;

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
    unsigned long instruction;
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

    PC = (unsigned long *) MEM;
    if ((pfIn = fopen(argv[2], "r")) == NULL) {
        printf("ERROR: cannot open file %s for reading!\n", argv[2]);
        exit(-1);
    }

    while (!feof(pfIn)) {
        fscanf(pfIn, "%li", &instruction);
        memcpy(PC, &instruction, sizeof(instruction));
        PC++;
    }

    fclose(pfIn);
    PC = (unsigned long *) MEM;
    while (ret) {
        IR = *PC;
        PC++;
        ret = (*ops[OPCODE])();
    }

    free(MEM);

    return 0;

}

int HTL() {
    return 0;
}

int JMP() {
    PC = (unsigned long*)(MEM + ADDRESS);
    return 1;
}

int CJMP() {
    if (PSW.compare_flg)
        PC = (unsigned long*)(MEM + ADDRESS);
    return 1;
}

int OJMP() {
    if (PSW.overflow_flg)
        PC = (unsigned long*)(MEM + ADDRESS);
    return 1;
}

int LOAD() {
    //TODO: change to *(short*)(MEM + ADDRESS)
    GR[REG0] = (short )(* (unsigned long)(MEM + ADDRESS));
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
    }else if (GR[REG2 < 0]) {
        PSW.overflow_flg = GR[REG0] > GR[REG1] ? 0 : 1;
    }

    return 1;
}

int IN() {
    
}




















