#include <stdio.h>
#include <gcrypt.h>

int main () {
    gcry_error_t     gcryError;
    gcry_cipher_hd_t gcryCipherHd;
    size_t           index;
    char * salsaKey = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"; // 32 bytes
    char * iniVector = "AAAAAAAA"; // 8 bytes

    gcryError = gcry_cipher_open(
        &gcryCipherHd, // gcry_cipher_hd_t *
        GCRY_CIPHER_SALSA20,   // int
        GCRY_CIPHER_MODE_STREAM,   // int
        0);            // unsigned int
    if (gcryError)
    {
        printf("gcry_cipher_open failed:  %s/%s\n",
               gcry_strsource(gcryError),
               gcry_strerror(gcryError));
        return;
    }
    printf("gcry_cipher_open worked\n");
    
    gcryError = gcry_cipher_setkey(gcryCipherHd, salsaKey, 32);
    if (gcryError)
    {
        printf("gcry_cipher_setkey failed:  %s/%s\n",
               gcry_strsource(gcryError),
               gcry_strerror(gcryError));
        return;
    }
    printf("gcry_cipher_setkey worked\n");
    
    gcryError = gcry_cipher_setiv(gcryCipherHd, iniVector, 8);
    if (gcryError)
    {
        printf("gcry_cipher_setiv failed:  %s/%s\n",
               gcry_strsource(gcryError),
               gcry_strerror(gcryError));
        return;
    }
    printf("gcry_cipher_setiv worked\n");
    
    size_t txtLength = 101;
    char * encBuffer = malloc(txtLength);
    char * textBuffer = malloc(txtLength);
    memset(textBuffer, 0, 101);

    gcryError = gcry_cipher_encrypt(
        gcryCipherHd, // gcry_cipher_hd_t
        encBuffer,    // void *
        txtLength,    // size_t
        textBuffer,    // const void *
        txtLength);   // size_t
    if (gcryError)
    {
        printf("gcry_cipher_decrypt failed:  %s/%s\n",
               gcry_strsource(gcryError),
               gcry_strerror(gcryError));
        return;
    }
    printf("gcry_cipher_decrypt worked\n");
    
    printf("encBuffer = ");
    for (index = 0; index<txtLength-1; index++)
        printf("%02X", (unsigned char)encBuffer[index]);
    printf("\n");
    return 0;
}