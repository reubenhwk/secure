
AC_PREREQ([2.68])

AC_INIT(cpt, [0.1])
AC_CONFIG_SRCDIR(cpt)
AC_CONFIG_MACRO_DIR([m4])
AC_CANONICAL_SYSTEM

AM_INIT_AUTOMAKE
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES(yes)])

AC_PROG_CC_C99
AX_PROG_NASM([nasm_avail="no"])

AC_DEFINE(TEST)

if test -z "$nasm_avail" ; then
	RC4_ASM="rc4/rc4-x64.o"
	RC4_ASM_FLAGS="-DRC4_ASM"
	AC_SUBST([RC4_ASM])
	AC_SUBST([RC4_ASM_FLAGS])

	MD4_ASM="md4/md4-x64.o"
	MD4_ASM_FLAGS="-DMD4_ASM"
	AC_SUBST([MD4_ASM])
	AC_SUBST([MD4_ASM_FLAGS])
fi

AC_OUTPUT(
	Makefile
)

cat > buildinfo.txt << EOF

Your build configuration:

	CC: $CC
	LD: $LD
	CPPFLAGS = $CPPFLAGS
	CFLAGS = $CFLAGS
	LDFLAGS = $LDFLAGS
	Version: $VERSION
	prefix: $prefix

EOF

cat buildinfo.txt

