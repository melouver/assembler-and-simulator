//
// Created by melouver on 1/24/17.
//

#include <stdlib.h>
#include <stdio.h>

#include "hashtbl.h"

struct ListNode {
    ElementType Element;
    Position Next;
};

typedef Position List;

struct HashTbl {
    int TableSize;
    List *TheLists;
};

HashTable InitializeTable(int TableSize) {
    HashTable H;
    int i;

    H = malloc(sizeof(struct HashTbl));
    if (H == NULL) {
        printf("ERROR: Out of space!\n");
        exit(-1);
    }
    H->TableSize = TableSize;
    H->TheLists = (List *)malloc(sizeof(struct ListNode) * H->TableSize);
    if (H->TheLists == NULL) {
        printf("ERROR: Out of space!\n");
        exit(-1);
    }

    H->TheLists[0] = (struct ListNode *)malloc(sizeof(struct ListNode) * H->TableSize);

    for (int i = 1; i < H->TableSize; ++i) {
        H->TheLists[i] = H->TheLists[0] + i;
        H->TheLists[i]->Next = NULL;
    }

    return H;
}








