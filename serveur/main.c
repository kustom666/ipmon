#include <string.h>
#include "handler.h"
#include "../Core/network.h"
#include "../Core/pokeheader.h"
#include "../Core/pokepacket.h"
#include "../Core/utility.h"
#include "../Core/generator.h"

int main (int argc, char **argv)
{
	winsock_init();
	SOCKET sock = setup_socket();
	SOCKADDR_IN listen = setup_addr();
	int nbpack = 0;

	struct hostent *host_info = NULL;
	SOCKADDR_IN from = {0};
	int fromsize = sizeof from;
	SOCKADDR_IN to;

	//Nos structures utilisées pour l'envoi/réception de packets
	pokeheader head;
	pokepacket pack;
	char *buffer = (char*) malloc(1024*sizeof(char));
	char *buff_send = (char*) malloc(1024*sizeof(char));

	if(bind(sock, (SOCKADDR *) &listen, sizeof listen) == SOCKET_ERROR)
	{
		perror("Impossible de bind le port");
		exit(1);
	}

	do
	{
		memset(buffer, 0, 1024);
		memset(buff_send, 0, 1024);

		int n = recvfrom(sock, buffer, 1024*sizeof(char), 0, (SOCKADDR *)&from, &fromsize);

		if ( n  < 0)
		{
			perror("Errreur de réception");
			exit(1);
		}

		pokepacket recv_pack = unserialize_pokepacket(buffer);
		char *recv_ip = inet_ntoa(from.sin_addr);
		//printf("Type : %d\nID : %d\nTaille donnees : %d \nDonnes : %s\n",recv_pack.header.type,recv_pack.header.id, recv_pack.header.data_size, recv_pack.data );

		if(strcmp(pck_type(buffer), TAG_NOUV) == 0)
		{
			handle_nouveau(sock,buffer, (SOCKADDR *)&from, sizeof(from), recv_ip);
		}
		else if(strcmp(pck_type(buffer), TAG_DINI) == 0)
		{
			handle_duel(sock,recv_pack.data, (SOCKADDR *)&from, sizeof(from), recv_ip);
		}
		else
		{
			printf("Type de packet inconnu :\n%s\n", buffer);
		}

	}while(1==1);

	winsock_end();
	
	free(buffer);
	free(buff_send);
	return 0;

}