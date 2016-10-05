#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdalign>

struct Banana {
    int ripeness;
    int age;
    float price;
};

int main(int argc, char **argv) {
    size_t bytes = sizeof(struct Banana);
    struct Banana* p2chunk = (struct Banana *) aligned_alloc(1024,bytes);
    printf("%p p2chunk\n", (void*)p2chunk);
    p2chunk[0].ripeness = 1;
    p2chunk[1].ripeness = 2;
    p2chunk[2].ripeness = 3;
    p2chunk[3].ripeness = 4;    
    printf("%d %d %d %d\n", p2chunk[0].ripeness, p2chunk[1].ripeness, p2chunk[2].ripeness, p2chunk[3].ripeness);
    printf("%ld\n", sizeof(p2chunk[0]));
    free(p2chunk);
}