#ifndef B64_H
#define B64_H

#include <stddef.h>

void b64_encode(const unsigned char *src, size_t len, char *out);
void b64_decode(const char *src, unsigned char *out, size_t *out_len);

#endif
