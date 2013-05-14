#include "pokeheader.h"

pokeheader unserialize_header(char *instream)
{
	char buff[32];
	range_strcpy(buff, instream, 0,3);
	uint32_t bufft = unserialize_uint32(buff);
	uint8_t buffid = instream[4];
	uint8_t bufftaille = instream[5];

	pokeheader buff_header = {bufft, buffid, bufftaille};
	return buff_header;

}

 char * serialize_header(pokeheader *header,  char *instream)
{
	 char *buff8 = ( char *) malloc(sizeof( char));
	 char *buff8b = ( char *) malloc(sizeof( char));
	 char *buff32 = ( char *) malloc(4*sizeof( char));
	buff32 = serialize_uint32(buff32, header->type);
	buff8 = serialize_uint8(buff8, header->id);
	buff8b = serialize_uint8(buff8b, header->data_size);

	 char *outstream = ( char *) malloc(10 * sizeof( char));
	strcpy(outstream, buff32);
	strcat(outstream, buff8);
	strcat(outstream, buff8b);

	free(buff8);
	free(buff8b);
	free(buff32);

	return outstream;
}


pokeheader poke_headeralloc(char *packet_type, uint8_t id, uint8_t data_length)
{
	pokeheader buff_header = (pokeheader) { packet_type, id, data_length};
	return buff_header;
}

