#include "pokeheader.h"

pokeheader unserialize_header(char *instream)
{

	char bufftype[4];
	range_strcpy(bufftype, instream, 0,3);
	printf("Header type serialisé : %s \n", bufftype);
	uint8_t buffid =  instream[4];
	uint8_t buffdatal = instream[5];

	pokeheader buff_header = poke_headeralloc(bufftype, buffid, buffdatal);
	return buff_header;

}

char serialize_header(pokeheader header, char *serialized)
{
	range_strcpy(serialized, header.type, 0, 3);
	serialized[4] = header.id;
	serialized[5] = header.data_size;
}

pokeheader poke_headeralloc(char *packet_type, uint8_t id, uint8_t data_length)
{
	pokeheader buff_header = (pokeheader) { packet_type, id, data_length};
	return buff_header;
}
