#include <stdio.h>
#include <string.h>
#include <bsd/string.h>

void go(char *data) {
    char name[10];
    char name2[10]
    char test[] = "testCodetettt";
    strlcpy(name, data, sizeof(name));
    strncpy(name2, data, sizeof(name));
    strlcat(name, test, sizeof(test));
    puts(name);
}

//gcc -std=c11 revised.c -lbsd

int main(int argc, char **argv) {
    go(argv[1]);
}