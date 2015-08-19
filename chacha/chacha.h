
#include "crypt_t.h"

#include <stdint.h>

crypt_t * chacha_new_ctx(
	unsigned char const * key,
	size_t keylen,
	unsigned char const * nonce,
	size_t noncelen,
	uint32_t counter,
	int rounds,
	int flags);

