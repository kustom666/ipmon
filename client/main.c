#include <string.h>
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
			sprintf(multiple, "%s-%d", nom, cur_poke.speed);
			len_pack = generate_packet(2, &pack, &head, TAG_DINI, &output, multiple);
			printf("Envoi du packet : %s\n", output);
			if(len_pack != 0)
			{
				sendto(sock, output, len_pack, 0, (SOCKADDR *)&to, sizeof(to));
				int n = recvfrom(sock, recv_buffer, 1024*sizeof(char), 0, (SOCKADDR *)&from, &fromsize);
				if ( n  < 0)
				{
					perror("Erreur de réception");
					exit(1);
				}

				printf("La forêt réponds : %s\n", recv_buffer);

				memset(multiple, 0,1024);
				strcpy(multiple, recv_buffer);

				char *buffstrtok = strtok(multiple, "-");
				char *pseudo_ennemi = buffstrtok;
				buffstrtok = strtok(NULL, "-"); 
				char *vit = buffstrtok;
				int vit_e = atoi(vit);
				int combat = 0;


				if(cur_poke.speed < vit_e)
				{
					printf("Un pokémon sauage apparaît\nIl à l'initiative\n");
					do{
						pokemon adv_poke;
						memset(multiple, 0, 1024);
						recvfrom(sock, recv_buffer, 1024*sizeof(char), 0, (SOCKADDR *)&from, &fromsize);	
						printf("\nLe pokémon de la forêt est : %s\n", recv_buffer);
						fflush(stdout);
						strcpy(multiple, recv_buffer);
						unserialize_pokemon(multiple, &adv_poke);
						int adv_atkp = (adv_poke.a_f +adv_poke.a_eau +adv_poke.a_ele +adv_poke.a_pl +adv_poke.a_air +adv_poke.a_pi)/6;
						int for_atkp = (cur_poke.a_f +cur_poke.a_eau +cur_poke.a_ele +cur_poke.a_pl +cur_poke.a_air +cur_poke.a_pi)/6;
						
						int for_defp = (cur_poke.d_f +cur_poke.d_eau +cur_poke.d_ele +cur_poke.d_pl+cur_poke.d_air +cur_poke.d_pi)/6;
						int adv_defp = (adv_poke.d_f +adv_poke.d_eau +adv_poke.d_ele +adv_poke.d_pl+adv_poke.d_air +adv_poke.d_pi)/6;

						int delta_adv = for_atkp - adv_defp;
						int delta_for = adv_atkp - for_defp;
						int adv_hp_loss, for_hp_loss;

						adv_hp_loss = delta_adv > 0 ? delta_adv : 0;
						for_hp_loss = delta_for > 0 ? delta_for : 0;

						printf("Le pokémon ennemi à attaqué avec une force de %d Il perd %d hp\nNotre pokémon à riposté avec une force de : %d Il perd %d hp\n", adv_atkp, adv_hp_loss, for_atkp, for_hp_loss);

						cur_poke.hp -= for_hp_loss;

						//Notre pokémon à défendu, on envoie la réponse

						char *new_issue = cur_poke.hp >= 0 ? "OK" : "KO";

						char * dataresp = (char*)malloc(64*sizeof(char));
						sprintf(dataresp, "%d:A:%s", adv_hp_loss, new_issue);

						printf("on a créé l'issue : %s\n", dataresp);
						fflush(stdout);
						memset(recv_buffer,0,1024);
						memset(send_buffer, 0,1024);
						len_pack = generate_packet(12, &pack, &head, TAG_ISSU, &send_buffer, dataresp);
						sendto(sock, send_buffer, len_pack, 0, (SOCKADDR *)&to, sizeof(to));

						printf("On envoie l'issue : %s\n", send_buffer);
						fflush(stdout);
						//On a envoyé l'issue, on prépare notre attaque 
						char *serialized_poke = (char*)malloc(512*sizeof(char));
						serialize_pokemon(cur_poke, &serialized_poke);
						len_pack = generate_packet(12, &pack, &head, TAG_ATCK, &send_buffer, serialized_poke);
						sendto(sock, send_buffer, len_pack,0, (SOCKADDR *)&to, sizeof(to));

						//On a attaqué, attendons l'issue
						memset(multiple, 0, 1024);
						recvfrom(sock, recv_buffer, 1024*sizeof(char), 0, (SOCKADDR *)&from, &fromsize);
						strcpy(multiple, recv_buffer);

						char *hp_loss = strtok(multiple, ":");
						char *victor = strtok(NULL, ":");
						char *issue = strtok(NULL, ":");

						if(strcmp(issue, "KO") == 0)
						{
							printf("Le pokémon ennemi est KO! Lol NUB! Vous avez gagné\n");
							combat = 1;
						}
						else if(strcmp(issue, "OK") == 0)
						{	
							cur_poke.hp -= atoi(hp_loss);
						}
						free(dataresp);
						free(serialized_poke);
					}while(combat != 1);
					
				}
				else
				{
					printf("Votre pokémon à l'initiative!\n");
				}

			}
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