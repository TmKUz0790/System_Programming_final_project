#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "helpers.h"

extern int n;
extern long long int public_key;
extern long long int private_key;
extern int pickRandomPrime();

int gcd(int a, int b){
    if(a == 0) return b;
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int mod_inverse(int a, int m) {
    int original_mod = m;
    int x0 = 0, x1 = 1;

    if (m == 1) {
        return 0;
    }

    while (a > 1) {
        // q is quotient
        int q = a / m;
        int temp = m;

        // m is remainder now, process same as Euclid's algo
        m = a % m;
        a = temp;
        
        temp = x0;
        x0 = x1 - q * x0;
        x1 = temp;
    }

    // Make x1 positive
    if (x1 < 0) {
        x1 += original_mod;
    }

    return x1;
}


long long int power(long long int x, unsigned int y, int p) {
    long long int res = 1;
    x = x % p;
    while (y > 0) {
        if (y & 1) {
            res = (res * x) % p;
        }
        y = y >> 1;
        x = (x * x) % p;
    }
    return res;
}

long long int encrypt(long long int message, int n, int public_key){
    return power(message, public_key, n);
}

long long int decrypt(long long int encrypted_text, int n, int private_key) {
    return power(encrypted_text, private_key, n);
}

void setKeys() {
    int prime1 = pickRandomPrime();
    int prime2 = pickRandomPrime();

    printf("prime1: %d\n", prime1);
    printf("prime2: %d\n", prime2);

    n = prime1 * prime2;
    printf("n: %d\n", n);

    int fi = (prime1 - 1) * (prime2 - 1);
    printf("fi: %d\n", fi);

    int e;
    do {
        // Generate a random number in the range [3, fi-1] and make sure it's odd
        e = (rand() % (fi - 3)) + 3;
        if (e % 2 == 0) e++; // Ensure e is odd
    } while (gcd(e, fi) != 1);

    printf("public_key (e): %d\n", e);


    printf("public_key (e): %d\n", e);

    public_key = e;

    int d = mod_inverse(e, fi);

    printf("private_key (d): %d\n", d);

    private_key = d;
}

void storeKeys(long long int public_key, long long int private_key){
    FILE* publicKey;
    FILE* privateKey;

    publicKey = fopen("publicKey.pem", "w");
    privateKey = fopen("privateKey.pem", "w");

    if(publicKey == NULL || privateKey == NULL){
        printf("failed to save keys");
        fclose(publicKey);
        fclose(privateKey);
    }

    printf("this is public key: %llu\n", public_key);
    printf("this is private key: %llu\n", private_key);

    //writing and saving public key inside a txt file
    fprintf(publicKey, "%llu", public_key);

    //writing and saving private key inside a txt file
    fprintf(privateKey, "%llu", private_key);

    fclose(publicKey);
    fclose(privateKey);
}