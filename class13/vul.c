#include <string.h>

void go(char *data) {
    char name[64];

    strcpy(name, data);
}

int main(int argc, char **argv) {
    go(argv[1]);
}