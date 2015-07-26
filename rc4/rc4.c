
#include "rc4.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef RC4_ASM
static inline unsigned char rc4_get_next_byte(rc4_ctx_t * const rc4)
{
	++rc4->i;
	unsigned char const tmpi = rc4->S[rc4->i];
	rc4->j += tmpi;
	unsigned char const tmpj = rc4->S[rc4->j];
	rc4->S[rc4->i] = tmpj;
	rc4->S[rc4->j] = tmpi;
	unsigned char const index = tmpi + tmpj;
	return rc4->S[index];
}

void rc4_crypt(rc4_ctx_t * rc4, void * _buffer, size_t len)
{
	unsigned char * buffer = (unsigned char*)_buffer;
	int i = (int)(len >> 2);
	while (i--) {
		*buffer++ ^= rc4_get_next_byte(rc4);
		*buffer++ ^= rc4_get_next_byte(rc4);
		*buffer++ ^= rc4_get_next_byte(rc4);
		*buffer++ ^= rc4_get_next_byte(rc4);
	}

	i = (int)(len & 0x3);
	while (i--) {
		*buffer++ ^= rc4_get_next_byte(rc4);
	}
}
#endif

#ifdef TEST
int main(void)
{
	rc4_ctx_t * rc4 = rc4_new_ctx(NULL, 0, 0);

	char buffer[16*1024];
	int rc;
	while (rc = fread(buffer, 1, sizeof(buffer), stdin), rc > 0) {
		rc4_crypt(rc4, buffer, rc);
		int wc = fwrite(buffer, 1, rc, stdout);
		if (rc != wc) {
			fprintf(stderr, "failed to write, %s\n", strerror(errno));
			exit(-1);
		}
	}
	rc4_free_ctx(rc4);
	return 0;
}
#endif

