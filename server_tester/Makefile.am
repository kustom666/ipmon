bin_PROGRAMS = server_tester

AM_CCFLAGS = $(INIT_CFLAGS) -fwritable-strings

server_tester_SOURCES = main.c ../Core/network.h ../Core/network.c ../Core/pokeheader.h ../Core/pokepacket.h ../Core/pokepacket.c ../Core/pokeheader.c ../Core/utility.c ../Core/utility.h ../Core/generator.h ../Core/generator.c ../Core/pokemon.h ../Core/pokemon.c

server_tester_LDADD = $(INIT_LIBS)