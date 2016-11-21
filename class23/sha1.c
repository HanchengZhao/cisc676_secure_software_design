#include <stdio.h>
#include <gcrypt.h>

int main () {
    size_t index;
    size_t txtLength = 4;
    char * hashBuffer = malloc(33);
    char * textBuffer = malloc(txtLength+1);
    memset(hashBuffer, 0, 33);

    strncpy(textBuffer, "andy", txtLength);

    gcry_md_hash_buffer(
        GCRY_MD_SHA256, // gcry_cipher_hd_t
        hashBuffer,    // void *
        textBuffer,    // const void *
        txtLength);   // size_t

    printf("hashBuffer = ");
    for (index = 0; index<32; index++)
        printf("%02X", (unsigned char)hashBuffer[index]);
    printf("\n");
    return 0;
}