
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROTL32(v, n) (((v) << (n)) | ((v) >> (32 - (n))))
#define ROTATE(v, c) ROTL32((v), (c))
#define XOR(v, w) ((v) ^ (w))
#define PLUS(x, y) ((x) + (y))

#ifdef BIGE
#define U32TO8_LITTLE(p, v) do { \
	(p)[0] = ((v)      ) & 0xff; \
	(p)[1] = ((v) >>  8) & 0xff; \
	(p)[2] = ((v) >> 16) & 0xff; \
	(p)[3] = ((v) >> 24) & 0xff; \
} while (0)
#define U8TO32_LITTLE(p) \
	(((uint32_t)((p)[0])      ) | \
	 ((uint32_t)((p)[1]) <<  8) | \
	 ((uint32_t)((p)[2]) << 16) | \
	 ((uint32_t)((p)[3]) << 24) )
#else
#define U32TO8_LITTLE(p, v) ((((uint32_t*)(p))[0]) = (((uint32_t)(v))))
#define U8TO32_LITTLE(p) (((uint32_t*)(p))[0])
#endif

#define QUARTERROUND(a,b,c,d) do { \
	x[a] = PLUS(x[a],x[b]); x[d] = ROTATE(XOR(x[d],x[a]),16); \
	x[c] = PLUS(x[c],x[d]); x[b] = ROTATE(XOR(x[b],x[c]),12); \
	x[a] = PLUS(x[a],x[b]); x[d] = ROTATE(XOR(x[d],x[a]), 8); \
	x[c] = PLUS(x[c],x[d]); x[b] = ROTATE(XOR(x[b],x[c]), 7); \
} while (0)

static void ChaChaCore(unsigned char output[64], const uint32_t input[16], int num_rounds)
{
	uint32_t x[16];

	memcpy(x, input, sizeof(uint32_t) * 16);

	for (int i = num_rounds; i > 0; i -= 2) {
		QUARTERROUND( 0, 4, 8,12);
		QUARTERROUND( 1, 5, 9,13);
		QUARTERROUND( 2, 6,10,14);
		QUARTERROUND( 3, 7,11,15);
		QUARTERROUND( 0, 5,10,15);
		QUARTERROUND( 1, 6,11,12);
		QUARTERROUND( 2, 7, 8,13);
		QUARTERROUND( 3, 4, 9,14);
	}

	for (int i = 0; i < 16; ++i) {
		U32TO8_LITTLE(output + 4 * i, x[i] + input[i]);
	}
}

typedef struct {
	uint64_t counter;
	int rounds;
	unsigned char key[32];
	unsigned char nonce[8];
} chacha_ctx_t;

void chacha_crypt(chacha_ctx_t * ctx, void * _buffer, size_t len)
{
	unsigned char * buffer = (unsigned char*)_buffer;
	unsigned char const sigma[] = {"expand 32-byte k"};
	uint32_t input[16];

	memcpy(input, sigma, 16);
	input[4] = U8TO32_LITTLE(ctx->key + 0);
	input[5] = U8TO32_LITTLE(ctx->key + 4);
	input[6] = U8TO32_LITTLE(ctx->key + 8);
	input[7] = U8TO32_LITTLE(ctx->key + 12);
	input[8] = U8TO32_LITTLE(ctx->key + 16);
	input[9] = U8TO32_LITTLE(ctx->key + 20);
	input[10] = U8TO32_LITTLE(ctx->key + 24);
	input[11] = U8TO32_LITTLE(ctx->key + 28);
	memcpy(&input[12], &ctx->counter, sizeof(ctx->counter));
	input[14] = U8TO32_LITTLE(ctx->nonce + 0);
	input[15] = U8TO32_LITTLE(ctx->nonce + 4);

	while (len >= 64) {
		unsigned char block[64];
		ChaChaCore(block, input, ctx->rounds);
		for (int i = 0; i < 64; i += 8) {
			buffer[i + 0] ^= block[i + 0];
			buffer[i + 1] ^= block[i + 1];
			buffer[i + 2] ^= block[i + 2];
			buffer[i + 3] ^= block[i + 3];
			buffer[i + 4] ^= block[i + 4];
			buffer[i + 5] ^= block[i + 5];
			buffer[i + 6] ^= block[i + 6];
			buffer[i + 7] ^= block[i + 7];
		}
		ctx->counter++;
		memcpy(&input[12], &ctx->counter, sizeof(ctx->counter));
		len -= 64;
		buffer += 64;
	}
	if (len > 0) {
		unsigned char block[64];
		ChaChaCore(block, input, ctx->rounds);
		for (int i = 0; i < len; ++i) {
			buffer[i] ^= block[i];
		}
	}
}

#define chacha_encrypt chacha_crypt
#define chacha_decrypt chacha_crypt

chacha_ctx_t * chacha_new_ctx(
	unsigned char const * key,
	size_t keylen,
	unsigned char const * nonce,
	size_t noncelen,
	int counter,
	int rounds,
	int flags)
{
	chacha_ctx_t * retval = malloc(sizeof(chacha_ctx_t));
	memset(retval, 0, sizeof(chacha_ctx_t));
	retval->counter = counter;
	retval->rounds = rounds;

	return retval;
}

int main(void)
{
	chacha_ctx_t * chacha = chacha_new_ctx(NULL, 0, NULL, 0, 0, ROUNDS, 0);

	char buffer[16*1024];
	int rc;
	while (rc = fread(buffer, 1, sizeof(buffer), stdin), rc > 0) {
		chacha_crypt(chacha, buffer, rc);
		int wc = fwrite(buffer, 1, rc, stdout);
		if (rc != wc) {
			fprintf(stderr, "failed to write, %s\n", strerror(errno));
			exit(-1);
		}
	}
	return 0;
}

