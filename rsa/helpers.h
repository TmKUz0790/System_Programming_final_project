#ifndef HELPERS_H
#define HELPERS_H

int gcd(int a, int b);
int mod_inverse(int a, int m);
long long int encrypt(long long int message, int n, int public_key);
long long int decrypt(long long int encrypted_text, int n, int private_key);
long long int power(long long int x, unsigned int y, int p);
void setKeys();
void storeKeys(long long int public_key, long long int private_key);

#endif
