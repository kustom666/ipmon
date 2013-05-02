#include "pokeheader.h"

pokeheader serialize_header(char *instream)
{

	char bufftype[4];
	range_strcpy(bufftype, instream, 0,3);
	printf("Header type serialisé : %s \n", bufftype);
	uint8_t buffid =  instream[4];
	uint8_t buffdatal = instream[5];

	pokeheader buff_header = pokealloc(bufftype, buffid, buffdatal);
	return buff_header;

}

pokeheader pokealloc(char *packet_type, uint8_t id, uint8_t data_length)
{
	pokeheader buff_header = (pokeheader) { packet_type, id, data_length};
	return buff_header;
}