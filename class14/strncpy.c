#include "stdio.h"
#include "string.h"

int main(void) {
    // Disable stdout buffering
    char test[] = "testCodetesttestestest";
    char first[10]; 
    char second[20];
    strncpy(first,test,sizeof(test));
    strncpy(second,test,sizeof(test));
    strcat(second,test);
	
    puts(first);
    printf("/n");
    puts(second);
    return 0;
}