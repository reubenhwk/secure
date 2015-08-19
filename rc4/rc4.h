
#pragma once

#include "crypt_t.h"

#include <stdlib.h>

crypt_t * rc4_new_ctx(unsigned char const * key, size_t keylen, unsigned int flags);

