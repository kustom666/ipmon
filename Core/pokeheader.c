#include "pokeheader.h"

pokeheader unserialize_header(char *instream)
{

	char bufftype[4];
	range_strcpy(bufftype, instream, 0,3);
	uint8_t buffid =  instream[4];
	uint8_t buffdatal = instream[5];

	pokeheader buff_header = poke_headeralloc(bufftype, buffid, buffdatal);
	return buff_header;

}

void serialize_header(pokeheader header, char **instream)
{

	range_strcpy(*instream, header.type, 0, 3);

	instream[4] = header.id;
	instream[5] = header.data_size;

	printf("Serialisé : %s\n", instream);
}

pokeheader poke_headeralloc(char *packet_type, uint8_t id, uint8_t data_length)
{
	pokeheader buff_header = (pokeheader) { packet_type, id, data_length};
	return buff_header;
}

