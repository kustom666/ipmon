#ifndef DEF_GENERATOR
#define DEF_GENERATOR

#include <string.h>
#include "network.h"
#include "pokeheader.h"
#include "pokepacket.h"
#include "utility.h"
#include "pokemon.h"


int generate_packet(int nbpack, pokepacket *pack, pokeheader *head, char *tag, char **output, char *indata);
char *generate_pokemon(char *output);


#endif
/* END DEF_GENERATOR */