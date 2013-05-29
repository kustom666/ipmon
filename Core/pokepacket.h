#ifndef DEF_POKEPACKET
#define DEF_POKEPACKET

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "pokeheader.h"
#include "network.h"
#include "utility.h"

typedef struct pokepacket{
	pokeheader header;
	char *data;
}pokepacket;

pokepacket unserialize_pokepacket(char *data);
char * forge_packet(char *origine, char *data, int size_origine, int size_data);
int send_pokepacket(SOCKET sock, char *type, char *pack_char, int size_pack, SOCKADDR_IN to, int to_size);

#endif