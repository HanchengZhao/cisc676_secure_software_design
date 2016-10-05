#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    size_t bytes = 1;
    int* p2chunk = (int *) calloc(1,bytes);
    printf("%p p2chunk\n", (void*)p2chunk);
    p2chunk[0] = 1;
    p2chunk[1] = 2;
    p2chunk[2] = 3;
    p2chunk[3] = 4;
    printf("%d %d %d %d\n", p2chunk[0], p2chunk[1], p2chunk[2], p2chunk[3]);
    free(p2chunk);
}