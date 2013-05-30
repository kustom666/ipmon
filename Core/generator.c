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

int generate_packet(int nbpack, pokepacket *pack, pokeheader *head, char *tag, char **output)
{
	int pck_size = 0;
	if(tag == TAG_DONE)
	{
		char * data = (char*)malloc(1024*sizeof(char));
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
		printf("Generated output : %s\n", *output);
		free(data);
		return 6 + lendata;
	}
	/*else if (tag == TAG_ATCK)
	{
		char *data = "42:25:34:53:10:5:(12:0:9:0:0:1):(19:0:3:20:0:30)";
		uint32_t donne = unserialize_uint32(( char *)TAG_DONE);
		head->type = donne;
		head->id = nbpack+1;
		head->data_size = strlen(data);
		pack->header = *head;
		pack->data = data;

		char *buff_head = serialize_header(head);

		*output = forge_packet(buff_head, data, 6, strlen(data));

		printf("Generated output : %s\n", *output);

		return 6 + strlen(data);
	}*/
	else
	{
		printf("Mauvais tag\n");
	}
}