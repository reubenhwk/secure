
#include "md4.h"

#include <stdio.h>
#include <string.h>


int main()
{
	md4_t ctx;
	MD4_Init(&ctx);
#if 0
	MD4_Update(&ctx, "a", 1);
#else
	char buffer[4*1024];
	int rc;
	while (rc = fread(buffer, 1, sizeof(buffer), stdin), rc > 0) {
		MD4_Update(&ctx, (unsigned char*)buffer, rc);
	}
#endif
	unsigned char dgst[16];
	MD4_Final(&ctx, dgst, 16);

	for (int i = 0; i < 16; ++i) {
		printf("%02x", dgst[i]);
	}
	printf("\n");

	return 0;
}

