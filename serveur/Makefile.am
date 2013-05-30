bin_PROGRAMS = serveur

AM_CCFLAGS = $(INIT_CFLAGS) -fwritable-strings

serveur_SOURCES = main.c ../Core/network.h ../Core/network.c ../Core/pokeheader.h ../Core/pokepacket.c ../Core/pokepacket.h ../Core/pokeheader.c ../Core/utility.c ../Core/utility.h ../Core/generator.h ../Core/generator.c ./handler.h ./handler.c ../Core/pokemon.h ../Core/pokemon.c

serveur_LDADD = $(INIT_LIBS)