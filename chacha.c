
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#define ROTL32(v, n) (((v) << (n)) | ((v) >> (32 - (n))))
#define ROTATE(v, c) ROTL32((v), (c))
#define XOR(v, w) ((v) ^ (w))
#define PLUS(x, y) ((x) + (y))
#define U32TO8_LITTLE(p, v) \
{ (p)[0] = ((v)      ) & 0xff; (p)[1] = ((v) >>  8) & 0xff; \
(p)[2] = ((v) >> 16) & 0xff; (p)[3] = ((v) >> 24) & 0xff; }

#define U8TO32_LITTLE(p)   \
(((uint32_t)((p)[0])      ) | ((uint32_t)((p)[1]) <<  8) | \
((uint32_t)((p)[2]) << 16) | ((uint32_t)((p)[3]) << 24)   )

#define QUARTERROUND(a,b,c,d) \
x[a] = PLUS(x[a],x[b]); x[d] = ROTATE(XOR(x[d],x[a]),16); \
x[c] = PLUS(x[c],x[d]); x[b] = ROTATE(XOR(x[b],x[c]),12); \
x[a] = PLUS(x[a],x[b]); x[d] = ROTATE(XOR(x[d],x[a]), 8); \
x[c] = PLUS(x[c],x[d]); x[b] = ROTATE(XOR(x[b],x[c]), 7);

static void ChaChaCore(unsigned char output[64], const uint32_t input[16], int num_rounds)
{
	uint32_t x[16];
	int i;

	memcpy(x, input, sizeof(uint32_t) * 16);

	for (i = num_rounds; i > 0; i -= 2) {
		QUARTERROUND( 0, 4, 8,12)
		QUARTERROUND( 1, 5, 9,13)
		QUARTERROUND( 2, 6,10,14)
		QUARTERROUND( 3, 7,11,15)
		QUARTERROUND( 0, 5,10,15)
		QUARTERROUND( 1, 6,11,12)
		QUARTERROUND( 2, 7, 8,13)
		QUARTERROUND( 3, 4, 9,14)
	}

	for (i = 0; i < 16; ++i) {
		x[i] = PLUS(x[i], input[i]);
	}

	for (i = 0; i < 16; ++i) {
		U32TO8_LITTLE(output + 4 * i, x[i]);
	}
}

void ChaCha20XOR(
	unsigned char *out,
	const unsigned char *in,
	unsigned int inLen,
	const unsigned char key[32],
	const unsigned char nonce[8],
	uint64_t counter)
{
	static const unsigned char sigma[16] = "expand 32-byte k";
	unsigned char block[64];
	uint32_t input[16];
	unsigned int u;
	unsigned int i;

	input[4] = U8TO32_LITTLE(key + 0);
	input[5] = U8TO32_LITTLE(key + 4);
	input[6] = U8TO32_LITTLE(key + 8);
	input[7] = U8TO32_LITTLE(key + 12);
	input[8] = U8TO32_LITTLE(key + 16);
	input[9] = U8TO32_LITTLE(key + 20);
	input[10] = U8TO32_LITTLE(key + 24);
	input[11] = U8TO32_LITTLE(key + 28);
	input[0] = U8TO32_LITTLE(sigma + 0);
	input[1] = U8TO32_LITTLE(sigma + 4);
	input[2] = U8TO32_LITTLE(sigma + 8);
	input[3] = U8TO32_LITTLE(sigma + 12);
	input[12] = counter;
	input[13] = counter >> 32;
	input[14] = U8TO32_LITTLE(nonce + 0);
	input[15] = U8TO32_LITTLE(nonce + 4);

	while (inLen >= 64) {
		ChaChaCore(block, input, 20);
		for (i = 0; i < 64; i++) {
			out[i] = in[i] ^ block[i];
		}
		input[12]++;
		if (input[12] == 0) {
			input[13]++;
		}
		inLen -= 64;
		in += 64;
		out += 64;
	}
	if (inLen > 0) {
		ChaChaCore(block, input, 20);
		for (i = 0; i < inLen; i++) {
			out[i] = in[i] ^ block[i];
		}
	}
}

