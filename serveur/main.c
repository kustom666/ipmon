#include <string.h>
#include "../Core/network.h"
#include "../Core/pokeheader.h"
#include "../Core/pokepacket.h"
#include "../Core/utility.h"

int main (int argc, char **argv)
{
	winsock_init();
	SOCKET sock = setup_socket();
	SOCKADDR_IN listen = setup_addr();

	SOCKADDR_IN to;

	if(bind(sock, (SOCKADDR *) &listen, sizeof listen) == SOCKET_ERROR)
	{
		perror("Impossible de bind le port");
		exit(1);
	}

	char buffer[1024];
	struct hostent *host_info = NULL;
	SOCKADDR_IN from = {0};
	int fromsize = sizeof from;
/*#define TAG_DATA "DATA"
#define TAG_NOUV "NOUV"
#define TAG_DONE "DONE"
#define TAG_DINI "DINI"
#define TAG_DACK "DACK"
#define TAG_ATCK "ATCK"
#define TAG_ISSU "ISSU"*/
	do
	{
		memset(buffer, 0, 1024);
		int n = recvfrom(sock, buffer, 1024*sizeof(char), 0, (SOCKADDR *)&from, &fromsize);

		if ( n  < 0)
		{
			perror("Errreur de réception");
			exit(1);
		}

		char *recv_ip = inet_ntoa(from.sin_addr);
		
		if(strcmp(pck_type(buffer), TAG_LOGI) == 0)
		{
			printf("Nouveau login depuis : %s\n", recv_ip);
			printf("Recu : %s\n", buffer);
			pokepacket recv_pack = unserialize_pokepacket(buffer);

			printf("Type : %d\nID : %d\nTaille donnees : %d \nDonnes : %s\n",recv_pack.header.type,recv_pack.header.id, recv_pack.header.data_size, recv_pack.data );
			sendto(sock ,"YO", 2, 0, (SOCKADDR *)&from, sizeof(from)); 
		}
		else if(strcmp(pck_type(buffer), TAG_NOUV) == 0)
		{
			printf("Nouveau joueur!\n");
			printf("Recu : %s\n", buffer);
			pokepacket recv_pack = unserialize_pokepacket(buffer);
			printf("Type : %d\nID : %d\nTaille donnees : %d \nDonnes : %s\n",recv_pack.header.type,recv_pack.header.id, recv_pack.header.data_size, recv_pack.data );
		}
		else if(strcmp(pck_type(buffer), TAG_DINI) == 0)
		{
			printf("C'est l'heure du dudududududuel!\n");
			printf("Recu : %s\n", buffer);
			pokepacket recv_pack = unserialize_pokepacket(buffer);
			printf("Type : %d\nID : %d\nTaille donnees : %d \nDonnes : %s\n",recv_pack.header.type,recv_pack.header.id, recv_pack.header.data_size, recv_pack.data );
		}
		else
		{
			printf("Type de packet inconnu :\n%s\n", buffer);
		}
		//printf("Data Type : %s - ID : %d - Data_Length : %d\n", recv_head.type, recv_head.id, recv_head.data_size);

	
	}while(1==1);

	winsock_end();
	return 0;

}