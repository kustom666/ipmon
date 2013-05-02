#include <string.h>
#include "../core/network.h"
#include "../core/pokeheader.h"
#include "../core/pokepacket.h"

int main(int arcg, char **argv)
{	

	SOCKET sock = setup_socket();
	SOCKADDR_IN to = setup_send_addr();
	int to_size = sizeof(to);
	int status;

	int cont = 0;

	do
	{
		char buffer_type[4];
		uint8_t buffer_id;
		uint8_t data_t;
		printf("Entrez le type de packet, suivi de l'id et de la taille des données\n");
		scanf("%s", &buffer_type);
		//printf("Buffer type : %s\n", buffer_type);
		scanf("%d", &buffer_id);
		scanf("%d", &data_t);
		pokeheader buff_head = poke_headeralloc(buffer_type, buffer_id, data_t);

		unsigned char buffer[sizeof(buff_head)];
		memcpy(&buffer, &buff_head, sizeof(buff_head));

		status = sendto(sock, buffer, sizeof(buffer), 0, (SOCKADDR *)&to, to_size);
		if(status < 0)
		{
			fprintf(stderr, "Erreur lors de l'initialisation de l'envoi du packet");
			exit(1);
		}

	}while(cont == 0);

	return 0;
}