#include <stdio.h>
#include <string.h>
#include <stdlib.h>
void show_chunk_hex(void* start, size_t bytes){
    int i=0;
    for(;i<bytes;i+=1){
        printf("%04x", *((unsigned char*) (start+i)));
    }
    printf("\n");
};

int main(int argc, char **argv){
    int x = 1000;
    void * space = malloc(8);
    memset(space,0x12,8);
    show_chunk_hex(space-2, sizeof(int));
    return 0;
}