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

char * serialize_header(pokeheader *header)
{
	char buff8;
	char buff8b;
	char *buff32 = ( char *) malloc(4*sizeof( char));

	buff32 = serialize_uint32(buff32, header->type);
	buff8 = serialize_uint8(buff8, header->id);
	buff8b = serialize_uint8(buff8b, header->data_size);

	char *outstream = ( char *) malloc(6* sizeof( char));
	for(int i = 0; i< 4; i++){
		outstream[i] = buff32[i];
 	}

	outstream[4] = buff8;
	outstream[5] = buff8b;

	free(buff32);

	return outstream;
}
