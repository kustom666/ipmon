#ifndef DEF_CHANDLER
#define DEF_CHANDLER

#include <string.h>
#include "../Core/network.h"
#include "../Core/pokeheader.h"
#include "../Core/pokepacket.h"
#include "../Core/utility.h"
#include "../Core/generator.h"
#include "../Core/pokemon.h"

void chandle_duel(SOCKET sock, const struct sockaddr *dest_addr, int dest_len, char *nom, pokemon cur_poke, pokemon *rec_poke);

#endif