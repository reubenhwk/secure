
#include "md4.h"

#include <string.h>
#include <stdio.h>

md4_t * MD4_Init(md4_t * md)
{
	if (!md) {
		md = malloc(sizeof(md4_t));
	}

	md->s._8[0x0] = 0x01;
	md->s._8[0x1] = 0x23;
	md->s._8[0x2] = 0x45;
	md->s._8[0x3] = 0x67;
	md->s._8[0x4] = 0x89;
	md->s._8[0x5] = 0xab;
	md->s._8[0x6] = 0xcd;
	md->s._8[0x7] = 0xef;
	md->s._8[0x8] = 0xfe;
	md->s._8[0x9] = 0xdc;
	md->s._8[0xa] = 0xba;
	md->s._8[0xb] = 0x98;
	md->s._8[0xc] = 0x76;
	md->s._8[0xd] = 0x54;
	md->s._8[0xe] = 0x32;
	md->s._8[0xf] = 0x10;

	md->count = 0;

	return md;
}

static inline uint32_t rol(uint32_t x, int y)
{
	return (x << y) | (x >> (32-y));
}

static inline void rounds(md4_t * md, uint32_t const * restrict b, size_t count)
{

#define ff(x, y, z) (x&y | (~x)&z)
#define gg(x, y, z) (x&y | x&z | y&z)
#define hh(x, y, z) (x ^ y ^ z)

#define r1(A, B, C, D, i, s) (A = rol(A + ff(B, C, D) + b[i], s))
#define r2(A, B, C, D, i, s) (A = rol(A + gg(B, C, D) + b[i] + 0x5A827999, s))
#define r3(A, B, C, D, i, s) (A = rol(A + hh(B, C, D) + b[i] + 0x6ED9EBA1, s))

	uint32_t A = md->s._32[0];
	uint32_t B = md->s._32[1];
	uint32_t C = md->s._32[2];
	uint32_t D = md->s._32[3];

	while (count--) {
		r1(A, B, C, D,  0,  3);
		r1(D, A, B, C,  1,  7);
		r1(C, D, A, B,  2, 11);
		r1(B, C, D, A,  3, 19);
		r1(A, B, C, D,  4,  3);
		r1(D, A, B, C,  5,  7);
		r1(C, D, A, B,  6, 11);
		r1(B, C, D, A,  7, 19);
		r1(A, B, C, D,  8,  3);
		r1(D, A, B, C,  9,  7);
		r1(C, D, A, B, 10, 11);
		r1(B, C, D, A, 11, 19);
		r1(A, B, C, D, 12,  3);
		r1(D, A, B, C, 13,  7);
		r1(C, D, A, B, 14, 11);
		r1(B, C, D, A, 15, 19);

		r2(A, B, C, D,  0,  3);
		r2(D, A, B, C,  4,  5);
		r2(C, D, A, B,  8,  9);
		r2(B, C, D, A, 12, 13);
		r2(A, B, C, D,  1,  3);
		r2(D, A, B, C,  5,  5);
		r2(C, D, A, B,  9,  9);
		r2(B, C, D, A, 13, 13);
		r2(A, B, C, D,  2,  3);
		r2(D, A, B, C,  6,  5);
		r2(C, D, A, B, 10,  9);
		r2(B, C, D, A, 14, 13);
		r2(A, B, C, D,  3,  3);
		r2(D, A, B, C,  7,  5);
		r2(C, D, A, B, 11,  9);
		r2(B, C, D, A, 15, 13);

		r3(A, B, C, D,  0,  3);
		r3(D, A, B, C,  8,  9);
		r3(C, D, A, B,  4, 11);
		r3(B, C, D, A, 12, 15);
		r3(A, B, C, D,  2,  3);
		r3(D, A, B, C, 10,  9);
		r3(C, D, A, B,  6, 11);
		r3(B, C, D, A, 14, 15);
		r3(A, B, C, D,  1,  3);
		r3(D, A, B, C,  9,  9);
		r3(C, D, A, B,  5, 11);
		r3(B, C, D, A, 13, 15);
		r3(A, B, C, D,  3,  3);
		r3(D, A, B, C, 11,  9);
		r3(C, D, A, B,  7, 11);
		r3(B, C, D, A, 15, 15);

		A = md->s._32[0] += A;
		B = md->s._32[1] += B;
		C = md->s._32[2] += C;
		D = md->s._32[3] += D;
	}
}

void MD4_Update(md4_t * md, unsigned char const * d, size_t len)
{
	size_t used = (md->count & 0x3f);
	size_t left = 0x40 - used;

	if (left < len) {
		memcpy(&md->b._8[used], d, left);
		rounds(md, md->b._32, 1);
		d += left;
		md->count += left;
		len -= left;
	}

	if (len >= 64) {
		rounds(md, (uint32_t *)d, (len / 0x40));
		d += 0x40 * (len / 0x40);
		len -= 0x40 * (len / 0x40);
		md->count += 0x40 * (len / 0x40);
	}

	memcpy(&md->b._8[0], d, len);
}

static inline int compute_pad_len(int count)
{
	return 64 - ((count + 8) & 0x3f);
}

void MD4_Final(md4_t * md, unsigned char * digest, size_t len)
{
	static uint8_t const pad[64] = {
		0x80, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
	};

	uint64_t bits = 8 * md->count;
	MD4_Update(md, pad, compute_pad_len(md->count));
	MD4_Update(md, (unsigned char*)&bits, sizeof(bits));

	if (digest) {
		memcpy(digest, (unsigned char *)md->s._8, len);
	}
}

