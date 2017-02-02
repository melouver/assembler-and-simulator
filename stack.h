//
// Created by melouver on 2/1/17.
//

#ifndef ASM_AND_SIMU_STACK_H
#define ASM_AND_SIMU_STACK_H

struct Node;
typedef struct Node *PtrToNode;
typedef PtrToNode Stack;

typedef int ElementType;

int IsEmpty(Stack S);
Stack CreateStack();
ElementType Top(Stack S);
void MakeEmpty(Stack S);
void Push(ElementType X, Stack S);
void Pop(Stack S);

#endif //ASM_AND_SIMU_STACK_H
