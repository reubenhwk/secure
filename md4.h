
#pragma once

#include <stdint.h>

typedef struct {
	union {
		uint32_t _32[4];
		uint8_t _8[16];
	} s;
} md4_t;

void MD4_Init(md4_t * md);
void MD4_Update(md4_t * md, unsigned char const * d, size_t len);
void MD4_Final(md4_t * md, unsigned char * digest, size_t len);

