
#pragma once

#include <stdint.h>
#include <stdlib.h>

typedef struct {
	union {
		uint32_t _32[4];
		uint8_t _8[16];
	} s;
	union {
		uint32_t _32[16];
		uint8_t _8[64];
	} b;
	uint64_t count;
} md4_t;

md4_t * MD4_Init(md4_t * md);
void MD4_Update(md4_t * md, unsigned char const * d, size_t len);
void MD4_Final(md4_t * md, unsigned char * digest, size_t len);

