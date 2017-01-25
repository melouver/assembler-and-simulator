#include <iostream>
#include "hashtbl.h"

int main() {
    HashTable H = InitializeTable(100);
    char *input_str;
    fgets(input_str, 10, STDIN_FILENO);
    Insert(input_str, H);
    




    return 0;
}