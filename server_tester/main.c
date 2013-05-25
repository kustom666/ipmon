#include <string.h>
#include "../Core/network.h"
#include "../Core/pokeheader.h"
#include "../Core/pokepacket.h"

int main(int arcg, char **argv)
{	
	winsock_init();
	char addr[64];
	printf("Se connecter à quel serveur?\n");
	scanf("%s", addr);
	printf("Connection au serveur %s\n", addr);
	SOCKET sock = setup_socket();
	SOCKADDR_IN to = setup_send_addr(addr);
	int to_size = sizeof(to);
	int status;
	do{
		printf("Veuillez entrer votre pseudo : ");
		char *data = (char *) malloc(64*sizeof(char));
		scanf("%s", data);
		uint32_t test = unserialize_uint32(( char *)TAG_LOGI);
		pokeheader header = {test, 1, strlen(data)};

		char buffer[10], *ptr;
		ptr = serialize_header(&header);

		char * append = forge_packet(ptr, data, 6, strlen(data));

		//send_pokepacket();
		sendto(sock ,append, 6+strlen(data), 0, (SOCKADDR *)&to, to_size); 
	}while(1==1);

	winsock_end();

	return 0;
}