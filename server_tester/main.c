#include <string.h>
#include "../core/network.h"
#include "../core/pokeheader.h"
#include "../core/pokepacket.h"

int main(int arcg, char **argv)
{	
	char buffer[1024];

	SOCKET sock = setup_socket();
	SOCKADDR_IN to = setup_send_addr();
	int to_size = sizeof(to);
	int status;

	strcpy(buffer, argv[1]);

	status = sendto(sock, buffer, strlen(buffer), 0, (SOCKADDR *)&to, to_size);

	if(status < 0)
	{
		fprintf(stderr, "Erreur lors de l'initialisation de l'envoi du packet");
		exit(1);
	}


	return 0;
}