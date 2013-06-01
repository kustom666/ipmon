#include "generator.h"


char * generate_pokemon(char *output)
{
	char pkm_specs[18][10];
	srand(time(NULL));
	int buffhp;
	for(int i = 0; i< 18; i++){
		if(i == 4) // Le pokémon commence à 0 xp
		{
			sprintf(pkm_specs[i], "%d", 0);
		}
		else if (i==2)
		{
			buffhp = (rand()%30)+1;
			sprintf(pkm_specs[i], "%d", buffhp);
		}
		else if(i == 3)
		{
			sprintf(pkm_specs[i], "%d", buffhp);
		}
		else
		{
			int buffrand = (rand()%30) +1 ;
			sprintf(pkm_specs[i], "%d", buffrand);
		}
	}

	strcpy(output, pkm_specs[0]);
	for(int i = 1; i<18; i++){
		strcat(output, ":");
		strcat(output, pkm_specs[i]);
	}

	//Valeurs spéciales des stats du pokémon
	//pkm_specs[2] = pkm_specs[3]; // Le pokémon commence avec ses HP au max

	
	printf("Pokémon généré : %s\n", output);

}

int generate_packet(int nbpack, pokepacket *pack, pokeheader *head, char *tag, char **output, char *indata)
{
	int pck_size = 0;
	char * data = (char*)malloc(1024*sizeof(char));
	char *buff_head = (char*)malloc(32*sizeof(char));
	
	if(tag == TAG_DONE)
	{
		generate_pokemon(data);
		uint32_t donne = unserialize_uint32(( char *)TAG_DONE);
		head->type = donne;
		head->id = nbpack+1;
		head->data_size = strlen(data);
		pack->header = *head;
		pack->data = data;

		serialize_header(head, &buff_head);
		forge_packet(buff_head, data, 6, strlen(data)+1, *output);
		int lendata = strlen(data);
		free(data);
		free(buff_head);
		return 6 + lendata;
	}
	else if (tag == TAG_NOUV)
	{
		strcpy(data, indata);
		uint32_t nouv = unserialize_uint32((char*)TAG_NOUV);
		head->type = nouv;
		head->id = nbpack+1;
		head->data_size = strlen(data);
		pack->header = *head;
		pack->data = data;

		serialize_header(head, &buff_head);
		forge_packet(buff_head, data, 6, strlen(data)+1, *output);
		int lendata = strlen(data);
		free(data);
		free(buff_head);
		return 6+lendata;
	}
	else if(tag == TAG_DACK)
	{
		
		uint32_t dack = unserialize_uint32(( char *)TAG_DACK);

		pokemon poke_gen;
		unserialize_pokemon(indata, &poke_gen);

		char *buffvit = (char*)malloc(8*sizeof(char));

		sprintf(buffvit, "%d", poke_gen.speed);

		printf("rapidité : %s\n", buffvit);

		char *aforet = "foret-";
		char *aforger = (char*) malloc(512*sizeof(char));
		strcpy(aforger, aforet);
		strcat(aforger, buffvit);

		printf("A forger : %s\n", aforger);

		head->type = dack;
		head->id = nbpack+1;
		head->data_size = strlen(aforger);
		pack->header = *head;
		pack->data = aforger;

		serialize_header(head, &buff_head);
		forge_packet(buff_head, aforger, 6, strlen(aforger)+1, *output);
		int lenaforger = strlen(aforger);
		free(aforger);
		free(data);
		free(buff_head);
		free(buffvit);
		return 6 + lenaforger;
	}
	else if(tag == TAG_DINI)
	{
		uint32_t dini = unserialize_uint32(( char *)TAG_DINI);
		head->type = dini;
		head->id = nbpack+1;
		head->data_size = strlen(indata);
		pack->header = *head;
		pack->data = indata;

		serialize_header(head, &buff_head);
		forge_packet(buff_head, indata, 6, strlen(indata)+1, *output);
		int lenindata = strlen(indata);
		free(data);
		free(buff_head);
		return 6 + lenindata;
	}
	else if(tag == TAG_ATCK)
	{
		strcpy(data, indata);
		uint32_t atck = unserialize_uint32((char*)TAG_ATCK);
		head->type = atck;
		head->id = nbpack+1;
		head->data_size = strlen(data);
		pack->header = *head;
		pack->data = data;

		serialize_header(head, &buff_head);
		forge_packet(buff_head, data, 6, strlen(data)+1, *output);
		int lendata = strlen(data);
		free(data);
		free(buff_head);
		return 6+lendata;
	}
	else if(tag == TAG_ISSU)
	{
		strcpy(data, indata);
		uint32_t issu = unserialize_uint32((char*)TAG_ISSU);
		head->type = issu;
		head->id = nbpack+1;
		head->data_size = strlen(data);
		pack->header = *head;
		pack->data = data;

		serialize_header(head, &buff_head);
		forge_packet(buff_head, data, 6, strlen(data)+1, *output);
		int lendata = strlen(data);
		free(data);
		free(buff_head);
		return 6+lendata;
	}
	else
	{
		printf("Mauvais tag\n");
		free(data);
		free(buff_head);
		return 0;
	}

}