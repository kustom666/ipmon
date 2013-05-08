#include "pokeheader.h"

pokeheader unserialize_header(char *instream)
{
	char buff[32];
	range_strcpy(buff, instream, 0,3);
	uint32_t bufft = unserialize_uint32(buff);
	range_strcpy(buff, instream, 4,7);
	uint32_t bufff = unserialize_uint32(buff);
	uint8_t buffid = instream[8];
	uint8_t bufftaille = instream[9];

	pokeheader buff_header = {bufft, bufff, buffid, bufftaille};
	return buff_header;

}

unsigned char * serialize_header(pokeheader *header, unsigned char *instream)
{
	unsigned char *buff8 = (unsigned char *) malloc(sizeof(unsigned char));
	unsigned char *buff8b = (unsigned char *) malloc(sizeof(unsigned char));
	unsigned char *buff32 = (unsigned char *) malloc(4*sizeof(unsigned char));
	unsigned char *buff32b = (unsigned char *) malloc(4*sizeof(unsigned char));
	buff32 = serialize_uint32(buff32, header->type);
	buff32b = serialize_uint32(buff32b, header->from);
	buff8 = serialize_uint8(buff8, header->id);
	buff8b = serialize_uint8(buff8b, header->data_size);

	unsigned char *outstream = (unsigned char *) malloc(10 * sizeof(unsigned char));
	strcpy(outstream, buff32);
	strcat(outstream, buff32b);
	strcat(outstream, buff8);
	strcat(outstream, buff8b);

	free(buff8);
	free(buff8b);
	free(buff32);
	free(buff32b);

	return outstream;
}


pokeheader poke_headeralloc(char *packet_type, uint8_t id, uint8_t data_length)
{
	pokeheader buff_header = (pokeheader) { packet_type, id, data_length};
	return buff_header;
}

