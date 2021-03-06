
#include "md4.h"

#include <string.h>
#include <stdio.h>

void md4_round(md4_t * md, uint32_t const * b);

#ifndef MD4_ASM
static inline uint32_t rol(uint32_t x, int y)
{
	return (x << y) | (x >> (32-y));
}

void md4_round(md4_t * md, uint32_t const * b)
{

#define ff(x, y, z) (x&y | (~x)&z)
#define gg(x, y, z) (x&y | x&z | y&z)
#define hh(x, y, z) (x ^ y ^ z)

#define r1(A, B, C, D, i, s) (A = rol(A + ff(B, C, D) + i, s))
#define r2(A, B, C, D, i, s) (A = rol(A + gg(B, C, D) + i + 0x5A827999, s))
#define r3(A, B, C, D, i, s) (A = rol(A + hh(B, C, D) + i + 0x6ED9EBA1, s))

#ifdef _BIG_ENDIAN
	uint32_t x[16];

	for (int i = 0; i < 16; ++i) {
		x[i] = __builtin_bswap32(b[i]);
	}
#else
#define x b
#endif
	uint32_t A = md->s._32[0];
	uint32_t B = md->s._32[1];
	uint32_t C = md->s._32[2];
	uint32_t D = md->s._32[3];

	r1(A, B, C, D, x[ 0],  3);
	r1(D, A, B, C, x[ 1],  7);
	r1(C, D, A, B, x[ 2], 11);
	r1(B, C, D, A, x[ 3], 19);
	r1(A, B, C, D, x[ 4],  3);
	r1(D, A, B, C, x[ 5],  7);
	r1(C, D, A, B, x[ 6], 11);
	r1(B, C, D, A, x[ 7], 19);
	r1(A, B, C, D, x[ 8],  3);
	r1(D, A, B, C, x[ 9],  7);
	r1(C, D, A, B, x[10], 11);
	r1(B, C, D, A, x[11], 19);
	r1(A, B, C, D, x[12],  3);
	r1(D, A, B, C, x[13],  7);
	r1(C, D, A, B, x[14], 11);
	r1(B, C, D, A, x[15], 19);

	r2(A, B, C, D, x[ 0],  3);
	r2(D, A, B, C, x[ 4],  5);
	r2(C, D, A, B, x[ 8],  9);
	r2(B, C, D, A, x[12], 13);
	r2(A, B, C, D, x[ 1],  3);
	r2(D, A, B, C, x[ 5],  5);
	r2(C, D, A, B, x[ 9],  9);
	r2(B, C, D, A, x[13], 13);
	r2(A, B, C, D, x[ 2],  3);
	r2(D, A, B, C, x[ 6],  5);
	r2(C, D, A, B, x[10],  9);
	r2(B, C, D, A, x[14], 13);
	r2(A, B, C, D, x[ 3],  3);
	r2(D, A, B, C, x[ 7],  5);
	r2(C, D, A, B, x[11],  9);
	r2(B, C, D, A, x[15], 13);

	r3(A, B, C, D, x[ 0],  3);
	r3(D, A, B, C, x[ 8],  9);
	r3(C, D, A, B, x[ 4], 11);
	r3(B, C, D, A, x[12], 15);
	r3(A, B, C, D, x[ 2],  3);
	r3(D, A, B, C, x[10],  9);
	r3(C, D, A, B, x[ 6], 11);
	r3(B, C, D, A, x[14], 15);
	r3(A, B, C, D, x[ 1],  3);
	r3(D, A, B, C, x[ 9],  9);
	r3(C, D, A, B, x[ 5], 11);
	r3(B, C, D, A, x[13], 15);
	r3(A, B, C, D, x[ 3],  3);
	r3(D, A, B, C, x[11],  9);
	r3(C, D, A, B, x[ 7], 11);
	r3(B, C, D, A, x[15], 15);

	md->s._32[0] += A;
	md->s._32[1] += B;
	md->s._32[2] += C;
	md->s._32[3] += D;
}
#endif

void MD4_Update(md4_t * md, unsigned char const * d, size_t len)
{
	MD_Update(md, d, len, md4_round);
}

void MD4_Final(md4_t * md, unsigned char * digest, size_t len)
{
	MD_Final(md, digest, len, md4_round);
}

#ifdef DEFINE_MD4_MAIN
int main()
{
	md4_t ctx;
	MD4_Init(&ctx);

	char buffer[4*1024];
	int rc;
	while (rc = fread(buffer, 1, sizeof(buffer), stdin), rc > 0) {
		MD4_Update(&ctx, (unsigned char*)buffer, rc);
	}

	unsigned char dgst[MD4_LENGTH];
	MD4_Final(&ctx, dgst, MD4_LENGTH);

	for (int i = 0; i < MD4_LENGTH; ++i) {
		printf("%02x", dgst[i]);
	}
	printf("\n");

	return 0;
}
#endif
