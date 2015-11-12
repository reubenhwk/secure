
AUTOMAKE_OPTIONS = 1.2 foreign subdir-objects

check_SCRIPTS = \
	test/md4.sh \
	test/md5.sh \
	test/rc4.sh \
	test/chacha8.sh \
	test/chacha12.sh \
	test/chacha20.sh

TESTS = \
	test/md4.sh \
	test/md5.sh \
	test/rc4.sh \
	test/chacha8.sh \
	test/chacha12.sh \
	test/chacha20.sh

%.o: %.asm
	$(AM_V_GEN)nasm @NASMFLAGS@ $<

bin_PROGRAMS = \
	cpt-md4 \
	cpt-md5 \
	cpt-rc4 \
	cpt-chacha20 \
	cpt-chacha12 \
	cpt-chacha8 \
	cpt-curve25519 \
	# end bin_PROGRAMS

cpt_curve25519_CPPFLAGS = \
	-DDEFINE_CURVE25519_MAIN

cpt_curve25519_SOURCES = \
	curve25519/curve25519-donna.c \
	curve25519/curve25519.c

cpt_chacha8_CPPFLAGS = \
	-DDEFINE_CHACHA_MAIN \
	-DROUNDS=8

cpt_chacha8_SOURCES = \
	chacha/chacha.c

cpt_chacha12_CPPFLAGS = \
	-DDEFINE_CHACHA_MAIN \
	-DROUNDS=12

cpt_chacha12_SOURCES = \
	chacha/chacha.c

cpt_chacha20_CPPFLAGS = \
	-DDEFINE_CHACHA_MAIN \
	-DROUNDS=20

cpt_chacha20_SOURCES = \
	chacha/chacha.c

cpt_rc4_CPPFLAGS = \
	-DDEFINE_RC4_MAIN \
	@RC4_ASM_FLAGS@

cpt_rc4_DEPENDENCIES = \
	@RC4_ASM@

cpt_rc4_LDADD = \
	@RC4_ASM@

cpt_rc4_SOURCES = \
	rc4/rc4-common.c \
	rc4/rc4.c

md4/md4-x86_64.asm: md4/md4-x86_64.sh
	$(AM_V_GEN)$< > $@

cpt_md4_CPPFLAGS = \
	-DDEFINE_MD4_MAIN \
	@MD4_ASM_FLAGS@

cpt_md4_DEPENDENCIES = \
	@MD4_ASM@

cpt_md4_LDADD = \
	@MD4_ASM@

cpt_md4_SOURCES = \
	md/md.c \
	md4/md4.c

md5/md5-x86_64.asm: md5/md5-x86_64.sh
	$(AM_V_GEN)$< > $@

cpt_md5_CPPFLAGS = \
	-DDEFINE_MD5_MAIN \
	@MD5_ASM_FLAGS@

cpt_md5_DEPENDENCIES = \
	@MD5_ASM@

cpt_md5_LDADD = \
	@MD5_ASM@

cpt_md5_SOURCES = \
	md/md.c \
	md5/md5.c

