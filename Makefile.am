
AUTOMAKE_OPTIONS = 1.2 foreign subdir-objects

%.o: %.asm
	$(AM_V_GEN)nasm @NASMFLAGS@ $<

bin_PROGRAMS = \
	cpt-md4 \
	cpt-md5 \
	cpt-rc4 \
	# end bin_PROGRAMS

cpt_rc4_CPPFLAGS = \
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
	@MD5_ASM_FLAGS@

cpt_md5_DEPENDENCIES = \
	@MD5_ASM@

cpt_md5_LDADD = \
	@MD5_ASM@

cpt_md5_SOURCES = \
	md/md.c \
	md5/md5.c

