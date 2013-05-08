#include "network.h"

SOCKET setup_socket()
{
	SOCKET sock =  socket(AF_INET, SOCK_DGRAM,0); // Création du socket sur protocole UDP

	if(sock == -1) // Vérification que le socket est bien initialisé
	{
		perror("Erreur lors de la création du socket");
		exit(1);
	}
	return sock;
}

SOCKADDR_IN setup_addr()
{
	SOCKADDR_IN in = { 0 };
	in.sin_addr.s_addr = htonl(INADDR_ANY);
	in.sin_family = AF_INET;
	in.sin_port = htons(l_port);

	return in;
}

SOCKADDR_IN setup_send_addr()
{
	SOCKADDR_IN to = {0};
	const char *hostname = "localhost";
	struct hostent *hostinfo = NULL;
	hostinfo = gethostbyname(hostname);

	if(hostinfo == NULL)
	{
		fprintf(stderr, "Unknown host %s\n", hostname);
		exit(1);
	}

	to.sin_addr = *(IN_ADDR *) hostinfo->h_addr;
	to.sin_port = htons(l_port);
	to.sin_family = AF_INET;

	return to;
}

int send_pokeheader(SOCKET socket, const SOCKADDR *dest, socklen_t dlen, pokeheader *header)
{
	unsigned char buffer[10], *buff;

	buff = serialize_header(header, buffer);
	return sendto(socket,buff, sizeof(buff), 0, dest, dlen) == sizeof(buff);
}