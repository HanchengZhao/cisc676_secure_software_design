#include <stdio.h>
#include <stdlib.h>
#include <gcrypt.h>


int main () {
    size_t index;
    size_t max_length = 40;
    size_t txtLength = 40;
    char thingy[] = "A8677DF7AAC4E90428D7226C5D28971273A5DDDB2F0338C963ACC521A6232934";
    char otherthingy[65] = "";
    
    
    char * hashBuffer = malloc(33);
    char * textBuffer = malloc(txtLength+1);
    memset(hashBuffer, 0, 33);
    
    char *word = malloc(max_length+1);
    FILE * fp;
    fp = fopen ("dictionary.txt", "r");
    int i=0;
    while(getline(&word, &max_length, fp) != -1 ){
        i++;
        // printf("%s %zd\n", word, strlen(word));
        txtLength = strlen(word) - 1;
        strncpy(textBuffer, word, txtLength);
        
        gcry_md_hash_buffer(,);   // size_t
        
        for (index = 0; index<32; index++){
          sprintf(otherthingy+index*2, "%02X", (unsigned char)hashBuffer[index]);
        }
        
        if(strcmp(otherthingy,thingy) == 0){
            printf("the word is: %s", word);
            return 0;
        }
       
    }
    printf("finished\n");
    fclose(fp);
}