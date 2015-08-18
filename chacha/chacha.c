
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* https://tools.ietf.org/html/rfc7539 */

typedef union {
	uint8_t u8[64];
	uint32_t u32[16];
	uint64_t u64[8];
} matrix_t;

typedef struct {
	matrix_t matrix;
	matrix_t block;
	int used;
	int rounds;
	int bige;
} chacha_ctx_t;

#define ROTL32(v, n) (((v) << (n)) | ((v) >> (32 - (n))))
#define ROTATE(v, c) ((v) = ROTL32((v), (c)))

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

static void ChaChaCore(chacha_ctx_t * ctx, int num_rounds)
{
	ctx->block = ctx->matrix;

	for (int i = num_rounds; i > 0; i -= 2) {
		chacha_rounds(ctx->block.u32);
	}

	for (int i = 0; i < 16; ++i) {
		ctx->block.u32[i] += ctx->matrix.u32[i];
	}

	/* The matrix may be in big or little endian, but the block must be
	 * little endian.  Swap all the bytes in the block on Big E systems. */
#ifdef _BIG_ENDIAN
		for (int i = 0; i < 16; ++i) {
			ctx->block.u32[i] = __builtin_bswap32(ctx->block.u32[i]);
		}
#endif

	++ctx->matrix.u32[12];
}

void chacha_crypt(chacha_ctx_t * ctx, void * _buffer, size_t len)
{
	if (0 == len) {
		return;
	}

	unsigned char * buffer = (unsigned char*)_buffer;

	/* Use up the remaining values in block. */
	while (len > 0 && ctx->used & 0x3f) {
		*(buffer++) ^= ctx->block.u8[ctx->used++];
		--len;
	}

	/* If the block values have all been used, generate a new block */
	if (64 == ctx->used) {
		ctx->used = 0;
		ChaChaCore(ctx, ctx->rounds);
	}

	/* Bulk xor the buffer with block and generate a new block */
	for (; len >= 64; buffer += 64, len -= 64) {
		uint64_t * b64 = (uint64_t*)buffer;
		b64[0] ^= ctx->block.u64[0];
		b64[1] ^= ctx->block.u64[1];
		b64[2] ^= ctx->block.u64[2];
		b64[3] ^= ctx->block.u64[3];
		b64[4] ^= ctx->block.u64[4];
		b64[5] ^= ctx->block.u64[5];
		b64[6] ^= ctx->block.u64[6];
		b64[7] ^= ctx->block.u64[7];
		ChaChaCore(ctx, ctx->rounds);
	}

	/* xor the remaining value in the buffer and track how much
	 * of the block is used in the process. */
	while (len--) {
		*(buffer++) ^= ctx->block.u8[ctx->used++];
	}
}

#define chacha_encrypt chacha_crypt
#define chacha_decrypt chacha_crypt

chacha_ctx_t * chacha_new_ctx(
	unsigned char const * key,
	size_t keylen,
	uint32_t counter,
	uint32_t nonce[3],
	int rounds,
	int flags)
{
	if (NULL == key) {
		key = "";
	}

	chacha_ctx_t * ctx = malloc(sizeof(chacha_ctx_t));
	memset(ctx, 0, sizeof(chacha_ctx_t));

	ctx->rounds = rounds;

	ctx->matrix.u32[0] = 0x61707865;
	ctx->matrix.u32[1] = 0x3320646e;
	ctx->matrix.u32[2] = 0x79622d32;
	ctx->matrix.u32[3] = 0x6b206574;

	/* The key is copied in little endian, so on Big E systems the key words
	 * need to be swapped. */
#ifdef _BIG_ENDIAN
	for (int i = 4; i < 12; ++i) {
		ctx->matrix.u32[i] = __builtin_bswap32(ctx->matrix.u32[i]);
	}
#endif

	ctx->matrix.u32[12] = counter;
	ctx->matrix.u32[13] = nonce[0];
	ctx->matrix.u32[14] = nonce[1];
	ctx->matrix.u32[15] = nonce[2];

	/* Generate the first block */
	ChaChaCore(ctx, ctx->rounds);

	return ctx;
}

int main(int argc, char * argv[])
{
	chacha_ctx_t * chacha = NULL;
	uint32_t nonce[3] = {0x1, 0x2, 0x3};

	chacha = chacha_new_ctx(
		"asdfasdfasdfasdf"
		"1234567887654321",
		32, 1024, nonce, ROUNDS, 0);

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

