
#include "md.h"

#include <string.h>
#include <stdio.h>

void MD_Update(md_t * md, unsigned char const * d, size_t len, MD_ROUNDS_PROC rounds)
{
	size_t used = (md->count & 0x3f);
	size_t left = 0x40 - used;

	if (left > 0) {
		size_t count = left < len ? left : len;
		memcpy(&md->b._8[used], d, count);
		len -= count;
		d += count;
		md->count += count;
	}

	if (0x40 == md->count) {
		rounds(md, md->b._32, 0x40);
	}

	while (len > 0x40) {
		rounds(md, (uint32_t const*)d, 0x40);
		len -= 0x40;
		d += 0x40;
		md->count += 0x40;
	}

	used = (md->count & 0x3f);
	memcpy(&md->b._8[used], d, len);
	md->count += len;
}

static inline int compute_pad_len(int count)
{
	return 64 - ((count + 8) & 0x3f);
}

void MD_Final(md_t * md, unsigned char * digest, size_t len, MD_ROUNDS_PROC rounds)
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
	MD_Update(md, pad, compute_pad_len(md->count), rounds);
	MD_Update(md, (unsigned char*)&bits, sizeof(bits), rounds);

	if (digest) {
		memcpy(digest, (unsigned char *)md->s._8, len);
	}
}

