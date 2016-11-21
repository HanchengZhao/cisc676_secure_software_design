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
    size_t bytes = 16;
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
    
    free(chunks[5]);
    free(chunks[7]);
    free(chunks[3]);
    printf("chunk 3 at %p:\n", chunks[3]);
    show_chunk_hex(chunks[3]-header, bytes+header); 
    printf("chunk 5 at %p:\n", chunks[5]);    
    show_chunk_hex(chunks[5]-header, bytes+header);
    printf("chunk 7 at %p:\n", chunks[7]);    
    show_chunk_hex(chunks[7]-header, bytes+header);
}