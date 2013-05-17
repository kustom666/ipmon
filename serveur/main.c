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

	if(bind(sock, (SOCKADDR *) &listen, sizeof listen) == SOCKET_ERROR)
	{
		perror("Impossible de bind le port");
		exit(1);
	}

	char buffer[1024];
	struct hostent *host_info = NULL;
	SOCKADDR_IN from = {0};
	int fromsize = sizeof from;
	pokeheader recv_head;
	pokepacket recv_pack;

	do
	{

		int n = recvfrom(sock, buffer, 1024*sizeof(char), 0, (SOCKADDR *)&from, &fromsize);
		if ( n  < 0)
		{
			perror("Errreur de réception");
			exit(1);
		}
		
		if(strcmp(pck_type(buffer), TAG_LOGI) == 0)
		{
			printf("RECV : %d\n", n);
			printf("New login!\n");
			pokeheader recv_header = unserialize_header(buffer);
			printf("Type : %d\n ID : %d\nTaille données : %d \n",recv_header.type,recv_header.id, recv_header.data_size );
		
		}
		else
		{
			printf("Type de packet inconnu :\n%s\n", buffer);
		}
		//printf("Data Type : %s - ID : %d - Data_Length : %d\n", recv_head.type, recv_head.id, recv_head.data_size);
	
	}while(strcmp("DISCONNECT",buffer) != 0);

	winsock_end();
	return 0;

}