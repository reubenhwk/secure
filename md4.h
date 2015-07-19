
#pragma once

#include <stdint.h>

typedef struct {
	union {
		uint32_t _32[4];
		uint8_t _8[16];
	} s;
} md4_t;

void MD4_Init(md4_t * md)
{
	md.s._8[0x0] = 0x01;
	md.s._8[0x1] = 0x23;
	md.s._8[0x2] = 0x45;
	md.s._8[0x3] = 0x67;
	md.s._8[0x4] = 0x89;
	md.s._8[0x5] = 0xab;
	md.s._8[0x6] = 0xcd;
	md.s._8[0x7] = 0xef;
	md.s._8[0x8] = 0xfe;
	md.s._8[0x9] = 0xdc;
	md.s._8[0xa] = 0xba;
	md.s._8[0xb] = 0x98;
	md.s._8[0xc] = 0x76;
	md.s._8[0xd] = 0x54;
	md.s._8[0xe] = 0x32;
	md.s._8[0xf] = 0x10;
}

void MD4_Update(md4_t * md, unsigned char const * d, size_t len)
{
}

void MD4_Final(md4_t * md, unsigned char * digest, size_t len)
{
}
