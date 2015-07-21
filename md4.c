
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

static uint32_t rol(uint32_t x, int y)
{
	return (x << y) | (x >> (32-y));
}

static uint32_t f(uint32_t x, uint32_t y, uint32_t z)
{
	/* XY v not(X)Z */
	return x&y | (~x)&z;
}

static uint32_t g(uint32_t x, uint32_t y, uint32_t z)
{
	/* XY v XZ v YZ */
	return x&y | x&z | y&z;
}

static uint32_t h(uint32_t x, uint32_t y, uint32_t z)
{
	/* X xor Y xor Z */
	return x ^ y ^ z;
}

static void round1(md4_t * md)
{
#define r1(A, B, C, D, i, s) (A = rol(A + f(B, C, D) + md->block[i], s))

	uint32_t A = md->s._32[0];
	uint32_t B = md->s._32[1];
	uint32_t C = md->s._32[2];
	uint32_t D = md->s._32[3];

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

	md->s._32[0] += A;
	md->s._32[1] += B;
	md->s._32[2] += C;
	md->s._32[3] += D;
}

static void round2(md4_t * md)
{
#define r2(A, B, C, D, i, s) (A = rol(A + g(B, C, D) + md->block[i] + 0x5a827999, s))

	uint32_t A = md->s._32[0];
	uint32_t B = md->s._32[1];
	uint32_t C = md->s._32[2];
	uint32_t D = md->s._32[3];

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

	md->s._32[0] += A;
	md->s._32[1] += B;
	md->s._32[2] += C;
	md->s._32[3] += D;
}

static void round3(md4_t * md)
{
#define r3(A, B, C, D, i, s) (A = rol(A + h(B, C, D) + md->block[i] + 0x6ED9EBA1, s))

	uint32_t A = md->s._32[0];
	uint32_t B = md->s._32[1];
	uint32_t C = md->s._32[2];
	uint32_t D = md->s._32[3];

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

	md->s._32[0] += A;
	md->s._32[1] += B;
	md->s._32[2] += C;
	md->s._32[3] += D;
}

void MD4_Update(md4_t * md, unsigned char const * d, size_t len)
{
	for (int i = 0; i < len; ++i) {
		md->block[(md->count++) & 0x3f] = d[i];
		if (0 == (md->count & 0x3f)) {
			round1(md);
			round2(md);
			round3(md);
		}
	}
}

static int compute_pad_len(int count)
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

	MD4_Update(md, pad, compute_pad_len(md->count));
	uint64_t bits = 8 * md->count;
	MD4_Update(md, (unsigned char*)&bits, sizeof(bits));

	if (digest) {
		memcpy(digest, (unsigned char *)md->s._8, len);
	}

	for (int i = 0; i < 16; ++i) {
		printf("%02x", md->s._8[i]);
	}
	printf("\n");
}

