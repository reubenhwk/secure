
#pragma once

#include <stdlib.h>

typedef struct {
	unsigned char S[256];
	unsigned char i;
	unsigned char j;
} rc4_ctx_t;

#define rc4_encrypt rc4_crypt
#define rc4_decrypt rc4_crypt

void rc4_crypt(rc4_ctx_t * rc4, void * buffer, size_t len);
void rc4_free_ctx(rc4_ctx_t * rc4);
rc4_ctx_t * rc4_new_ctx(unsigned char const * key, size_t keylen, unsigned int flags);

