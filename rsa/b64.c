#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

static const char b64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

void b64_encode(const unsigned char *src, size_t len, char *out) {
    size_t i, j;
    for (i = 0, j = 0; i < len;) {
        uint32_t octet_a = i < len ? src[i++] : 0;
        uint32_t octet_b = i < len ? src[i++] : 0;
        uint32_t octet_c = i < len ? src[i++] : 0;
        uint32_t triple = (octet_a << 16) + (octet_b << 8) + octet_c;

        out[j++] = b64_table[(triple >> 18) & 0x3F];
        out[j++] = b64_table[(triple >> 12) & 0x3F];
        out[j++] = b64_table[(triple >> 6) & 0x3F];
        out[j++] = b64_table[triple & 0x3F];
    }

    size_t mod_len = len % 3;
    if (mod_len > 0) {
        for (; mod_len < 3; mod_len++) {
            out[j++] = '=';
        }
    }
    out[j] = '\0';
}




void b64_decode(const char *src, unsigned char *out, size_t *out_len) {
    size_t len = strlen(src);
    size_t padding = 0;

    if (len >= 2) {
        if (src[len - 1] == '=') padding++;
        if (src[len - 2] == '=') padding++;
    }

    *out_len = (len / 4 * 3) - padding;

    for (size_t i = 0, j = 0; i < len;) {
        uint32_t sextet_a = src[i] == '=' ? 0 & i++ : strchr(b64_table, src[i++]) - b64_table;
        uint32_t sextet_b = src[i] == '=' ? 0 & i++ : strchr(b64_table, src[i++]) - b64_table;
        uint32_t sextet_c = src[i] == '=' ? 0 & i++ : strchr(b64_table, src[i++]) - b64_table;
        uint32_t sextet_d = src[i] == '=' ? 0 & i++ : strchr(b64_table, src[i++]) - b64_table;
        uint32_t triple = (sextet_a << 18) + (sextet_b << 12) + (sextet_c << 6) + sextet_d;

        if (j < *out_len) out[j++] = (triple >> 16) & 0xFF;
        if (j < *out_len) out[j++] = (triple >> 8) & 0xFF;
        if (j < *out_len) out[j++] = triple & 0xFF;
    }
}



