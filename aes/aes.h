
#include "crypt_t.h"

#include <stdint.h>

crypt_t * aes128_new_ctx(
	unsigned char const * key,
	size_t keylen);

crypt_t * aes192_new_ctx(
	unsigned char const * key,
	size_t keylen);

crypt_t * aes256_new_ctx(
	unsigned char const * key,
	size_t keylen);

