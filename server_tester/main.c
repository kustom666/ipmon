#include <string.h>
#include "../Core/network.h"
#include "../Core/pokeheader.h"
#include "../Core/pokepacket.h"
#include "../Core/generator.h"

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

	int selecteur = 0;
	do{
		printf("Tester quel paquet?\n1-Nouveau\n2-DuelInit\n9-Stop\n");
		scanf("%d", &selecteur);
		memset(recv_buffer, 0, 1024);
		if(selecteur == 1)
		{
			printf("\nVeuillez entrer votre pseudo : ");
			char *data = (char *) malloc(64*sizeof(char));
			scanf("%s", data);
			printf("Len data: %d\n", strlen(data));
			uint32_t test = unserialize_uint32(( char *)TAG_NOUV);
			pokeheader header = {test, 1, strlen(data)};

			char buffer[10], *ptr;
			ptr = serialize_header(&header);

			//char * append = forge_packet(ptr, data, 6, strlen(data));
			char *append = (char*)malloc(strlen(data)+7*sizeof(char));
			forge_packet(ptr, data, 6, strlen(data)+1, append);
			printf("Envoi de : %s - len : %d\n", append, strlen(append));
			//send_pokepacket();
			sendto(sock ,append, strlen(append), 0, (SOCKADDR *)&to, to_size); 
			int n = recvfrom(sock, recv_buffer, 1024*sizeof(char), 0, (SOCKADDR *)&from, &fromsize);
			if ( n  < 0)
			{
				perror("Erreur de réception");
				exit(1);
			}
			free(append);
		}
		else if(selecteur == 2)
		{

			printf("\nVeuillez entrer votre pseudo : ");
			char *data = (char *) malloc(64*sizeof(char));
			char *sendbuffer = (char *) malloc(1024 * sizeof(char));
			scanf("%s", data);
			printf("Len data: %d\n", strlen(data));
			pokepacket pack;
			pokeheader head;
			int pck_size = generate_packet(5, &pack, &head, TAG_DINI, &sendbuffer, data);
			if(pck_size == 0)
			{
				printf("Erreur lors de la création du packet\n");
			}
			else
			{
				printf("Envoi de : %s\n", sendbuffer);
				sendto(sock , sendbuffer, pck_size, 0, (SOCKADDR *)&to, sizeof(to)); 
			}
			int n = recvfrom(sock, recv_buffer, 1024*sizeof(char), 0, (SOCKADDR *)&from, &fromsize);
			if ( n  < 0)
			{
				perror("Erreur de réception");
				exit(1);
			}
			free(data);
			free(sendbuffer);

		}
		else if(selecteur == 9)
		{
			printf("Sortie!\n");
		}
		else
		{
			printf("Mauvaise sélection\n");
		}

		pokepacket recv_pack = unserialize_pokepacket(recv_buffer);
		printf("Type : %d\nID : %d\nTaille donnees : %d \nDonnes : %s\n",recv_pack.header.type,recv_pack.header.id, recv_pack.header.data_size, recv_pack.data );
		//printf("Réponse : %s ", recv_buffer);

	}while(selecteur!=9);

	winsock_end();
	return 0;
}