#include "generator.h"
int generate_packet(int nbpack, pokepacket *pack, pokeheader *head, char *tag, char **output)
{
	int pck_size = 0;
	if(tag == TAG_DONE)
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
	}
	else
	{
		printf("Mauvais tag\n");
	}
}