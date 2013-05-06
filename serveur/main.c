#include <string.h>
#include "../Core/network.h"
#include "../Core/pokeheader.h"
#include "../Core/pokepacket.h"

int main (int argc, char **argv)
{
	SOCKET sock = setup_socket();
	SOCKADDR_IN listen = setup_addr();

	if(bind(sock, (SOCKADDR *) &listen, sizeof listen) == -1)
	{
		perror("Impossible de bind le port");
		exit(1);
	}

	char buffer[1024];
	struct hostent *host_info = NULL;
	SOCKADDR_IN from = {0};
	socklen_t fromsize = sizeof from;
	char test_head[6] = "DATAXX";
	pokeheader recv_head;
	pokepacket recv_pack;

	do
	{

		int n = recvfrom(sock, &buffer, sizeof buffer, 0, (SOCKADDR *)&from, &fromsize);
		if ( n  < 0)
		{
			perror("Errreur de réception");
			exit(1);
		}
		printf("%s\n", buffer);
		//printf("Data Type : %s - ID : %d - Data_Length : %d\n", recv_head.type, recv_head.id, recv_head.data_size);
	
	}while(strcmp("DISCONNECT",buffer) != 0);


	return 0;

}