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

pokepacket unserialize_pokepacket(char *data)
{
	pokeheader head = unserialize_header(data);
	int payload_size = head.data_size;
	char *buff_payload = (char *) malloc(payload_size+1 * sizeof(char));
	range_strcpy(buff_payload, data, 6, payload_size+6);

	pokepacket packet = {head, buff_payload};
	return packet;
}