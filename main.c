#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/aes.h>

#define AES_BLOCK_SIZE 16

void encrypt_file(const char *input_filename, const char *output_filename, const unsigned char *key) {
    FILE *input_file = fopen(input_filename, "rb");
    FILE *output_file = fopen(output_filename, "wb");

    AES_KEY aes_key;
    AES_set_encrypt_key(key, 128, &aes_key);

    unsigned char input_buffer[AES_BLOCK_SIZE];
    unsigned char output_buffer[AES_BLOCK_SIZE];

    int bytes_read;
    while ((bytes_read = fread(input_buffer, 1, AES_BLOCK_SIZE, input_file)) > 0) {
        AES_encrypt(input_buffer, output_buffer, &aes_key);
        fwrite(output_buffer, 1, bytes_read, output_file);
    }

    fclose(input_file);
    fclose(output_file);
}

void decrypt_file(const char *input_filename, const char *output_filename, const unsigned char *key) {
    FILE *input_file = fopen(input_filename, "rb");
    FILE *output_file = fopen(output_filename, "wb");

    AES_KEY aes_key;
    AES_set_decrypt_key(key, 128, &aes_key);

    unsigned char input_buffer[AES_BLOCK_SIZE];
    unsigned char output_buffer[AES_BLOCK_SIZE];

    int bytes_read;
    while ((bytes_read = fread(input_buffer, 1, AES_BLOCK_SIZE, input_file)) > 0) {
        AES_decrypt(input_buffer, output_buffer, &aes_key);
        fwrite(output_buffer, 1, bytes_read, output_file);
    }

    fclose(input_file);
    fclose(output_file);
}

int main() {
    const char *input_filename = "input.txt";
    const char *encrypted_filename = "encrypted.txt";
    const char *decrypted_filename = "decrypted.txt";
    const unsigned char key[AES_BLOCK_SIZE] = "1234567890123456";

    // Encrypt file
    encrypt_file(input_filename, encrypted_filename, key);
    printf("File encrypted successfully.\n");

    // Decrypt file
    decrypt_file(encrypted_filename, decrypted_filename, key);
    printf("File decrypted successfully.\n");

    return 0;
}
