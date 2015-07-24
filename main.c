
#include "md4.h"

#include <stdio.h>
#include <string.h>


int main()
{
	md4_t ctx;
	MD4_Init(&ctx);
	char buffer[4*1024];
	int rc;
	while (rc = fread(buffer, 1, sizeof(buffer), stdin), rc > 0) {
		MD4_Update(&ctx, buffer, rc);
	}
	MD4_Final(&ctx, NULL, 0);
	return 0;
}

