#include "encrypt.h"
#include "checksum.h"
#include <stdio.h>

int main(){

    char s[] = "Speak frend and enter";
    encrypt(s);
    printf("Encrypted to '%s'\n", s);
    printf("Checksum is %i\n", checksum(s));
    encrypt(s);
    printf("Decrypted back to '%s'\n", s);
    printf("Checksum is %i\n", checksum(s));
    return 0;
}

