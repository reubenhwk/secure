
#include "md4.h"

#include <string.h>

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

static uint32_t f(uint32_t x, uint32_t y, uint32_t z)
{
	/* XY v not(X)Z */
	return x*y | (~x)*z;
}

static uint32_t g(uint32_t x, uint32_t y, uint32_t z)
{
	/* XY v XZ v YZ */
	return x*y | x*z | y*z;
}

static uint32_t h(uint32_t x, uint32_t y, uint32_t z)
{
	/* X xor Y xor Z */
	return x ^ y ^ z;
}

static void roun1(md4_t * md)
{
}

void MD4_Update(md4_t * md, unsigned char const * d, size_t len)
{
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

	int i;
	for (i = 0; i < 129; ++i) {
		int pad_len = compute_pad_len(i);
		printf("%3d %2d %4d\n", i, pad_len, 8*i + 8*pad_len+64);
	}
	return;

	MD4_Update(md, pad, compute_pad_len(md->count));
	md->count *= 8;
	MD4_Update(md, (unsigned char*)&md->count, sizeof(md->count));

	if (digest) {
		memcpy(digest, (unsigned char *)md->s._8, len);
	}
}

