
AC_PREREQ([2.68])

AC_INIT(cpt, [0.1])
AC_CONFIG_SRCDIR(cpt)
AC_CONFIG_MACRO_DIR([m4])
AC_CANONICAL_SYSTEM

AM_INIT_AUTOMAKE
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES(yes)])

AC_PROG_CC_C99
AX_PROG_NASM([nasm_avail="no"])

AC_ARG_ENABLE([asm],
  AS_HELP_STRING([--enable-asm],
    [use assembly language implementations (if abailable)]),
)

AC_ARG_ENABLE([rc4-asm],
  AS_HELP_STRING([--enable-rc4-asm],
    [use rc4 assembly language implementation (if abailable)]),
)

AC_ARG_ENABLE([md4-asm],
  AS_HELP_STRING([--enable-md4-asm],
    [use md4 assembly language implementation (if abailable)]),
)

AC_ARG_ENABLE([md5-asm],
  AS_HELP_STRING([--enable-md5-asm],
    [use md5 assembly language implementation (if abailable)]),
)

case $target in
	x86_64*darwin*) NASMFLAGS=-fmacho64 ;;
	x86_64*linux*) NASMFLAGS=-felf64 ;;
	*) NASMFLAGS="" ;;
esac

AC_SUBST([NASMFLAGS])

if test "$enable_asm" != "no" ; then
	if test -z "$nasm_avail" ; then
		changequote(<<,>>)
		if [ "$enable_rc4_asm" != "no" ] && \
			[[ -x rc4/rc4-$target_cpu.sh || -f rc4/rc4-$target_cpu.asm ]] ; then
		changequote([,])
			RC4_ASM="rc4/rc4-x86_64.o"
			RC4_ASM_FLAGS="-DRC4_ASM"
			AC_SUBST([RC4_ASM])
			AC_SUBST([RC4_ASM_FLAGS])
			AC_MSG_NOTICE([using rc4 assembly (disable with --disable-rc4-asm)])
		fi

		changequote(<<,>>)
		if [ "$enable_md4_asm" != "no" ] && \
			[[ -x md4/md4-$target_cpu.sh || -f md4/md4-$target_cpu.asm ]] ; then
		changequote([,])
			MD4_ASM="md4/md4-x86_64.o"
			MD4_ASM_FLAGS="-DMD4_ASM"
			AC_SUBST([MD4_ASM])
			AC_SUBST([MD4_ASM_FLAGS])
			AC_MSG_NOTICE([using md4 assembly (disable with --disable-md4-asm)])
		fi

		changequote(<<,>>)
		if [ "$enable_md5_asm" != "no" ] && \
			[[ -x md5/md5-$target_cpu.sh || -f md5/md5-$target_cpu.asm ]] ; then
		changequote([,])
			MD5_ASM="md5/md5-x86_64.o"
			MD5_ASM_FLAGS="-DMD5_ASM"
			AC_SUBST([MD5_ASM])
			AC_SUBST([MD5_ASM_FLAGS])
			AC_MSG_NOTICE([using md5 assembly (disable with --disable-md5-asm)])
		fi
	fi
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

