
#pragma once

#include "crypt_t.h"

typedef struct {
	crypt_t h;
	unsigned char S[256];
	unsigned char i;
	unsigned char j;
} rc4_ctx_t;

