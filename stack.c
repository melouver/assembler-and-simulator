//
// Created by melouver on 2/1/17.
//



#include <stdio.h>
#include "stack.h"
#include <stdlib.h>

struct Node {
    ElementType Element;
    PtrToNode Next;
};


int IsEmpty(Stack S) {
    return S->Next == NULL;
}

Stack CreateStack() {
    Stack S;

    S = (struct Node*)malloc(sizeof(struct Node));
    if (S == NULL) {
        printf("Out of space");
        exit(-1);
    }
    S->Next = NULL;
    MakeEmpty(S);

    return S;
}

ElementType Top(Stack S) {
    if (!IsEmpty(S))
        return S->Next->Element;
    printf("Empty stack!");
    exit(-1);
}

void MakeEmpty(Stack S) {
    if (S == NULL) {
        printf("Must call CreateStack first\n");
        exit(-1);
    }
    while (!IsEmpty(S))
        Pop(S);
}

void Push(ElementType X, Stack S) {
    PtrToNode tempCell;

    tempCell = (struct Node*)malloc(sizeof(struct Node));
    if (tempCell == NULL) {
        printf("out of space\n");
        exit(-1);
    }

    tempCell->Element = X;
    tempCell->Next = S->Next;
    S->Next = tempCell;
}


void Pop(Stack S) {
    PtrToNode firstCell;
    if (S->Next == NULL)
        printf("Empty Stack");

    firstCell= S->Next;
    S->Next = firstCell->Next;
    free(firstCell);
}