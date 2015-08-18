
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROTL32(v, n) (((v) << (n)) | ((v) >> (32 - (n))))
#define ROTATE(v, c) ((v) = ROTL32((v), (c)))

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

static inline void chacha_round(uint32_t x[16], int a, int b, int c, int d)
{
	x[a] += x[b]; x[d] ^= x[a]; ROTATE(x[d], 16);
	x[c] += x[d]; x[b] ^= x[c]; ROTATE(x[b], 12);
	x[a] += x[b]; x[d] ^= x[a]; ROTATE(x[d],  8);
	x[c] += x[d]; x[b] ^= x[c]; ROTATE(x[b],  7);
}

static inline void chacha_rounds(uint32_t x[16])
{
	chacha_round(x, 0, 4, 8,12);
	chacha_round(x, 1, 5, 9,13);
	chacha_round(x, 2, 6,10,14);
	chacha_round(x, 3, 7,11,15);
	chacha_round(x, 0, 5,10,15);
	chacha_round(x, 1, 6,11,12);
	chacha_round(x, 2, 7, 8,13);
	chacha_round(x, 3, 4, 9,14);
}

static void ChaChaCore(unsigned char output[64], const uint32_t matrix[16], int num_rounds)
{
	uint32_t x[16];

	memcpy(x, matrix, sizeof(uint32_t) * 16);

	for (int i = num_rounds; i > 0; i -= 2) {
		chacha_rounds(x);
	}

	for (int i = 0; i < 16; ++i) {
		U32TO8_LITTLE(output + 4 * i, x[i] + matrix[i]);
	}
}

typedef struct {
	uint64_t counter;
	uint64_t nonce;
	int used;
	int rounds;
	unsigned char block[64];
	unsigned char key[32];
} chacha_ctx_t;

void init_chacha_matrix(chacha_ctx_t const * ctx, uint32_t matrix[16])
{
	unsigned char const sigma[] = {"expand 32-byte k"};

	memcpy(matrix, sigma, 16);
	memcpy(matrix+4, ctx->key, sizeof(ctx->key));
	memcpy(matrix+14, &ctx->nonce, sizeof(ctx->nonce));
}

void chacha_crypt(chacha_ctx_t * ctx, void * _buffer, size_t len)
{
	if (0 == len) {
		return;
	}

	unsigned char * buffer = (unsigned char*)_buffer;

	/* Use up the remaining values in block. */
	while (len > 0 && ctx->used & 0x3f) {
		*(buffer++) ^= ctx->block[ctx->used++];
		--len;
	}

	uint32_t matrix[16];
	if (len > 64) {
		init_chacha_matrix(ctx, matrix);
	}

	/* If the block values have all been used, generate a new block */
	if (64 == ctx->used) {
		ctx->used = 0;
		memcpy(matrix+12, &ctx->counter, sizeof(ctx->counter));
		++ctx->counter;
		ChaChaCore(ctx->block, matrix, ctx->rounds);
	}

	/* Bulk xor the buffer with block and generate a new block */
	for (; len >= 64; buffer += 64, len -= 64) {
		*(uint64_t*)(buffer+0) ^= *(uint64_t*)(ctx->block+0);
		*(uint64_t*)(buffer+8) ^= *(uint64_t*)(ctx->block+8);
		*(uint64_t*)(buffer+16) ^= *(uint64_t*)(ctx->block+16);
		*(uint64_t*)(buffer+24) ^= *(uint64_t*)(ctx->block+24);
		*(uint64_t*)(buffer+32) ^= *(uint64_t*)(ctx->block+32);
		*(uint64_t*)(buffer+40) ^= *(uint64_t*)(ctx->block+40);
		*(uint64_t*)(buffer+48) ^= *(uint64_t*)(ctx->block+48);
		*(uint64_t*)(buffer+56) ^= *(uint64_t*)(ctx->block+56);
		memcpy(matrix+12, &ctx->counter, sizeof(ctx->counter));
		++ctx->counter;
		ChaChaCore(ctx->block, matrix, ctx->rounds);
	}

	/* xor the remaining value in the buffer and track how much
	 * of the block is used in the process. */
	while (len--) {
		*(buffer++) ^= ctx->block[ctx->used++];
	}
}

#define chacha_encrypt chacha_crypt
#define chacha_decrypt chacha_crypt

chacha_ctx_t * chacha_new_ctx(
	unsigned char const * key,
	size_t keylen,
	uint64_t nonce,
	int counter,
	int rounds,
	int flags)
{
	chacha_ctx_t * retval = malloc(sizeof(chacha_ctx_t));
	memset(retval, 0, sizeof(chacha_ctx_t));
	retval->counter = counter;
	retval->nonce = nonce;
	retval->rounds = rounds;

	/* Generate the first block */
	uint32_t matrix[16];
	init_chacha_matrix(retval, matrix);
	memcpy(matrix+12, &retval->counter, sizeof(retval->counter));
	++retval->counter;
	ChaChaCore(retval->block, matrix, retval->rounds);

	return retval;
}

int main(void)
{
	chacha_ctx_t * chacha = chacha_new_ctx(NULL, 0, 0, 0, ROUNDS, 0);

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

