#include "chandler.h"

void chandle_duel(SOCKET sock, const struct sockaddr *dest_addr, int dest_len, char *nom, pokemon cur_poke)
{
	pokepacket pack;
	pokeheader head;
	int len_pack;
	SOCKADDR_IN from = {0};
	int fromsize = sizeof from;
	char *recv_buffer = (char*)malloc(1024*sizeof(char));
	char *send_buffer = (char*)malloc(1024*sizeof(char));
	char *output = (char*)malloc(512*sizeof(char));
	char *multiple = (char*)malloc(1024*sizeof(char));

	sprintf(multiple, "%s-%d", nom, cur_poke.speed);
	len_pack = generate_packet(2, &pack, &head, TAG_DINI, &output, multiple);
	printf("Envoi du packet : %s\n", output);
	if(len_pack != 0)
	{
		sendto(sock, output, len_pack, 0, dest_addr, dest_len);
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

				adv_hp_loss = delta_adv > 0 ? delta_adv : 1;
				for_hp_loss = delta_for > 0 ? delta_for : 1;

				printf("Le pokémon ennemi à attaqué avec une force de %d Il perd %d hp\nNotre pokémon à riposté avec une force de : %d Il perd %d hp\n", adv_atkp, adv_hp_loss, for_atkp, for_hp_loss);

				cur_poke.hp -= for_hp_loss;

				//Notre pokémon à défendu, on envoie la réponse

				char *new_issue = cur_poke.hp > 0 ? "OK" : "KO";

				char * dataresp = (char*)malloc(64*sizeof(char));
				sprintf(dataresp, "%d:A:%s", adv_hp_loss, new_issue);

				printf("on a créé l'issue : %s\n", dataresp);
				fflush(stdout);
				memset(recv_buffer,0,1024);
				memset(send_buffer, 0,1024);
				len_pack = generate_packet(12, &pack, &head, TAG_ISSU, &send_buffer, dataresp);
				sendto(sock, send_buffer, len_pack, 0, dest_addr, dest_len);

				printf("On envoie l'issue : %s\n", send_buffer);
				fflush(stdout);

				if(strcmp(new_issue, "KO") == 0)
				{
					printf("Votre pokémon est tombé!\n");
					break;
				}
				//On a envoyé l'issue, on prépare notre attaque 
				char *serialized_poke = (char*)malloc(512*sizeof(char));
				serialize_pokemon(cur_poke, &serialized_poke);
				len_pack = generate_packet(12, &pack, &head, TAG_ATCK, &send_buffer, serialized_poke);
				sendto(sock, send_buffer, len_pack,0, dest_addr, dest_len);

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

	free(recv_buffer);
	free(send_buffer);
	free(multiple);
	free(output);
}