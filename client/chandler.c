#include "chandler.h"

void chandle_duel(SOCKET sock, const struct sockaddr *dest_addr, int dest_len, char *nom, pokemon cur_poke, pokemon *rec_poke)
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
			printf("Un pokémon sauvage apparaît\nIl à l'initiative\n");
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

				cur_poke.hp -= for_hp_loss;
				printf("Le pokémon ennemi à attaqué avec une force de %d Il perd %d hp\nNotre pokémon à riposté avec une force de : %d Il perd %d hp et tombe à %d HP\n", adv_atkp, adv_hp_loss, for_atkp, for_hp_loss, cur_poke.hp);

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
					memset(recv_buffer,0,1024);
					printf("Le pokémon ennemi est KO! Lol NUB! Vous avez gagné\n");
					//On attends l'envoi du pokémon qu'on a battu
					recvfrom(sock, recv_buffer, 1024*sizeof(char), 0, (SOCKADDR *)&from, &fromsize);
					//On l'enregistre
					unserialize_pokemon(recv_buffer, rec_poke);
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
			printf("Votre pokémon à l'initiative!\nAttaque!");
			int cont = 0;
			while(cont==0)
			{
				char *serialized_poke = (char*)malloc(512*sizeof(char));
				char *resp = (char*)malloc(512*sizeof(char));
				
				//On attaque une première fois
				serialize_pokemon(cur_poke, &serialized_poke);
				len_pack = generate_packet(12, &pack, &head, TAG_ATCK, &send_buffer, serialized_poke);
				//On reçoit l'issue
				pokemon adv_poke;
				sendto(sock, send_buffer, len_pack, 0, dest_addr, dest_len);
				memset(send_buffer, 0,1024);
				recvfrom(sock, send_buffer, 1024*sizeof(char), 0, (SOCKADDR *)&from, &fromsize);
				printf("On a reçu l'issue : \n%s\n", send_buffer);
				strcpy(resp, send_buffer);
				char *hp_loss = strtok(resp, ":");
				char *victor = strtok(NULL, ":");
				char *issue = strtok(NULL, ":");

				if(strcmp(issue, "KO") == 0) //Si l'issue est un ko ennemi
				{
					printf("Le pokémon sauvage est KO! Lol NUB!\n");
					//On attends l'envoi du pokémon qu'on a battu
					recvfrom(sock, recv_buffer, 1024*sizeof(char), 0, (SOCKADDR *)&from, &fromsize);
					//On l'enregistre
					unserialize_pokemon(recv_buffer, rec_poke);
					cont = 1;
				}
				else if(strcmp(issue, "OK") == 0) //Sinon le duel continue
				{
					memset(resp,0,512);
					//On retire des HP à notre pokémon
					printf("Le pokémon sauvage à survécu.\nNotre pokémon à perdu %s hp\n", hp_loss);
					fflush(stdout);
					cur_poke.hp -= atoi(hp_loss);

					//On reçoit l'attaque adverse
					recvfrom(sock, send_buffer, 1024*sizeof(char), 0, (SOCKADDR *)&from, &fromsize);
					strcpy(resp, send_buffer);

					//calcul des dégats (moyenne arithmétique des composantes)
					unserialize_pokemon(resp, &adv_poke);
					int adv_atkp = (adv_poke.a_f +adv_poke.a_eau +adv_poke.a_ele +adv_poke.a_pl +adv_poke.a_air +adv_poke.a_pi)/6;
					int for_atkp = (cur_poke.a_f +cur_poke.a_eau +cur_poke.a_ele +cur_poke.a_pl +cur_poke.a_air +cur_poke.a_pi)/6;
					
					int for_defp = (cur_poke.d_f +cur_poke.d_eau +cur_poke.d_ele +cur_poke.d_pl+cur_poke.d_air +cur_poke.d_pi)/6;
					int adv_defp = (adv_poke.d_f +adv_poke.d_eau +adv_poke.d_ele +adv_poke.d_pl+adv_poke.d_air +adv_poke.d_pi)/6;

					int delta_adv = for_atkp - adv_defp;
					int delta_for = adv_atkp - for_defp;
					int adv_hp_loss, for_hp_loss;

					//On perds au moins 1HP à chaque fois, sous peine de boucle infinie
					adv_hp_loss = delta_adv > 0 ? delta_adv : 1;
					for_hp_loss = delta_for > 0 ? delta_for : 1;

					printf("Le pokémon sauvage à attaqué avec une force de %d Il perd %d hp\nNotre pokémon à riposté avec une force de : %d Il perd %d hp\n", adv_atkp, adv_hp_loss, for_atkp, for_hp_loss);
					
					cur_poke.hp -= for_hp_loss;

					char *new_issue = cur_poke.hp > 0 ? "OK" : "KO";

					char * dataresp = (char*)malloc(64*sizeof(char));
					sprintf(dataresp, "%d:A:%s", adv_hp_loss, new_issue);

					printf("Issue de l'attaque : %s\n", dataresp);
					len_pack = generate_packet(12, &pack, &head, TAG_ISSU, &send_buffer, dataresp);
					sendto(sock, send_buffer, len_pack, 0, dest_addr, dest_len);

					if(strcmp(new_issue, "KO") == 0)
					{
						printf("Notre pokémon est KO!\n");
						cont = 1;
					}
				}
				else
				{
					fflush(stdout);
					printf("Mauvais packet\nReçu : %s\nHP loss : %s - Victor : %s - Issue : %s \n",send_buffer,hp_loss,victor, issue);
					fflush(stdout);
					cont = 1;
				}
				memset(send_buffer,0,1024);
				free(serialized_poke);
				free(resp); 
			}
			printf("Le combat est terminé!\n");
		}
	}

	free(recv_buffer);
	free(send_buffer);
	free(multiple);
	free(output);
}