#include "network.h"

SOCKET setup_socket()
{
	SOCKET sock =  socket(AF_INET, SOCK_DGRAM,0); // Création du socket sur protocole UDP

	if(sock == INVALID_SOCKET) // Vérification que le socket est bien winsock_initialisé
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

SOCKADDR_IN setup_send_addr(const char *hostname)
{
	SOCKADDR_IN to = {0};
	//const char *hostname = "192.168.1.73";
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

int send_pokeheader(SOCKET socket, const SOCKADDR *dest, int dlen, pokeheader *header)
{
	char *buff;

	buff = serialize_header(header);
	int nb = sendto(socket,buff, 6, 0, dest, dlen); // Impossible d'utiliser sizeof, puisque buff est un pointeur, donc sizeof retourne la taille du pointeur
	return 0;
}



void winsock_init()
{
	#ifdef WIN32
		WSADATA wsa;
		int err = WSAStartup(MAKEWORD(2, 2), &wsa);
		if(err < 0)
		{
			puts("WSAStartup failed !");
			exit(EXIT_FAILURE);
		}
	#endif
	}

void winsock_end()
{
	#ifdef WIN32
		WSACleanup();
	#endif
}