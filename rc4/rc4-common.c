
#include "rc4-internal.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void rc4_crypt(rc4_ctx_t * rc4, void * _buffer, size_t len);

crypt_t * rc4_new_ctx(unsigned char const * key, size_t keylen, unsigned int flags)
{
	rc4_ctx_t * rc4 = malloc(sizeof(rc4_ctx_t));

	if (!rc4) {
		fprintf(stderr, "error: unable to allocate rc4_ctx_t object, exiting");
		exit(-1);
	}

	for (int i = 0; i < 256; ++i) {
		rc4->S[i] = i;
	}
	rc4->i = 0;
	rc4->j = 0;

	if (key) {
		int j = 0;
		for (int i = 0; i < sizeof(rc4->S); ++i) {
			j = (j + rc4->S[i] + key[i % keylen]) & 0xff;
			unsigned char tmp = rc4->S[i];
			rc4->S[i] = rc4->S[j];
			rc4->S[j] = tmp;
		}
	}

	rc4->h.encrypt = (encrypt_t)rc4_crypt;
	rc4->h.decrypt = (decrypt_t)rc4_crypt;
	rc4->h.free = free;

	return (crypt_t*)rc4;
}

void rc4_free_ctx(rc4_ctx_t * rc4)
{
	free(rc4);
}

