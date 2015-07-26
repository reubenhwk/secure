
AUTOMAKE_OPTIONS = 1.2 foreign subdir-objects

%.o: %.asm
	$(AM_V_GEN)nasm -felf64 $<

bin_PROGRAMS = \
	cpt-md4 \
	cpt-md5 \
	cpt-rc4 \
	# end bin_PROGRAMS

cpt_rc4_CPPFLAGS = \
	-DASSEMBLY

cpt_rc4_DEPENDENCIES = \
	rc4/rc4-x64.o

cpt_rc4_LDADD = \
	rc4/rc4-x64.o

cpt_rc4_SOURCES = \
	rc4/rc4-common.c \
	rc4/rc4.c

cpt_md4_SOURCES = \
	md4/md4.c


cpt_md5_SOURCES = \
	md5/md5.c


