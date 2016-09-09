#include "stdio.h"

void show_bits(void* start, int n_bits){
    int* fetch_value = start;
    int k,i;
    for(i = 0 ; i <= n_bits - 1; i++){
        k = (*fetch_value) >> i;
       putchar((k & 1) ? '1' : '0');
    }
    printf("\n");
};

int main(void) {
    // Disable stdout buffering
    setvbuf(stdout, NULL, _IONBF, 0);
    
    int x=1025;
    show_bits( (void*) &x, 11);
    printf("should have shown\n10000000001\n\n");
    char y=10;
    show_bits( (void*) &y, 8);
    printf("should have shown\n01010000\n\n");
    char z='A';
    show_bits( (void*) &z, 8);
    printf("should have shown\n10000010\n");
    
    fflush(stdout);
    usleep(1);
    return 0;
} 