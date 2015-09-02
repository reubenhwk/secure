
#include "curve25519.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static void print_key(curve25519_key_t const * key)
{
	for (int i = 0; i < CURVE22519_COUNT; ++i) {
		printf("%02x", key->values[i]);
	}
	printf("\n");
}

int main(int argc, char * argv[])
{
	unsigned int seed = (unsigned int)&seed * time(NULL) * 1021;
	srand(seed);

	curve25519_key_t prikey1 = curve25519_generate_private();
	curve25519_key_t prikey2 = curve25519_generate_private();

	curve25519_key_t pubkey1 = curve25519_compute_public(&prikey1);
	curve25519_key_t pubkey2 = curve25519_compute_public(&prikey2);

	curve25519_key_t secret1;
	curve25519_key_t secret2;

	curve25519(&secret1, &prikey1, &pubkey2);
	curve25519(&secret2, &prikey2, &pubkey1);

	print_key(&prikey1);
	print_key(&prikey2);
	print_key(&pubkey1);
	print_key(&pubkey2);
	print_key(&secret1);
	print_key(&secret2);
	return 0;
}

