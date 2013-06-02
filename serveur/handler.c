#include "handler.h"

void handle_nouveau(SOCKET sock, char *buffer, const struct sockaddr *dest_addr, int dest_len, char *recv_ip)
{

	printf("Nouveau login depuis : %s\n", recv_ip);
	printf("Recu : %s\n", buffer);
	pokepacket pack;
	pokeheader head;

	char *buff_send = (char * ) malloc(1024*sizeof(char));
	int pck_size = generate_packet(12, &pack, &head, TAG_DONE, &buff_send, NULL);

	if(pck_size == 0)
	{
		printf("Erreur lors de la création du packet\n");
	}
	else
	{
		printf("Envoi de : %s\n", buff_send);
		sendto(sock , buff_send, pck_size, 0, dest_addr, dest_len); 
	}
	free(buff_send);
}

void handle_duel(SOCKET sock, char *buffer, const struct sockaddr *dest_addr, int dest_len, char *recv_ip)
{
	//Variables nécessaires
	char *storage = (char*)malloc(512*sizeof(char));
	char *buff_send = (char * )malloc(1024*sizeof(char));
	char *prepsend = (char*)malloc(1024*sizeof(char));
	SOCKADDR_IN from = {0};
	int fromsize = sizeof from;
	pokepacket pack;
	pokeheader head;
	pokemon rand_poke;

	memset(buff_send, 0,1024);
	memset(prepsend, 0, 1024);

	printf("Demande de duel de la part de %s\nC'est l'heure du dudududududuuuuuel\n", recv_ip);

	strcpy(storage, buffer);
    char *buffstrtok = strtok(storage, "-");
	char *pseudo_ennemi = buffstrtok;
	buffstrtok = strtok(NULL, "-"); 
	char *vit = buffstrtok;
	int vit_e = atoi(vit);

	printf("L'ennemi est : %s\nVitesse : %d\n",pseudo_ennemi, vit_e);

	generate_pokemon(prepsend);
	unserialize_pokemon(prepsend, &rand_poke);

	int pck_size = generate_packet(12, &pack, &head, TAG_DACK, &buff_send, prepsend);
	if(pck_size == 0)
	{
		printf("Erreur lors de la création du packet\n");
	}
	else
	{
		sendto(sock , buff_send, pck_size, 0, dest_addr, dest_len); 
		if(rand_poke.speed > vit_e)
		{
			printf("La forêt à l'initiative!\nAttaque!\n");
			int cont = 0;
			while(cont==0)
			{
				char *serialized_poke = (char*)malloc(512*sizeof(char));
				char *resp = (char*)malloc(512*sizeof(char));
				
				//On attaque une première fois
				serialize_pokemon(rand_poke, &serialized_poke);
				pck_size = generate_packet(12, &pack, &head, TAG_ATCK, &buff_send, serialized_poke);
				//On reçoit l'issue
				pokemon adv_poke;
				sendto(sock, buff_send, pck_size, 0, dest_addr, dest_len);
				memset(buff_send, 0,1024);
				recvfrom(sock, buff_send, 1024*sizeof(char), 0, (SOCKADDR *)&from, &fromsize);
				printf("On a reçu l'issue : \n%s\n", buff_send);
				strcpy(resp, buff_send);
				char *hp_loss = strtok(resp, ":");
				char *victor = strtok(NULL, ":");
				char *issue = strtok(NULL, ":");

				if(strcmp(issue, "KO") == 0) //Si l'issue est un ko ennemi
				{
					printf("Le pokémon ennemi est KO! Lol NUB!\n");
					cont = 1;
				}
				else if(strcmp(issue, "OK") == 0) //Sinon le duel continue
				{
					memset(resp,0,512);
					//On retire des HP à notre pokémon
					printf("Le pokémon ennemi à survécu.\nNotre pokémon à perdu %s hp\n", hp_loss);
					fflush(stdout);
					rand_poke.hp -= atoi(hp_loss);

					//On reçoit l'attaque adverse
					recvfrom(sock, buff_send, 1024*sizeof(char), 0, (SOCKADDR *)&from, &fromsize);
					strcpy(resp, buff_send);

					//calcul des dégats (moyenne arithmétique des composantes)
					unserialize_pokemon(resp, &adv_poke);
					int adv_atkp = (adv_poke.a_f +adv_poke.a_eau +adv_poke.a_ele +adv_poke.a_pl +adv_poke.a_air +adv_poke.a_pi)/6;
					int for_atkp = (rand_poke.a_f +rand_poke.a_eau +rand_poke.a_ele +rand_poke.a_pl +rand_poke.a_air +rand_poke.a_pi)/6;
					
					int for_defp = (rand_poke.d_f +rand_poke.d_eau +rand_poke.d_ele +rand_poke.d_pl+rand_poke.d_air +rand_poke.d_pi)/6;
					int adv_defp = (adv_poke.d_f +adv_poke.d_eau +adv_poke.d_ele +adv_poke.d_pl+adv_poke.d_air +adv_poke.d_pi)/6;

					int delta_adv = for_atkp - adv_defp;
					int delta_for = adv_atkp - for_defp;
					int adv_hp_loss, for_hp_loss;

					//On perds au moins 1HP à chaque fois, sous peine de boucle infinie
					adv_hp_loss = delta_adv > 0 ? delta_adv : 1;
					for_hp_loss = delta_for > 0 ? delta_for : 1;

					printf("Le pokémon ennemi à attaqué avec une force de %d Il perd %d hp\nNotre pokémon à riposté avec une force de : %d Il perd %d hp\n", adv_atkp, adv_hp_loss, for_atkp, for_hp_loss);
					
					rand_poke.hp -= for_hp_loss;

					char *new_issue = rand_poke.hp > 0 ? "OK" : "KO";

					char * dataresp = (char*)malloc(64*sizeof(char));
					sprintf(dataresp, "%d:A:%s", adv_hp_loss, new_issue);

					printf("Issue de l'attaque : %s\n", dataresp);
					pck_size = generate_packet(12, &pack, &head, TAG_ISSU, &buff_send, dataresp);
					sendto(sock, buff_send, pck_size, 0, dest_addr, dest_len);

					if(strcmp(new_issue, "KO") == 0)
					{
						printf("Le pokémon sauvage est KO!\n");
						cont = 1;
					}
				}
				else
				{
					fflush(stdout);
					printf("Mauvais packet\nReçu : %s\nHP loss : %s - Victor : %s - Issue : %s \n",buff_send,hp_loss,victor, issue);
					fflush(stdout);
					cont = 1;
				}
				memset(buff_send,0,1024);
				free(serialized_poke);
				free(resp); 
			}
			printf("Le combat est terminé!\n");
		}
		else
		{
			printf("%s à l'initiative!\n", pseudo_ennemi);
		}
	}

	
	free(buff_send);
	free(prepsend);
	free(storage);
}