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