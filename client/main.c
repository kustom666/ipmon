#include <string.h>
#include "chandler.h"
#include "../Core/network.h"
#include "../Core/pokeheader.h"
#include "../Core/pokepacket.h"
#include "../Core/generator.h"
#include "../Core/pokemon.h"

int main(int arcg, char **argv)
{	
	winsock_init();
	SOCKADDR_IN listen = setup_addr();
	

	char nom[128];
	SOCKADDR_IN from = {0};
	int fromsize = sizeof from;

	printf("Connection au serveur %s\n", argv[1]);
	SOCKET sock = setup_socket();
	SOCKADDR_IN to = setup_send_addr(argv[1]);
	int to_size = sizeof(to);
	pokepacket pack;
	pokeheader head;
	pokemon cur_poke;
	
	if(bind(sock, (SOCKADDR *) &listen, sizeof listen) == SOCKET_ERROR)
	{
		perror("Impossible de bind le port");
		exit(1);
	}

	/*Mise en place d'un timeout*/
	int len_pack, selecteur = 0;
	struct timeval timeout;      
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;

    if (setsockopt (sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,
                sizeof(timeout)) < 0)
        error("setsockopt failed\n");

    if (setsockopt (sock, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout,
                sizeof(timeout)) < 0)
        error("setsockopt failed\n");


   	printf("Bienvenue, je suis le prof Ken. Prépare toi à vivre des aventures hallucinogènantes!\nMais d'abord, quel est ton nom?\n");
	scanf("%s", nom);

	do{

		printf("Salutation %s!\nQue veux tu faire?\n", nom);
		printf("1- Nouveau pokémon\n2- Duel la forêt\n8- Afficher les stats de ton pokémon\n9- Quitter\n");
		scanf("%d", &selecteur);
		char *recv_buffer = (char*)malloc(1024*sizeof(char));
		char *send_buffer = (char*)malloc(1024*sizeof(char));
		char *output = (char*)malloc(512*sizeof(char));
   		char *multiple = (char*)malloc(1024*sizeof(char));

		if(selecteur == 1)
		{
			len_pack = generate_packet(2, &pack, &head, TAG_NOUV, &output, nom);
			if(len_pack != 0)
			{
				sendto(sock, output, len_pack, 0, (SOCKADDR *)&to, sizeof(to));
				int n = recvfrom(sock, recv_buffer, 1024*sizeof(char), 0, (SOCKADDR *)&from, &fromsize);
				if ( n  < 0)
				{
					perror("Erreur de réception");
					exit(1);
				}
				unserialize_pokemon(recv_buffer, &cur_poke);
				printf("La forêt t'envoie un nouveau pokémon d'espèce : %d\n", cur_poke.spec);

			}
		}
		else if(selecteur == 2)
		{
			chandle_duel(sock, (SOCKADDR *)&to, sizeof(to), nom, cur_poke);
		}
		else if(selecteur == 8)
		{
			poke_dump(cur_poke);
		}
		/*memset(recv_buffer,0,1024);
		memset(multiple, 0, 1024);
		memset(output, 0, 512);*/
		memset(multiple, 0, 1024);
		free(output);
		//free(multiple);
		free(recv_buffer);
		free(send_buffer);
	}while(selecteur!=9);


	winsock_end();
	return 0;
}