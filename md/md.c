
#include "md.h"

#include <string.h>
#include <stdio.h>

void MD_Update(md_t * md, unsigned char const * d, size_t len, MD_ROUNDS_PROC rounds)
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

