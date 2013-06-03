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

	if(argv[1] == NULL)
	{
		printf("Vous devez spécifier une addresse serveur en lançant le programme.\nExemple : ./client bytechain.com\n");
		exit(1);
	}
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
	pokemon pokeceinture[6];
	int nbpokemon = 0;
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

	char *out = (char*)malloc(1024*sizeof(char));
	len_pack = generate_packet(2, &pack, &head, TAG_NOUV, &out, nom);
	if(len_pack != 0)
	{
		char *recv = (char*)malloc(1024*sizeof(char));
		sendto(sock, out, len_pack, 0, (SOCKADDR *)&to, sizeof(to));
		int n = recvfrom(sock, recv, 1024*sizeof(char), 0, (SOCKADDR *)&from, &fromsize);
		if ( n  < 0)
		{
			perror("Erreur de réception");
			exit(1);
		}
		unserialize_pokemon(recv, &cur_poke);
		printf("La forêt t'envoie un nouveau pokémon d'espèce : %d\n", cur_poke.spec);
		free(recv);

	}
	free(out);

	do{

		printf("Salutation %s!\nQue veux tu faire?\n", nom);
		printf("1- Duel la forêt\n2- Afficher la pokéceinture \n3- Afficher les stats de ton pokémon actuel\n4- Choisir le pokémon courant\n5- Jeter un pokémon\n6- Démarrer un serveur de troc\n7- Lancer un troc (serveur démarré chez l'autre)\n9- Quitter\n");
		scanf("%d", &selecteur);
		char *recv_buffer = (char*)malloc(1024*sizeof(char));
		char *send_buffer = (char*)malloc(1024*sizeof(char));
		char *output = (char*)malloc(512*sizeof(char));
   		char *multiple = (char*)malloc(1024*sizeof(char));
   		pokemon cap_poke;

		if(selecteur == 1)
		{
			int issue_finale = chandle_duel(sock, (SOCKADDR *)&to, sizeof(to), nom, cur_poke, &cap_poke);
			if(nbpokemon < 6 && issue_finale == 0)
			{

				cur_poke.xp += 5;
				printf("Vous avez gagné le combat avec le pokémon sauvage!\nVous remportez 5 xp\n");
				printf("Vous avez capturé le pokémon\nIl est ajouté à votre pokéceinture\n");
				pokeceinture[nbpokemon] = cap_poke;
				nbpokemon++;
			}
			else if(issue_finale == 1)
			{
				printf("Vous n'avez pas pu capturer le pokémon\n");
			}
			else
			{
				printf("Votre pokéceinture est pleine. Le pokémon sauvage ne peut pas être capturé!\n");
			}
		}
		else if(selecteur == 2)
		{
			printf("Contenu de votre pokéceinture : \n");
			for(int x = 0; x < nbpokemon; x++){
				char *bufferpoke = (char*)malloc(512*sizeof(char));
				serialize_pokemon(pokeceinture[x], &bufferpoke);
				printf("Pokéball n°%d : %s\n", x+1, bufferpoke);
				free(bufferpoke);
			}
		}
		else if(selecteur == 3)
		{
			poke_dump(cur_poke);
		}
		else if(selecteur == 4)
		{
			int ech;
			printf("Echanger le pokémon courant contre quel pokémon?\n");
			printf("Contenu de votre pokéceinture : \n");
			for(int x = 0; x < nbpokemon; x++){
				char *bufferpoke = (char*)malloc(512*sizeof(char));
				serialize_pokemon(pokeceinture[x], &bufferpoke);
				printf("Pokéball n°%d : %s\n", x+1, bufferpoke);
				free(bufferpoke);
			}
			scanf("%d", &ech);
			if(ech >= 1 && ech <= 6)
			{
				printf("Echange du pokémon courant contre le pokémon n°%d\n", ech);
				pokemon pokebuff = cur_poke;
				cur_poke = pokeceinture[ech-1];
				pokeceinture[ech-1] = pokebuff;
			}
			else
			{
				printf("Mauvais numéro de pokémon!\n");
			}

		}
		else if(selecteur == 5)
		{
			int nbsup;
			printf("Contenu de votre pokéceinture : \n");
			for(int x = 0; x < nbpokemon; x++){
				char *bufferpoke = (char*)malloc(512*sizeof(char));
				serialize_pokemon(pokeceinture[x], &bufferpoke);
				printf("Pokéball n°%d : %s\n", x+1, bufferpoke);
				free(bufferpoke);
			}

			printf("Supprimer le pokémon n°:\n");
			scanf("%d", &nbsup);
			if(nbsup >= 1 && nbsup < 6)
			{
				for(int j = nbsup-1; j < 5; j++){
					pokeceinture[j] = pokeceinture[j+1];
				}
				pokemon pokenull;
				pokeceinture[5] = pokenull;
				nbpokemon --;
			}
			else if(nbsup == 6)
			{
				pokemon pokenull;
				pokeceinture[5] = pokenull;
				nbpokemon --;
			}
			else
			{
				printf("Mauvais numéro de pokéball");
			}

		}
		else if(selecteur ==6)
		{
			printf("Quel pokémon voulez vous échanger?\n");
			int nbech;
			printf("Contenu de votre pokéceinture : \n");
			for(int x = 0; x < nbpokemon; x++){
				char *bufferpoke = (char*)malloc(512*sizeof(char));
				serialize_pokemon(pokeceinture[x], &bufferpoke);
				printf("Pokéball n°%d : %s\n", x+1, bufferpoke);
				free(bufferpoke);
			}
			scanf("%d", &nbech);
			if(nbech >= 1 && nbech < 6)
			{
				pokemon poke_env = pokeceinture[nbech];
				int ret = chandle_troc_toserver(sock, &poke_env);
				if(ret == 0)
				{
					pokeceinture[nbech] = poke_env;
				}
			}
		}
		else if(selecteur == 7)
		{
			printf("Quel pokémon voulez vous échanger?\n");
			int nbech;
			printf("Contenu de votre pokéceinture : \n");
			for(int x = 0; x < nbpokemon; x++){
				char *bufferpoke = (char*)malloc(512*sizeof(char));
				serialize_pokemon(pokeceinture[x], &bufferpoke);
				printf("Pokéball n°%d : %s\n", x+1, bufferpoke);
				free(bufferpoke);
			}
			scanf("%d", &nbech);
			if(nbech >= 1 && nbech < 6)
			{
				char ip_dresseur[128];
				printf("Quel est l'ip du dresseur avec qui échanger le pokémon?\n");
				scanf("%s", ip_dresseur);
				pokemon poke_env = pokeceinture[nbech];
				int ret = chandle_troc(sock, ip_dresseur, &poke_env);
				if(ret == 0)
				{
					pokeceinture[nbech] = poke_env;
				}
			}
		}

		/*memset(recv_buffer,0,1024);
		memset(multiple, 0, 1024);
		memset(output, 0, 512);*/
		memset(multiple, 0, 1024);
		free(output);
		free(multiple);
		free(recv_buffer);
		free(send_buffer);
	}while(selecteur!=9);


	winsock_end();
	return 0;
}