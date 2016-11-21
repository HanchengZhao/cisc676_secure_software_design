#include "stdio.h" 
int main(void) { // Disable stdout buffering
    setvbuf(stdout, NULL, _IONBF, 0);
    unsigned int randval;
    FILE *f;
    f = fopen("/dev/urandom", "r");
    fread(&randval, sizeof(randval), 1, f);
    fclose(f);
    printf("%u\n", randval);
    fflush(stdout); 
    usleep(1);
    return 0;
}