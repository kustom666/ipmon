#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.65])
AC_INIT(IPmon, 0.0.1, paul@bytechain.com)
AM_INIT_AUTOMAKE(IPmon, 0.0.1)
# Checks for programs.
AC_PROG_CC
AC_PROG_CC_STDC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h netdb.h netinet/in.h stdint.h stdlib.h string.h sys/socket.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_UINT8_T
AC_TYPE_UINT32_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([gethostbyname socket])

AC_OUTPUT(Makefile server_tester/Makefile serveur/Makefile client/Makefile)
