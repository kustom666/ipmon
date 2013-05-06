#ifndef DEF_POKEHEADER
#define DEF_POKEHEADER

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utility.h"

typedef struct pokeheader{
	char type[4];
	uint8_t id;
	uint8_t data_size;
}pokeheader;


pokeheader unserialize_header(char *instream);
pokeheader poke_headeralloc(char *bufftype, uint8_t id, uint8_t data_length);
void serialize_header(pokeheader header);

#endif