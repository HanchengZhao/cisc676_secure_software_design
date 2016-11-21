#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void show_chunk_hex(void* start, size_t bytes){
    int i=0;
    for(;i<bytes;i+=1){
        printf("%02x", *((unsigned char*) (start+i)));
    }
    printf("\n");
};

int main(int argc, char **argv) {
    size_t bytes = 8;
    size_t header = 16;
    int nchunks = 12;
    unsigned char filler = 0x01;
    void* chunks[nchunks];
    int i=0;
    for(; i < nchunks; i++){
        chunks[i] = malloc(bytes);
        filler += 0x09;
        memset(chunks[i], filler, bytes);
        printf("chunk %d at %p:\n", i, chunks[i]);
        show_chunk_hex(chunks[i]-header, bytes+header);
        if (i> 0){
            printf("previous byte: %02x\n", *((unsigned char*) (chunks[i]-header-1)));
        }
    }
}