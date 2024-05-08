#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

int isPrime(int n){
    int i;
    for (i = 2; i <= sqrt(n); i++){
        if (n % i == 0) return 0;
    }
    return 1;
}

int gcd(int a, int b){
    if (a == 0) return b;
    return gcd(b % a, a);
}

int totient(int p, int q){
    return (p - 1) * (q - 1);
} 

int randome(int lambda_n){
    printf("\nThe number e should be less than %d\n and greater than 1.",lambda_n);
    for (int i = 2; i < lambda_n; i++){
        if (gcd(i, lambda_n) == 1) return i;
    }
    return -1;}

int private_key(int enc, int lambda_n){
    for (int i = 1; i < lambda_n; i++){
        if ((i * enc) % lambda_n == 1){
            printf("\nThus, (i * enc) %% lambda_n = 1, (%d * %d) %% %d = 1", i, enc,lambda_n);
            return i;}
    }

    return -1;
}


long pomod(long a, long b, long m){
    long x = 1, y = a;
    while (b > 0){
        if (b % 2 == 1){
            x = (x * y) % m;
        }
        y = (y * y) % m;
        b /= 2;
    }

    return x % m;
}
/* Encryption

 * A function which takes the message, the public key and a number n which is the

 * product of p and q. The function encrypts the message using the public key

 * and returns the encrypted message.

 */

char *encrypt(char *message, long enc, long n){
    long len = strlen(message);     // taking length of message
    char *cipher = (char *)malloc((len+1) * sizeof(char));

    for (long i = 0; i < len; i++)    {
        cipher[i] = pomod(message[i], enc, n);
        printf("\n%c -> %c", message[i], cipher[i]);
    }

    cipher[len] = '\0';
    return cipher;
}

/* Decryption

 * A function which takes the cipher text, the private key and a number n which

 * is he product of p and q. The function decrypts the cipher text using the

 * private key and returns the decrypted message.

 */

char *decrypt(char *cipher, long d, long n){
    long i;
    long len = strlen(cipher);
    char *message = (char *)malloc((len+1) * sizeof(char));
    for (i = 0; i < len; i++){
        // message[i] = (long) pow(cipher[i], d) % n;
        message[i] = pomod(cipher[i], d, n);
        printf("\n%c -> %c", cipher[i], message[i]);
    }

    message[len] = '\0';
    return message;
}

int main(){
    int p, q, lambda_n;

    long n, enc, d;

    char *message;

    char *cipher;

    printf("\nEnter the value of p: ");

    scanf("%d", &p);

    printf("\nEnter the value of q: ");

    scanf("%d", &q);

    if (isPrime(p) && isPrime(q))    {
        n = p * q;
        lambda_n = totient(p, q);
        enc = randome(lambda_n);

        d = private_key(enc, lambda_n);

        printf("\nThe value of n is %ld", n);

        printf("\nThe value of lambda_n is %d", lambda_n);

        printf("\nThe value of e is %ld", enc);

        printf("\nThe value of d is %ld", d);

        printf("\nEnter the message: ");

        message = (char *)malloc(sizeof(char) * 100);

        scanf("%s", message);

        cipher = encrypt(message, enc, n);

        puts("%s\nThe encrypted message is: ");

        printf("%s", cipher);

        message = decrypt(cipher, d, n);

        puts("%s\nThe original message was: ");

        printf("%s", message);}

    else{
        printf("\nThe value of p and q should be prime.");
    }

    return 0;
}
