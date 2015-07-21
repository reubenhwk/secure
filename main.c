
#include "md4.h"

#include <stdio.h>
#include <string.h>

int main()
{
	char const * test = "The quick brown fox jumps over the lazy dog";
	md4_t md;
	MD4_Init(&md);
	MD4_Update(&md, test, strlen(test));
	MD4_Final(&md, NULL, 0);
	printf("1bee69a46ba811185c194762abaeae90\n");
}

