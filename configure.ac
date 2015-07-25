
AC_PREREQ([2.68])

AC_INIT(cpt, [0.1])
AC_CONFIG_SRCDIR(cpt.c)
AC_CONFIG_MACRO_DIR([m4])
AC_CANONICAL_SYSTEM

AM_INIT_AUTOMAKE
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES(yes)])

AC_PROG_CC_C99

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

