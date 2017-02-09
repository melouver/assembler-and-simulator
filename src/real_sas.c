//
// Created by melouver on 1/16/17.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "util/customized_print_functions.h"
#include "hashtbl/hashtbl.h"
#define MAX_LEN 1000
#define INSTRS_COUNT (sizeof(g_instrs_name) / sizeof(g_instrs_name[0]))
#define INSTR_SYM {"HLT", "JMP", "CJMP", "OJMP", "CALL", "RET",\
                   "PUSH", "POP", "LOADB", "LOADW", "STOREB", "STOREW", \
                   "LOADI", "NOP", "IN", "OUT", "ADD", "ADDI", "SUB", "SUBI",\
                   "MUL", "DIV", "AND", "OR", "NOR", "NOTB", "SAL", "SAR", \
                   "EQU", "LT", "LTE", "NOTC"}

#define instrs_format_macro "12222133444451667575777778778881"
/*
 * Instructions label array
 */

const char* g_instrs_name[] = INSTR_SYM;
/*
 * Instructions format
 */
HashTable LineHash;
HashTable DataHash;

const char instr_format[32] = instrs_format_macro;

int GetInstrCode(const char* op_sym);
unsigned long TransToCode(char* instr_line, int instr_num);
int GetRegNum(char* instr_line, char* reg_name);

