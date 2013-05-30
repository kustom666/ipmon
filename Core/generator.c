#include "generator.h"


char * generate_pokemon(char *output)
{
	char pkm_specs[18][10];
	srand(time(NULL));
	for(int i = 0; i< 18; i++){
		int buffrand = rand()%30;
		sprintf(pkm_specs[i], "%d", buffrand);
	}


	strcpy(output, pkm_specs[0]);
	for(int i = 1; i<18; i++){
		strcat(output, ":");
		strcat(output, pkm_specs[i]);
	}
	
	printf("Pokémon généré : %s\n", output);

}

int generate_packet(int nbpack, pokepacket *pack, pokeheader *head, char *tag, char **output, char *indata)
{
	int pck_size = 0;
	char * data = (char*)malloc(1024*sizeof(char));
	if(tag == TAG_DONE)
	{
		generate_pokemon(data);
		uint32_t donne = unserialize_uint32(( char *)TAG_DONE);
		head->type = donne;
		head->id = nbpack+1;
		head->data_size = strlen(data);
		pack->header = *head;
		pack->data = data;

		char *buff_head = serialize_header(head);
		forge_packet(buff_head, data, 6, strlen(data)+1, *output);
		int lendata = strlen(data);
		return 6 + lendata;
	}
	else if(tag == TAG_DACK)
	{
		
		uint32_t dack = unserialize_uint32(( char *)TAG_DACK);
		char *buffstrtok = strtok(indata, ":");
		int i = 0;
		while(i != 5)
		{
			buffstrtok = strtok(NULL, ":");
			i++;
		}
		printf("rapidité : %s\n", buffstrtok);

		char *aforet = "foret-";
		char *aforger = (char*) malloc(512*sizeof(char));
		strcpy(aforger, aforet);
		strcat(aforger, buffstrtok);

		printf("A forger : %s\n", aforger);

		head->type = dack;
		head->id = nbpack+1;
		head->data_size = strlen(aforger);
		pack->header = *head;
		pack->data = aforger;

		char *buff_head = serialize_header(head);
		forge_packet(buff_head, aforger, 6, strlen(aforger)+1, *output);
		int lenaforger = strlen(aforger);
		printf("Generated output : %s\n", *output);
		free(aforger);
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

		char *buff_head = serialize_header(head);
		forge_packet(buff_head, indata, 6, strlen(indata)+1, *output);
		int lenindata = strlen(indata);
		printf("Generated output : %s\n", *output);
		return 6 + lenindata;
	}
	else
	{
		printf("Mauvais tag\n");
	}
	free(data);
}