#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "helpers.h"
#include "b64.h"

#define MAX_SIZE 8192

int prime[MAX_SIZE];
int prime_count = 0;
bool used[MAX_SIZE] = {false};
long long int public_key;
long long int private_key;
int n;

void primeFiller() {
    bool seive[MAX_SIZE];
    for (int i = 0; i < MAX_SIZE; i++) {
        seive[i] = true;
    }
    seive[0] = false;
    seive[1] = false;

    for (int p = 2; p * p < MAX_SIZE; p++) {
        if (seive[p]) {
            for (int j = p * p; j < MAX_SIZE; j += p) {
                seive[j] = false;
            }
        }
    }

    for (int k = 2; k < MAX_SIZE; k++) {
        if (seive[k]) {
            prime[prime_count++] = k;
        }
    }
}

int pickRandomPrime() {
    int k;
    do {
        k = rand() % prime_count;
    } while (used[k]);

    used[k] = true;
    prime_count--;

    return prime[k];
}

void encoder(char *message, int *encoded) {
    int i = 0;
    while (message[i] != '\0') {
        encoded[i] = encrypt((int)message[i], n, public_key);
        i++;
    }
    encoded[i] = -1;
}

void decoder(int *encoded, char *decoded) {
    int i = 0;
    while (encoded[i] != -1) {
        int decrypted_value = decrypt(encoded[i], n, private_key);
        decoded[i] = (char)decrypted_value;
        i++;
    }
    decoded[i] = '\0';
}

void encodeKeys(long long int public_key, int n, char *encodedPublicKey, char *encodedN) {
    unsigned char publicKeyBytes[sizeof(public_key)];
    unsigned char nBytes[sizeof(n)];

    memcpy(publicKeyBytes, &public_key, sizeof(public_key));
    memcpy(nBytes, &n, sizeof(n));

    b64_encode(publicKeyBytes, sizeof(public_key), encodedPublicKey);
    b64_encode(nBytes, sizeof(n), encodedN);
}

void decodeKeys(const char *encodedPublicKey, const char *encodedN) {
    unsigned char publicKeyBytes[sizeof(public_key)];
    unsigned char nBytes[sizeof(n)];
    size_t out_len;

    // Decode public key
    b64_decode(encodedPublicKey, publicKeyBytes, &out_len);
    if (out_len != sizeof(public_key)) {
        fprintf(stderr, "Decoded public key length mismatch. Expected %zu, got %zu.\n", sizeof(public_key), out_len);
        exit(1);
    }
    memcpy(&public_key, publicKeyBytes, sizeof(public_key));

    // Decode n
    b64_decode(encodedN, nBytes, &out_len);
    if (out_len != sizeof(n)) {
        fprintf(stderr, "Decoded n length mismatch. Expected %zu, got %zu.\n", sizeof(n), out_len);
        exit(1);
    }
    memcpy(&n, nBytes, sizeof(n));
}



int main() {
    char options[][100] = {"Encryption", "Decryption", "Generate Keys", "Decode base64 public key and N"};
    int lenOptions = sizeof(options) / sizeof(options[0]);
    int getOption = 0;
    FILE* orig_file;
    char fileName[150];
    int counter = 0;
    char* buffer;
    int encoded[1000];
    char encodedPublicKey[256];
    char encodedN[256];

    for (int i = 0; i < lenOptions; i++) {
        printf("%d: %s\n", i + 1, options[i]);
    }

    for (int j = 0; j < lenOptions; j++) {
        printf("For choosing %s enter %d\n", options[j], j + 1);
    }
    scanf("%d", &getOption);

    primeFiller();

    if (getOption == 1) {
        printf("You have chosen encryption \n");

        printf("Please enter the public key \n");
        scanf("%lld", &public_key);

        printf("Please enter the file you want to encrypt \n");
        scanf("%s", fileName);

        orig_file = fopen(fileName, "r");

        if (orig_file == NULL) {
            printf("Failed to open file");
            exit(1);
        }

        while (fgetc(orig_file) != EOF) {
            counter++;
        }

        fseek(orig_file, 0, SEEK_SET);

        buffer = (char*)malloc((counter + 1) * sizeof(char));
        if (buffer == NULL) {
            printf("Memory allocation failed.\n");
            fclose(orig_file);
            return 1;
        }

        fread(buffer, sizeof(char), counter, orig_file);
        buffer[counter] = '\0';

        setKeys();

        encoder(buffer, encoded);
        for (int i = 0; encoded[i] != -1; i++) {
            printf("%d ", encoded[i]);
        }
        printf("\n");
        fclose(orig_file);
        free(buffer);
    }

    if (getOption == 2) {
        printf("You have chosen decryption \n");
        printf("Please enter the ciphered text (end with -1): \n");
        int i = 0;
        while (1) {
            scanf("%d", &encoded[i]);
            if (encoded[i] == -1 && i > 0) {
                break;
            }
            i++;
        }

        printf("Please enter the private key \n");
        scanf("%lld", &private_key);

        printf("Please enter the value of n (product of primes) \n");
        scanf("%d", &n);

        char decoded[10000];
        decoder(encoded, decoded);
        printf("Decoded message: %s\n", decoded);
    }

    if (getOption == 3) {
        printf("You have selected generating keys\n");
        setKeys();
        storeKeys(public_key, private_key);

        encodeKeys(public_key, n, encodedPublicKey, encodedN);
        
        printf("Encoded public key: %s\n", encodedPublicKey);
        printf("Encoded n: %s\n", encodedN);
    }

    if (getOption == 4) {
        printf("Enter the base64 encoded public key\n");
        scanf("%s", encodedPublicKey);

        printf("Enter the base64 encoded value of N\n");
        scanf("%s", encodedN);

        decodeKeys(encodedPublicKey, encodedN);

        printf("Decoded public key: %lld\n", public_key);
        printf("Decoded n: %d\n", n);
    }
    return 0;
}
