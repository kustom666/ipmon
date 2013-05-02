#include <string.h>
#include "../core/network.h"
#include "../core/pokeheader.h"
#include "../core/pokepacket.h"

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
		//pokeheader head = serialize_header(test_head);
		int n = recvfrom(sock,buffer, sizeof buffer - 1, 0, (SOCKADDR *)&from, &fromsize);
		if ( n  < 0)
		{
			perror("Errreur de réception");
			exit(1);
		}
		recv_head = serialize_header(buffer);
	
	}while(strcmp("DISCONNECT",buffer) != 0);

	return 0;

}