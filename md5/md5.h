
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
} md5_t;

md5_t * MD5_Init(md5_t * md);
void MD5_Update(md5_t * md, unsigned char const * d, size_t len);
void MD5_Final(md5_t * md, unsigned char * digest, size_t len);