int main(int argc, char *argv[]) {
    char a_line[MAX_LEN];
    char op_sym[8];
    int op_num;
    char *pcPos;
    FILE *pfIn, *pfOut;

    int n;
    if (argc < 2) {
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
/*
 * Build LineHash
 */

    LineHash = InitializeTable(100);
    char* column_ptr = NULL;
    char label_name[50];
    int line_no = 0;

    fgets(a_line, MAX_LEN, pfIn);
    while (!feof(pfIn)) {

        if ((pcPos = strchr(a_line, '#')) != NULL) {
            *pcPos = '\0';
        }
        n = sscanf(a_line, "%s", op_sym);
        if (n < 1) {
            fgets(a_line, MAX_LEN, pfIn);
            continue;
        }
        if (strcmp(op_sym, "BYTE") == 0 || strcmp(op_sym, "WORD") == 0) {
            fgets(a_line, MAX_LEN, pfIn);
            continue;
        }

        if ((column_ptr = strchr(a_line, ':')) != NULL) {
            *column_ptr = '\0';
            n = sscanf(a_line, "%s", label_name);
            Insert(label_name, LineHash, line_no*4);
        }

        line_no++;
        fgets(a_line, MAX_LEN, pfIn);
    }


    rewind(pfIn);




/*
 * Write Data SEGMENT and Build data hash table
 */
    FILE *data_f;
    if ((data_f = fopen("DATA_OUT", "w")) == NULL) {
        printf("ERROR: cannot open file %s for writing! \n", argv[2]);
        return 0;
    }
    DataHash = InitializeTable(100);
    int index = 0, offset = 0, unit = 1, count = 0, number;
    char *left_braket_pointer, *left_brace_pointer, *right_brace_pointer, *left_quote_ptr, *right_quote_ptr;
    char var_name[10];

    fgets(a_line, MAX_LEN, pfIn);
    while (!feof(pfIn)) {
        if ((pcPos = strchr(a_line, '#')) != NULL)
            *pcPos = '\0';
        n = sscanf(a_line, "%s %s", op_sym, var_name);

        if (n < 2) {
            fgets(a_line, MAX_LEN, pfIn);
            continue;
        }
        if (strcmp(op_sym, "BYTE") == 0) {
            unit = 1;
        } else if (strcmp(op_sym, "WORD") == 0) {
            unit = 2;
        } else {
            fgets(a_line, MAX_LEN, pfIn);
            continue;
        }
        
        int offset_tmp = offset;

        if ((left_braket_pointer = strchr(a_line, '[')) != NULL) {
            n = sscanf(left_braket_pointer + 1, "%d]", &count);
            if (n < 1) {
                printf("ERROR: invalid array capicity!\n");
            }
            *left_braket_pointer = '\0';
            n = sscanf(a_line, "%s %s", op_sym, var_name);
            *left_braket_pointer = '[';

            if ((left_brace_pointer = strchr(a_line, '{')) != NULL) {
                if ((right_brace_pointer = strchr(a_line, '}')) != NULL) {
                    int written_count = 0;
                    //  {  } exists
                    // e.g. BYTE cell[4] = {1, 2, 3, 4} PASSED
                    *right_brace_pointer = '\0';
                    char *token = strtok(left_brace_pointer + 1, ",");
                    while (token) {
                        n = sscanf(token, "%i", &number);
                        if (n < 1) {
                            printf("ERROR: INVALID INITIALIZER LIST\n");
                        }
                        if (unit == 1) {
                            byte_print(&index, "%02x", &number, data_f, &offset);
                        } else {
                            int tmp = number & 0x0F;
                            byte_print(&index, "%02x", &tmp, data_f, &offset);
                            tmp = number & 0x0F0;
                            byte_print(&index, "%02x", &tmp, data_f, &offset);
                        }
                        written_count++;
                        token = strtok(NULL, ",");
                    }
                    // init list shorter than expected
                    // e.g. BTYE cell[4] = {1} PASSED
                    if (written_count < count) {
                        for (int i = 0; i < count - written_count; ++i) {
                            if (unit == 1) {
                                byte_print(&index, "00", NULL, data_f, &offset);
                            } else {
                                byte_print(&index, "00", NULL, data_f, &offset);
                                byte_print(&index, "00", NULL, data_f, &offset);
                            }
                        }
                    }
                }
            } else if ((left_quote_ptr = strchr(a_line, '"')) != NULL) {
                if ((right_quote_ptr = strchr(left_quote_ptr+1, '"')) != NULL) {
                    // E.G. BYTE cell[4] = "1234"
                    if (unit != 1) {
                        printf("ERROR: should declare byte with \" \" ");
                        exit(-1);
                    }
                    int written_count = 0;
                    char *ptr = left_quote_ptr+1;
                    int num;
                    while (ptr != right_quote_ptr) {
                        num = *ptr;
                        byte_print(&index, "%02x", &num, data_f, &offset);
                        ptr++;
                        written_count++;
                    }
                    // E.G. BYTE cell[4] = "12"
                    if (written_count < count) {
                        for (int i = 0; i < count - written_count; ++i) {
                            byte_print(&index, "00", NULL, data_f, &offset);
                        }
                    }
                }
            } else {
                // no { } exists, but [] exists, init with 0
                // e.g. BYTE cell[10] PASSED
                for (int i = 0; i < count; ++i) {
                    if (unit == 1) {
                        byte_print(&index, "00", NULL, data_f, &offset);
                    } else {
                        byte_print(&index, "00", NULL, data_f, &offset);
                        byte_print(&index, "00", NULL, data_f, &offset);
                    }
                }
            }
        } else {
            // no [ ] , only one variable
            char *equ_pointer;
            if ((equ_pointer = strchr(a_line, '=')) != NULL) {
                // e.g. BYTE cell = 3 PASSED
                n = sscanf(equ_pointer + 1, "%i", &number);
                *equ_pointer = '\0';
                n = sscanf(a_line, "%s %s", op_sym, var_name);

                *equ_pointer = '=';
                if (n < 1) {
                    printf("ERROR: no initialize value\n");
                }
                if (unit == 1) {
                    byte_print(&index, "%02x", &number, data_f, &offset);
                } else {
                    int tmp = number & 0x0F;
                    byte_print(&index, "%02x", &tmp, data_f, &offset);
                    tmp = number & 0x0F0;
                    byte_print(&index, "%02x", &tmp, data_f, &offset);
                }
            } else {
                // e.g. BYTE cell PASSED
                n = sscanf(a_line, "%s %s", op_sym, var_name);
                if (unit == 1) {
                    byte_print(&index, "00", NULL, data_f, &offset);
                } else {
                    byte_print(&index, "00", NULL, data_f, &offset);
                    byte_print(&index, "00", NULL, data_f, &offset);
                }
            }


        }

        Insert(var_name, DataHash, offset_tmp);

        fgets(a_line, MAX_LEN, pfIn);
    }

    if (index > 0) {
        for (int i = 0; i < 4-index; ++i) {
            fprintf(data_f, "00");
        }
        fprintf(data_f, "\n");
    }

    fprintf(data_f, "0x%08x\n", offset);

    rewind(pfIn);

/*
 * TransToCode
 */
    fgets(a_line, MAX_LEN, pfIn);
    while (!feof(pfIn)) {
        if ((pcPos = strchr(a_line, '#')) != NULL) {
            *pcPos = '\0';
        }

        if ((column_ptr = strchr(a_line, ':')) != NULL) {
            n = sscanf(column_ptr+1, "%s", op_sym);
        }else {
            n = sscanf(a_line, "%s", op_sym);
        }

        if (n < 1) {
            fgets(a_line, MAX_LEN, pfIn);
            continue;
        }

        if (strcmp(op_sym, "BYTE") == 0 || strcmp(op_sym, "WORD") == 0) {
            fgets(a_line, MAX_LEN, pfIn);
            continue;
        }

        op_num = GetInstrCode(op_sym);
        if (op_num > 31) {
            printf("ERROR: %s is an invalid instruction!\n", a_line);
            exit(-1);
        }
        fprintf(pfOut, "0x%08lx\n", TransToCode(column_ptr?column_ptr+1:a_line, op_num));
        fgets(a_line, MAX_LEN, pfIn);
    }
    fclose(data_f);
    FILE* last_file;

    if ((last_file = fopen("DATA_OUT", "r")) == NULL) {
        printf("ERROR: can't open file %s", "DATA_OUT");
        exit(-1);
    }

    unsigned int ins;

    fscanf(last_file, "%i", &ins);
    while (!feof(last_file)) {
        fprintf(pfOut, "0x%08lx\n", ins);
        fscanf(last_file, "%i", &ins);
    }

    fclose(last_file);
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
    char label[10];
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
            n = sscanf(instr_line, "%s %s", op_sym, label);
            if (n < 2) {
                printf("ERROR: invalid instruction format! %s\n", instr_line);
                exit(-1);
            }
            addr = GetOffset(Find(label, LineHash));
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
            n = sscanf(instr_line, "%s %s %s", op_sym, reg0, label);
            if (n < 3) {
                printf("ERROR: invalid instruction format! %s\n", instr_line);
                exit(-1);
            }
            op_code = GetInstrCode(op_sym);
            addr = GetOffset(Find(label, DataHash));
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
            instr_code = (op_code << 27) | (arg1 << 24) | (immediate & 0x0FFFF);
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
            instr_code = (op_code << 27) | (arg1 << 24) | (port & 0x0FF);
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
            instr_code = (op_code << 27) | (arg1 << 24) | (arg2 << 20) | (arg3 << 16);
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












