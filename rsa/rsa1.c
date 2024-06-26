#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define MAX_SIZE 250

int prime[MAX_SIZE];
int prime_count = 0;

int public_key;
int private_key;
int n;

int gcd(int a, int b){
    if (a == 0) return b;
    return gcd(b % a, a);
}

void primefiller() {
    bool seive[MAX_SIZE];
    for (int i = 0; i < MAX_SIZE; i++) {
        seive[i] = true;
    }
    seive[0] = false;
    seive[1] = false;

    for (int i = 2; i < MAX_SIZE; i++) {
        if (seive[i]) {
            prime[prime_count++] = i;
            for (int j = i * 2; j < MAX_SIZE; j += i) {
                seive[j] = false;
            }
        }
    }
}

int pickrandomprime() {
    int k = rand() % prime_count;
    int ret = prime[k];
    for (int i = k; i < prime_count - 1; i++) {
        prime[i] = prime[i + 1];
    }
    prime_count--;
    return ret;
}

void setkeys() {
    int prime1 = pickrandomprime();
    int prime2 = pickrandomprime();
    n = prime1 * prime2;
    int fi = (prime1 - 1) * (prime2 - 1);
    int e = 2;
    while (1) {
        if (gcd(e, fi) == 1)
            break;
        e++;
    }
    public_key = e;
    int d = 2;
    while (1) {
        if ((d * e) % fi == 1)
            break;
        d++;
    }
    private_key = d;
}

long long int power(long long int x, unsigned int y, int p) {
    long long int res = 1;
    x = x % p;
    while (y > 0) {
        if (y & 1)
            res = (res * x) % p;
        y = y >> 1;
        x = (x * x) % p;
    }
    return res;
}

long long int encrypt(double message) {
    int e = public_key;
    long long int encrypted_text = 1;
    while (e--) {
        encrypted_text *= message;
        encrypted_text %= n;
    }
    return encrypted_text;
}

long long int decrypt(int encrypted_text) {
    int d = private_key;
    long long int decrypted = 1;
    while (d--) {
        decrypted *= encrypted_text;
        decrypted %= n;
    }
    return decrypted;
}

void encoder(char *message, int *encoded) {
    int i = 0;
    while (message[i] != '\0') {
        encoded[i] = encrypt((int)message[i]);
        i++;
    }
    encoded[i] = -1; // Marking end of encoded array
}

void decoder(int *encoded, char *decoded) {
    int i = 0;
    while (encoded[i] != -1) {
        decoded[i] = (char)decrypt(encoded[i]);
        i++;
    }
    decoded[i] = '\0'; // Marking end of decoded string
}

int main() {
    FILE* fptr;
    int counter = 0;
    char* buffer;

    fptr = fopen("text.txt", "r");

    if (fptr == NULL) {
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

    primefiller();
    setkeys();
    int encoded[100];
    encoder(buffer, encoded);

    printf("Initial message:\n%s\n\n", buffer);
    printf("The encoded message(encrypted by public key):\n");
    int i = 0;
    while (encoded[i] != -1) {
        printf("%d", encoded[i]);
        i++;
    }

    char decoded[100];
    decoder(encoded, decoded);
    printf("\n\nThe decoded message(decrypted by private key):\n%s\n", decoded);

    return 0;
}
