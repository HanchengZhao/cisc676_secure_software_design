#include <stdio.h>
#include <gcrypt.h>

int main (int argc, const char **argv) {
  printf("%s\n", gcry_check_version(NULL));
  return 0;
}