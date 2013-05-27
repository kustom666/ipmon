#include <string.h>
#include "../Core/network.h"
#include "../Core/pokeheader.h"
#include "../Core/pokepacket.h"

int main(int arcg, char **argv)
{	
	winsock_init();
	SOCKADDR_IN listen = setup_addr();
	
	char recv_buffer[1024];
	SOCKADDR_IN from = {0};
	int fromsize = sizeof from;

	char addr[64];
	printf("Se connecter à quel serveur?\n");
	scanf("%s", addr);
	printf("Connection au serveur %s\n", addr);
	SOCKET sock = setup_socket();
	SOCKADDR_IN to = setup_send_addr(addr);
	int to_size = sizeof(to);
	
	if(bind(sock, (SOCKADDR *) &listen, sizeof listen) == SOCKET_ERROR)
	{
		perror("Impossible de bind le port");
		exit(1);
	}

	do{
		printf("Veuillez entrer votre pseudo : ");
		char *data = (char *) malloc(64*sizeof(char));
		scanf("%s", data);
		printf("Len data: %d\n", strlen(data));
		uint32_t test = unserialize_uint32(( char *)TAG_LOGI);
		pokeheader header = {test, 1, strlen(data)};

		char buffer[10], *ptr;
		ptr = serialize_header(&header);

		char * append = forge_packet(ptr, data, 6, strlen(data));
		printf("Envoi de : %s - len : %d\n", append, strlen(append));

		//send_pokepacket();
		sendto(sock ,append, strlen(append), 0, (SOCKADDR *)&to, to_size); 

		int n = recvfrom(sock, recv_buffer, 1024*sizeof(char), 0, (SOCKADDR *)&from, &fromsize);

		if ( n  < 0)
		{
			perror("Errreur de réception");
			exit(1);
		}

		printf("Réponse : %s ", recv_buffer);

	}while(1==1);

	winsock_end();

	return 0;
}