#ifndef DEF_GENERATOR
#define DEF_GENERATOR

#include <string.h>
#include "network.h"
#include "pokeheader.h"
#include "pokepacket.h"
#include "utility.h"


int generate_packet(int nbpack, pokepacket *pack, pokeheader *head, char *tag, char **output);
char *generate_pokemon(char *output);


#endif
/* END DEF_GENERATOR */