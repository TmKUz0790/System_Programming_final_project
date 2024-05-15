#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "helpers.h"

#define MAX_SIZE 8192

unsigned long int prime[MAX_SIZE];
int prime_count = 0;
bool used[MAX_SIZE] = {false};     //Array to track used primes 

int public_key;
int private_key;
int n;

void primeFiller() {
    bool seive[MAX_SIZE];
    for (int i = 0; i < MAX_SIZE; i++) {
        seive[i] = true;
    }
    seive[0] = false;
    seive[1] = false;

    for(int p = 2; p*p < MAX_SIZE; p++){
        if(seive[p]){
            for(int j = p*p; j<MAX_SIZE; j+=p){
                seive[j] = false; 
            }
        }
    }

    for(int k = 2; k < MAX_SIZE; k++){
        if(seive[k]){
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

void setkeys() {
    int prime1 = pickRandomPrime();
    int prime2 = pickRandomPrime();
    n = prime1 * prime2;
    int fi = (prime1 - 1) * (prime2 - 1);
    int e = 2;
    while (gcd(e,fi) != 1) {
        e++;
    }

    public_key = e;
    
    int d = mod_inverse(e,fi);
    
    private_key = d;
}

// long long int power(long long int x, unsigned int y, int p) {
//     long long int res = 1;
//     x = x % p;
//     while (y > 0) {
//         if (y & 1) {
//             res = (res * x) % p;
//         }   
//         y = y >> 1;
//         x = (x * x) % p;
//     }
//     return res;
// }

void encoder(char *message, int *encoded) {
    int i = 0;
    while (message[i] != '\0') {
        encoded[i] = encrypt((int)message[i], n, public_key);
        i++;
    }
    encoded[i] = -1; // Marking end of encoded array
}

void decoder(int *encoded, char *decoded) {
    int i = 0;
    while (encoded[i] != -1) {
        int decrypted_value = decrypt(encoded[i], n, private_key);
        decoded[i] = (char)decrypted_value;
        i++;
    }
    decoded[i] = '\0'; // Marking end of decoded string
}

int main() {
    FILE* fptr;
    FILE* output_file;
    int counter = 0;
    char* buffer;
    char filename[1000];

    printf("please enter filename: ");
    scanf("%s",filename);

    fptr = fopen(filename, "r");
    output_file = fopen("encrypted.txt", "wb");

    if (fptr == NULL || output_file == NULL) {
        printf("Failed to open the file.\n");
        return 1;
    }

    while (fgetc(fptr) != EOF) {
        counter++;
    }

    // Rewind the file pointer to the beginning of the file
    fseek(fptr, 0, SEEK_SET);

    // Allocate memory for the buffer dynamically
    buffer = (char*)malloc(counter * sizeof(char));
    if (buffer == NULL) {
        printf("Memory allocation failed.\n");
        fclose(fptr);
        return 1;
    }

    // Read the content of the file into the buffer
    fread(buffer, sizeof(char), counter, fptr);

    // Add null terminator at the end of the buffer
    buffer[counter] = '\0';

    primeFiller();
    setkeys();
    int encoded[1000];
    encoder(buffer, encoded);

    printf("Initial message:\n%s\n\n", buffer);
    printf("The encoded message(encrypted by public key):\n");
    int i = 0;
    while (encoded[i] != -1) {
        fprintf(output_file,"%d", encoded[i]);
        i++;
    }

    char decoded[1000];
    decoder(encoded, decoded);
    printf("\n\nThe decoded message(decrypted by private key):\n%s\n", decoded);
    
    fclose(fptr);
    fclose(output_file);

    return 0;
}