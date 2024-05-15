#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "helpers.h"

void storeKeys(long long int public_key, long long int private key){
    FILE* publicKey;
    FILE* privateKey;

    publicKey = fopen("publicKey.pem", "w");
    privateKey = fopen("privateKey.pem", "w");

    if(publicKey == NULL || privateKey == NULL){
        printf("failed to save keys");
        return 1;
    }

    printf("this is public key: %llu\n", public_key);
    printf("this is private key: %llu\n", private_key);

    //writing and saving public key inside a txt file
    fprintf(publicKey, "%llu", public_key);

    //writing and saving private key inside a txt file
    fprintf(privateKey, "%llu", private_key);

    fclose(publicKey);
    fclose(privateKey);

    return 0;
}