//
// Created by melouver on 1/14/17.
//

#include <stdio.h>
#include <ctype.h>

#define MAXLINE 1000

int getline(char s[], int lim);
void copy(char to[], char from[]);

int main () {
    int len;
    char line[MAXLINE];

    while ((len = getline(line, MAXLINE)) >= 0) {
        if (len > 0)
            printf("%s\n", line);
    }
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





















