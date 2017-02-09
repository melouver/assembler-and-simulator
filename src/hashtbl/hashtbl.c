//
// Created by melouver on 1/24/17.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hashtbl.h"

struct ListNode {
    ElementType Element;
    int offset;
    Position Next;
};

typedef Position List;
typedef unsigned int hash_index;

struct HashTbl {
    int TableSize;
    List *TheLists;
};

hash_index Hash(const char* Key, int TableSize) {
    unsigned int HashVal = 0;

    while (*Key != '\0') {
        HashVal += *Key++;
    }

    return HashVal % TableSize;
}

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

Position Find(ElementType Key, HashTable H) {
    Position P;
    List L;

    L = H->TheLists[Hash(Key, H->TableSize)];
    P = L->Next;
    while (P != NULL && strcmp(Key, P->Element) != 0) {
        P = P->Next;
    }

    return P;
}

void Insert(ElementType Key, HashTable H, int offset) {
    Position Pos, NewCell;
    List L;

    Pos = Find(Key, H);
    if (Pos == NULL) {
        NewCell = (Position)malloc(sizeof(struct ListNode));
        if (NewCell == NULL) {
            printf("Out of space!\n");
        }else {
            L = H->TheLists[Hash(Key, H->TableSize)];
            NewCell->Next = L->Next;
            NewCell->Element = (char*)malloc(sizeof(50));
            strcpy(NewCell->Element, Key);
            NewCell->offset = offset;
            L->Next = NewCell;
        }

    }
}


void PrintOffset(Position P) {
    if (P == NULL) {
        printf("Not found");
        return;
    }

    printf("Offset is %d", P->offset);
}


int GetOffset(Position P) {
    return P->offset;
}





















