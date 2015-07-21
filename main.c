
#include "md4.h"

#include <stdio.h>
#include <string.h>

static void do_md4(char const * s, char const * s2)
{
	md4_t md;
	MD4_Init(&md);
	MD4_Update(&md, s, strlen(s));
	printf("MD4(\"%s\") = \"", s);
	MD4_Final(&md, NULL, 0);
	printf("MD4(\"%s\") = \"%s\" real\n", s, s2);
}

int main()
{
	do_md4("", "31d6cfe0d16ae931b73c59d7e0c089c0");
	do_md4("a", "bde52cb31de33e46245e05fbdbd6fb24");
	return 0;
}

