
#include "md.h"

#include <string.h>
#include <stdio.h>

md_t * MD_Init(md_t * md)
{
	if (!md) {
		md = malloc(sizeof(md_t));
	}

	md->s._32[0] = 0x67452301;
	md->s._32[1] = 0xefcdab89;
	md->s._32[2] = 0x98badcfe;
	md->s._32[3] = 0x10325476;

	md->count = 0;

	return md;
}

void MD_Update(md_t * md, unsigned char const * d, size_t len, MD_ROUNDS_PROC md_round)
{
	size_t const used = (md->count & 0x3f);
	if (used > 0) {
		size_t const left = 0x40 - used;
		size_t const count = left < len ? left : len;
		memcpy(&md->b._8[used], d, count);
		len -= count;
		d += count;
		md->count += count;

		if (0 == (md->count & 0x3f)) {
			md_round(md, md->b._32);
		}
	}

	while (len >= 0x40) {
		md_round(md, (uint32_t const*)d);
		len -= 0x40;
		d += 0x40;
		md->count += 0x40;
	}

	memcpy(&md->b._8[0], d, len);
	md->count += len;
}

static inline int compute_pad_len(int count)
{
	return 64 - ((count + 8) & 0x3f);
}

void MD_Final(md_t * md, unsigned char * digest, size_t len, MD_ROUNDS_PROC md_round)
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

#ifdef BIGENDIAN
	uint64_t bits = __builtin_bswap64(8 * md->count);
#else
	uint64_t bits = 8 * md->count;
#endif
	MD_Update(md, pad, compute_pad_len(md->count), md_round);
	MD_Update(md, (unsigned char*)&bits, sizeof(bits), md_round);

#ifdef BIGENDIAN
	for (int i = 0; i < 4; ++i) {
		md->s._32[i] = __builtin_bswap32(md->s._32[i]);
	}
#endif
	if (digest) {
		memcpy(digest, (unsigned char *)md->s._8, len);
	}
}

