//
// Created by melouver on 1/24/17.
//

#ifndef ASM_AND_SIMU_HASHTBL_H
#define ASM_AND_SIMU_HASHTBL_H


typedef char* ElementType;
struct ListNode;
typedef struct ListNode *Position;
struct HashTbl;
typedef struct HashTbl* HashTable;

HashTable InitializeTable(int TableSize);
void DestroyTable(HashTable H);
Position Find(ElementType Key, HashTable H);
void Insert(ElementType Key, HashTable H, int offset);
ElementType Retrieve(Position P);
void PrintOffset(Position P);
#endif //ASM_AND_SIMU_HASHTBL_H
