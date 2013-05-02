all: serveur client

serveur: main_server.o network.o utility.o pokeheader.o 
	gcc -o ipmon_serveur serveur/main_server.o core/network.o core/utility.o core/pokeheader.o 

main_server.o: serveur/main.c 
	gcc -o serveur/main_server.o -c serveur/main.c

network.o: core/network.c core/network.h
	gcc -o core/network.o -c core/network.c

utility.o: core/utility.c core/utility.h
	gcc -o core/utility.o -c core/utility.c

pokeheader.o: core/pokeheader.c core/pokeheader.h
	gcc -o core/pokeheader.o -c core/pokeheader.c




