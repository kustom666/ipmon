#include <string.h>
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
	char buffer[1024];

	struct hostent *host_info = NULL;
	SOCKADDR_IN from = {0};
	int fromsize = sizeof from;
	SOCKADDR_IN to;

	//Nos structures utilisées pour l'envoi/réception de packets
	pokeheader head;
	pokepacket pack;
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
		printf("Type : %d\nID : %d\nTaille donnees : %d \nDonnes : %s\n",recv_pack.header.type,recv_pack.header.id, recv_pack.header.data_size, recv_pack.data );

		if(strcmp(pck_type(buffer), TAG_NOUV) == 0)
		{
			printf("Nouveau login depuis : %s\n", recv_ip);
			printf("Recu : %s\n", buffer);

			int pck_size = generate_packet(recv_pack.header.id+1, &pack, &head, TAG_DONE, &buff_send);

			if(pck_size == 0)
			{
				printf("Erreur lors de la création du packet\n");
			}
			else
			{
				printf("Envoi de : %s\n", buff_send);
				sendto(sock , buff_send, pck_size, 0, (SOCKADDR *)&from, sizeof(from)); 
			}
		}
		else if(strcmp(pck_type(buffer), TAG_DINI) == 0)
		{
			printf("C'est l'heure du dudududududuel!\nEnvoyé depuis : %s\n", recv_ip);
			
		}
		else
		{
			printf("Type de packet inconnu :\n%s\n", buffer);
		}
	}while(1==1);

	winsock_end();
	free(buff_send);
	return 0;

}