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
	printf("Demande de duel de la part de %s\nC'est l'heure du dudududududuuuuuel\n", recv_ip);
	pokepacket pack;
	pokeheader head;

/*	char *buffstrtok = strtok(buffer, "-");

	char *pseudo_ennemi = buffstrtok;

	buffstrtok = strtok(NULL, "-"); 
	char *vit = buffstrtok;*/

	char *delim = strchr(buffer, '-');

	char *pseudo_ennemi, *vit;
	int offset = delim-buffer+1;
	printf("offset delim : %d\n", offset);
	//strncpy(pseudo_ennemi, buffer, delim-buffer);

	int vit_e = atoi(delim+1);

	printf("L'ennemi est : %s\nVitesse : %d\n",pseudo_ennemi, vit_e);

	char *buff_send = (char * )malloc(1024*sizeof(char));
	memset(buff_send, 0,1024);
	char *prepsend = (char*)malloc(1024*sizeof(char));
	memset(prepsend, 0, 1024);
	generate_pokemon(prepsend);

	pokemon rand_poke;
	unserialize_pokemon(prepsend, &rand_poke);

	int pck_size = generate_packet(12, &pack, &head, TAG_DACK, &buff_send, prepsend);
	if(pck_size == 0)
	{
		printf("Erreur lors de la création du packet\n");
	}
	else
	{
		//printf("Envoi de : %s\n\nTaille : %d", buff_send, pck_size);
		sendto(sock , buff_send, pck_size, 0, dest_addr, dest_len); 
	}

	if(rand_poke.speed > vit_e)
	{
		printf("La forêt à l'initiative!");
	}
	else
	{
		printf("%s à l'initiative!", pseudo_ennemi);
	}
	free(buff_send);
	free(prepsend);
}