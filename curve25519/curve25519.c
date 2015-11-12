
#include "curve25519.h"

#include <stdio.h>
#include <stdlib.h>

#ifdef DEFINE_CURVE25519_MAIN
#include <time.h>
static void print_key(curve25519_key_t const * key)
{
	for (int i = 0; i < CURVE22519_COUNT; ++i) {
		printf("%02x", key->values[i]);
	}
}

int main(int argc, char * argv[])
{
	struct timespec tv = {};
	clock_gettime(CLOCK_REALTIME, &tv);
	srand(tv.tv_sec * tv.tv_nsec);

	curve25519_key_t prikey1 = curve25519_generate_private();
	curve25519_key_t prikey2 = curve25519_generate_private();

	curve25519_key_t pubkey1 = curve25519_compute_public(&prikey1);
	curve25519_key_t pubkey2 = curve25519_compute_public(&prikey2);

	curve25519_value_t secret1;
	curve25519_value_t secret2;

	curve25519_compute_secret(&secret1, &prikey1, &pubkey2);
	curve25519_compute_secret(&secret2, &prikey2, &pubkey1);

	print_key((curve25519_key_t*)&secret1);
	printf(" = ");
	print_key(&prikey1);
	printf(" * ");
	print_key(&pubkey2);
	printf("\n");

	print_key((curve25519_key_t*)&secret2);
	printf(" = ");
	print_key(&prikey2);
	printf(" * ");
	print_key(&pubkey1);
	printf("\n");


	return 0;
}
#endif

