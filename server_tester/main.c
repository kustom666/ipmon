#include <string.h>
#include "../Core/network.h"
#include "../Core/pokeheader.h"
#include "../Core/pokepacket.h"

int main(int arcg, char **argv)
{	
	winsock_init();
	SOCKET sock = setup_socket();
	SOCKADDR_IN to = setup_send_addr();
	int to_size = sizeof(to);
	int status;

	int cont = 0;

	/*do
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
		char * instream = (char *)malloc(6*sizeof(char));
		
		printf("Sending : %s\n", instream);
		serialize_header(buff_head, &instream);
		status = sendto(sock, instream, sizeof(*instream), 0, (SOCKADDR *)&to, to_size);
		if(status < 0)
		{
			fprintf(stderr, "Erreur lors de l'winsock_initialisation de l'envoi du packet");
			exit(1);
		}

	}while(cont == 0);*/
	uint32_t test = unserialize_uint32(( char *)TAG_LOGI);
	pokeheader header = {test, 1, 5};
	printf("Pre Serialisation : %d, %d, %d\n", header.type, header.id, header.data_size);
	char *data = "PAUL";

	char buffer[10], *ptr, *ptb;
	ptr = serialize_header(&header);

	char * append = forge_packet(ptr, data, 6, 5);
	printf("appended %s -  %s \n", data, append);

	//send_pokepacket();
	sendto(sock ,append, 11, 0, (SOCKADDR *)&to, to_size); 

	winsock_end();

	return 0;
}