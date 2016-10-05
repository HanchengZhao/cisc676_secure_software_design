#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    size_t bytes = 256;
    char* p2chunk = (char *)malloc(bytes);
    printf("%p\n", (void*) p2chunk);
    getline(&p2chunk, &bytes, stdin);
    free(p2chunk);
    char* anotherchunk = (char *)calloc(bytes);
    // memset(anotherchunk,'A',256);
    // int i=0;
    // for(;i<256;i+=4){
    //     printf("%08x", *((int*) (anothercunk+i)));
    // }
    // printf("\n");
    // printf("%s\n", anotherchunk);
    
    // for(int i; i <)
    printf("%p\n", (void*) anotherchunk);
    free(anotherchunk);
}