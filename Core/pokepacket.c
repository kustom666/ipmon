#include "pokepacket.h"

char * forge_packet(char *origine, char *data, int size_origine, int size_data)
{
	int taille = size_origine + size_data;
	char *output = malloc(taille * sizeof(char));
	strcpy(output, origine);
	strcat(output, data);
	
	return output;
}

int send_pokepacket(SOCKET sock, char *type, char *pack_char, int size_pack, SOCKADDR_IN to, int to_size)
{
	return sendto(sock, pack_char, size_pack, 0, (SOCKADDR *)&to, to_size) == size_pack; 
}