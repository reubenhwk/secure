
#include "md5.h"

#include <string.h>
#include <stdio.h>

void md5_round(md5_t * md, uint32_t const * b);

#ifndef MD5_ASM
static inline uint32_t rol(uint32_t x, int y)
{
	return (x << y) | (x >> (32-y));
}

void md5_round(md5_t * md, uint32_t const * b)
{

#define ff(x, y, z) ((x&y) | ((~x)&z))
#define gg(x, y, z) ((x&z) | (y&(~z)))
#define hh(x, y, z) (x ^ y ^ z)
#define ii(x, y, z) (y ^ (x | (~z)))

#define r1(a, b, c, d, k, s, i) (a = b + rol(a + ff(b, c, d) + k + i, s))
#define r2(a, b, c, d, k, s, i) (a = b + rol(a + gg(b, c, d) + k + i, s))
#define r3(a, b, c, d, k, s, i) (a = b + rol(a + hh(b, c, d) + k + i, s))
#define r4(a, b, c, d, k, s, i) (a = b + rol(a + ii(b, c, d) + k + i, s))

#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21

	uint32_t x[16];

#ifdef _BIG_ENDIAN
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

	r1(A, B, C, D, x[ 0], S11, 0xd76aa478); /* 1 */
	r1(D, A, B, C, x[ 1], S12, 0xe8c7b756); /* 2 */
	r1(C, D, A, B, x[ 2], S13, 0x242070db); /* 3 */
	r1(B, C, D, A, x[ 3], S14, 0xc1bdceee); /* 4 */
	r1(A, B, C, D, x[ 4], S11, 0xf57c0faf); /* 5 */
	r1(D, A, B, C, x[ 5], S12, 0x4787c62a); /* 6 */
	r1(C, D, A, B, x[ 6], S13, 0xa8304613); /* 7 */
	r1(B, C, D, A, x[ 7], S14, 0xfd469501); /* 8 */
	r1(A, B, C, D, x[ 8], S11, 0x698098d8); /* 9 */
	r1(D, A, B, C, x[ 9], S12, 0x8b44f7af); /* 10 */
	r1(C, D, A, B, x[10], S13, 0xffff5bb1); /* 11 */
	r1(B, C, D, A, x[11], S14, 0x895cd7be); /* 12 */
	r1(A, B, C, D, x[12], S11, 0x6b901122); /* 13 */
	r1(D, A, B, C, x[13], S12, 0xfd987193); /* 14 */
	r1(C, D, A, B, x[14], S13, 0xa679438e); /* 15 */
	r1(B, C, D, A, x[15], S14, 0x49b40821); /* 16 */

	r2(A, B, C, D, x[ 1], S21, 0xf61e2562); /* 17 */
	r2(D, A, B, C, x[ 6], S22, 0xc040b340); /* 18 */
	r2(C, D, A, B, x[11], S23, 0x265e5a51); /* 19 */
	r2(B, C, D, A, x[ 0], S24, 0xe9b6c7aa); /* 20 */
	r2(A, B, C, D, x[ 5], S21, 0xd62f105d); /* 21 */
	r2(D, A, B, C, x[10], S22, 0x02441453); /* 22 */
	r2(C, D, A, B, x[15], S23, 0xd8a1e681); /* 23 */
	r2(B, C, D, A, x[ 4], S24, 0xe7d3fbc8); /* 24 */
	r2(A, B, C, D, x[ 9], S21, 0x21e1cde6); /* 25 */
	r2(D, A, B, C, x[14], S22, 0xc33707d6); /* 26 */
	r2(C, D, A, B, x[ 3], S23, 0xf4d50d87); /* 27 */
	r2(B, C, D, A, x[ 8], S24, 0x455a14ed); /* 28 */
	r2(A, B, C, D, x[13], S21, 0xa9e3e905); /* 29 */
	r2(D, A, B, C, x[ 2], S22, 0xfcefa3f8); /* 30 */
	r2(C, D, A, B, x[ 7], S23, 0x676f02d9); /* 31 */
	r2(B, C, D, A, x[12], S24, 0x8d2a4c8a); /* 32 */

	r3(A, B, C, D, x[ 5], S31, 0xfffa3942); /* 33 */
	r3(D, A, B, C, x[ 8], S32, 0x8771f681); /* 34 */
	r3(C, D, A, B, x[11], S33, 0x6d9d6122); /* 35 */
	r3(B, C, D, A, x[14], S34, 0xfde5380c); /* 36 */
	r3(A, B, C, D, x[ 1], S31, 0xa4beea44); /* 37 */
	r3(D, A, B, C, x[ 4], S32, 0x4bdecfa9); /* 38 */
	r3(C, D, A, B, x[ 7], S33, 0xf6bb4b60); /* 39 */
	r3(B, C, D, A, x[10], S34, 0xbebfbc70); /* 40 */
	r3(A, B, C, D, x[13], S31, 0x289b7ec6); /* 41 */
	r3(D, A, B, C, x[ 0], S32, 0xeaa127fa); /* 42 */
	r3(C, D, A, B, x[ 3], S33, 0xd4ef3085); /* 43 */
	r3(B, C, D, A, x[ 6], S34, 0x04881d05); /* 44 */
	r3(A, B, C, D, x[ 9], S31, 0xd9d4d039); /* 45 */
	r3(D, A, B, C, x[12], S32, 0xe6db99e5); /* 46 */
	r3(C, D, A, B, x[15], S33, 0x1fa27cf8); /* 47 */
	r3(B, C, D, A, x[ 2], S34, 0xc4ac5665); /* 48 */

	r4(A, B, C, D, x[ 0], S41, 0xf4292244); /* 49 */
	r4(D, A, B, C, x[ 7], S42, 0x432aff97); /* 50 */
	r4(C, D, A, B, x[14], S43, 0xab9423a7); /* 51 */
	r4(B, C, D, A, x[ 5], S44, 0xfc93a039); /* 52 */
	r4(A, B, C, D, x[12], S41, 0x655b59c3); /* 53 */
	r4(D, A, B, C, x[ 3], S42, 0x8f0ccc92); /* 54 */
	r4(C, D, A, B, x[10], S43, 0xffeff47d); /* 55 */
	r4(B, C, D, A, x[ 1], S44, 0x85845dd1); /* 56 */
	r4(A, B, C, D, x[ 8], S41, 0x6fa87e4f); /* 57 */
	r4(D, A, B, C, x[15], S42, 0xfe2ce6e0); /* 58 */
	r4(C, D, A, B, x[ 6], S43, 0xa3014314); /* 59 */
	r4(B, C, D, A, x[13], S44, 0x4e0811a1); /* 60 */
	r4(A, B, C, D, x[ 4], S41, 0xf7537e82); /* 61 */
	r4(D, A, B, C, x[11], S42, 0xbd3af235); /* 62 */
	r4(C, D, A, B, x[ 2], S43, 0x2ad7d2bb); /* 63 */
	r4(B, C, D, A, x[ 9], S44, 0xeb86d391); /* 64 */

	md->s._32[0] += A;
	md->s._32[1] += B;
	md->s._32[2] += C;
	md->s._32[3] += D;
}
#endif

void MD5_Update(md5_t * md, unsigned char const * d, size_t len)
{
	MD_Update(md, d, len, md5_round);
}

void MD5_Final(md5_t * md, unsigned char * digest, size_t len)
{
	MD_Final(md, digest, len, md5_round);
}

#ifdef DEFINE_MD5_MAIN
int main()
{
	md5_t ctx;
	MD5_Init(&ctx);

	char buffer[4*1024];
	int rc;
	while (rc = fread(buffer, 1, sizeof(buffer), stdin), rc > 0) {
		MD5_Update(&ctx, (unsigned char*)buffer, rc);
	}

	unsigned char dgst[MD5_LENGTH];
	MD5_Final(&ctx, dgst, MD5_LENGTH);

	for (int i = 0; i < MD5_LENGTH; ++i) {
		printf("%02x", dgst[i]);
	}
	printf("\n");

	return 0;
}
#endif
