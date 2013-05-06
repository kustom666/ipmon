#ifndef DEF_POKEPACKET
#define DEF_POKEPACKET

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "pokeheader.h"

typedef struct pokepacket{
	pokeheader header;
	char *data;
}pokepacket;

#endif