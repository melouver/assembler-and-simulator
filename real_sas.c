//
// Created by melouver on 1/16/17.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEN 80
#define INSTRS_COUNT (sizeof(g_instrs_name) / sizeof(g_instrs_name[0]))
#define INSTR_SYM {"HLT", "JMP", "CJMP", "OJMP", "CALL", "RET",\
                   "PUSH", "POP", "LOADB", "LOADW", "STOREB", "STOREW", \
                   "LOADI", "NOP", "IN", "OUT", "ADD", "ADDI", "SUB", "SUBI",\
                   "MUL", "DIV", "ADN", "OR", "NOR", "NOTB", "SAL", "SAR", \
                   "EQU", "LT", "LTE", "NOTC"}

#define instrs_format_macro "12222133444451667575777778778881"
/*
 * Instructions label array
 */

const char* g_instrs_name[] = INSTR_SYM;
/*
 * Instructions format
 */

const char instr_format[32] = instrs_format_macro;

int GetInstrCode(const char* op_sym);
unsigned long TransToCode(char* instr_line, int instr_num);
int GetRegNum(char* instr_line, char* reg_name);

int main(int argc, char *argv[]) {
    char a_line[MAX_LEN];
    char op_sym[8];
    int op_num;
    char* pcPos;
    FILE *pfIn, *pfOut;

    int n;
    if (argc < 3) {
        printf("ERROR: no enough command line arguments!\n");
        return 0;
    }

    if ((pfIn = fopen(argv[1], "r")) == NULL) {
        printf("ERROR: cannot open file %s for reading! \n", argv[1]);
        return 0;
    }

    if ((pfOut = fopen(argv[2], "w")) == NULL) {
        printf("ERROR: cannot open file %s for writing! \n", argv[2]);
        return 0;
    }

    fgets(a_line, MAX_LEN, pfIn);
    while (!feof(pfIn)) {
        if ((pcPos = strchr(a_line, '#')) != NULL) {
            *pcPos = '\0';
        }

        n = sscanf(a_line, "%s", op_sym);

        if (n < 1) {
            continue;
        }

        op_num = GetInstrCode(op_sym);
        if (op_num > 31) {
            printf("ERROR: %s is an invalid instruction!\n", a_line);
            exit(-1);
        }

        fprintf(pfOut, "0x%08lx\n", TransToCode(a_line, op_num));
        fgets(a_line, MAX_LEN, pfIn);
    }

    fclose(pfIn);
    fclose(pfOut);


    return 0;
}

int GetInstrCode(const char* op_sym) {
    int i;

    for (i = 0; i < INSTRS_COUNT; ++i) {
        if (strcmp(op_sym, g_instrs_name[i]) == 0)
            break;
    }

    return i;
}

unsigned long TransToCode(char* instr_line, int instr_num) {
    unsigned long op_code, arg1, arg2, arg3, addr, instr_code;
    char op_sym[8], reg0[8], reg1[8], reg2[8];
    int immediate, port, n;

    instr_code = 0ul;

    switch (instr_format[instr_num]) {
        case '1':
            /*
             * HLT RET NOP NOTC
             */
            op_code = instr_num;
            instr_code = op_code << 27;
            break;
        case '2':
            /*
             * JMP CJMP OJMP CALL
             */
            n = sscanf(instr_line, "%s 0x%lx", op_sym, &addr);
            if (n < 2) {
                printf("ERROR: invalid instruction format! %s\n", instr_line);
                exit(-1);
            }
            op_code = GetInstrCode(op_sym);
            instr_code = (op_code << 27) | (addr & 0x0FFFFFF);
            break;
        case '3':
            /*
             * PUSH POP
             */
            n = sscanf(instr_line, "%s %s", op_sym, reg0);
            if (n < 2) {
                printf("ERROR: invalid instruction format! %s\n", instr_line);
                exit(-1);
            }
            op_code = GetInstrCode(op_sym);
            arg1 = GetRegNum(instr_line, reg0);
            instr_code = (op_code << 27) | (arg1 << 24);
            break;
        case '4':
            /*
             * LOADB LOADW STOREB STOREW
             */
            n = sscanf(instr_line, "%s %s 0x%lx", op_sym, reg0, &addr);
            if (n < 3) {
                printf("ERROR: invalid instruction format! %s\n", instr_line);
                exit(-1);
            }
            op_code = GetInstrCode(op_sym);
            arg1 = GetRegNum(instr_line, reg0);
            instr_code = (op_code << 27) | (arg1 << 24) | (addr & 0x0ffffff);
            break;
        case '5':
            /*
             * LOADI ADDI SUBI
             */
            n = sscanf(instr_line, "%s %s %i", op_sym, reg0, &immediate);
            if (n < 3) {
                printf("ERROR: invalid instruction format! %s\n", instr_line);
                exit(-1);
            }
            op_code = GetInstrCode(op_sym);
            arg1 = GetRegNum(instr_line, reg0);
            instr_code = (op_code << 27) | (arg1 << 24) | (immediate & 0x0FFF);
            break;
        case '6':
            /*
             * IN OUT
             */
            n = sscanf(instr_line, "%s %s %i", op_sym, reg0, &port);
            if (n < 3) {
                printf("ERROR: invalid instruction format! %s\n", instr_line);
                printf("ERROR: invalid instruction format!\n");
                exit(-1);
            }
            op_code = GetInstrCode(op_sym);
            arg1 = GetRegNum(instr_line, reg0);
            instr_code = (op_code << 27) | (arg1 << 24) | (port & 0x0ff);
            break;
        case '7':
            /*
             * ADD SUB MUL DIV AND OR NOR SAL SAR
             */
            n = sscanf(instr_line, "%s %s %s %s", op_sym, reg0, reg1, reg2);
            if (n < 4) {
                printf("ERROR: invalid instruction format! %s\n", instr_line);
                exit(-1);
            }
            op_code = GetInstrCode(op_sym);
            arg1 = GetRegNum(instr_line, reg0);
            arg2 = GetRegNum(instr_line, reg1);
            arg3 = GetRegNum(instr_line, reg2);
            instr_code = (op_code << 27) | (arg1 < 24) | (arg2 << 20) | (arg3 << 16);
            break;
        case '8':
            /*
             * NOTB EQU LT LTE
             */
            n = sscanf(instr_line, "%s %s %s", op_sym, reg0, reg1);
            if (n < 3) {
                printf("ERROR: invalid instruction format! %s\n", instr_line);
                exit(-1);
            }
            op_code = GetInstrCode(op_sym);
            arg1 = GetRegNum(instr_line, reg0);
            arg2 = GetRegNum(instr_line, reg1);
            instr_code = (op_code << 27) | (arg1 << 24) | (arg2 << 20);
            break;
        case '9':
            /*
             * B
             */
    }
    return instr_code;
}


int GetRegNum(char* instr_line, char* reg_name) {
    int reg_num;
    if (tolower(*reg_name) == 'z')
        reg_num = 0;
    else if (tolower(*reg_name) >= 'a' && tolower(*reg_name) <= 'g') {
        reg_num = tolower(*reg_name) - 'a' + 1;
    }
    else {
        printf("ERROR: invalid register name in %s! register name is %s", instr_line, reg_name);
        exit(-1);
    }

    return reg_num;
}












