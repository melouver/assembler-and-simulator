//
// Created by melouver on 1/14/17.
//

#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#define MAXLINE 1000

int getline(char s[], int lim);
void copy(char to[], char from[]);

int main () {
    FILE* pfIn;
    pfIn = fopen("out", "r");
    unsigned int instruction;
    fscanf(pfIn, "0x%u", &instruction);
    printf("%u", instruction);
    fscanf(pfIn, "0x");
    fscanf(pfIn, "%u", &instruction);
    printf("%u", instruction);
    return 0;
}

int getline(char s[], int lim) {
    int c, i, j;

    for (i = 0; i < lim-1 && (c = getchar()) != EOF && c != ' ' && c != '\n'; ++i) {
        s[i] = c;
    }

    if (c == '\n') {
        s[i] = c;
        ++i;
    }
    s[i] = '\0';
    return i;
}

void copy(char to[], char from[]) {
    int i;

    i = 0;
    while ((to[i] = from[i]) != '\0')
        i++;
}





















