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
	int fromsize = sizeof from;

	do
	{
		
		int n = recvfrom(sock,buffer, sizeof buffer - 1, 0, (SOCKADDR *)&from, &fromsize);
		if ( n  < 0)
		{
			perror("Errreur de réception");
			exit(1);
		}
	
		printf("Buffer : %s", buffer);
	}while(strcmp("DISCONNECT",buffer) != 0);

	return 0;

}