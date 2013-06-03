#ifndef DEF_CHANDLER
#define DEF_CHANDLER

#include <string.h>
#include "../Core/network.h"
#include "../Core/pokeheader.h"
#include "../Core/pokepacket.h"
#include "../Core/utility.h"
#include "../Core/generator.h"
#include "../Core/pokemon.h"

int chandle_duel(SOCKET sock, const struct sockaddr *dest_addr, int dest_len, char *nom, pokemon cur_poke, pokemon *rec_poke);
int chandle_troc(SOCKET sock, char *ip_dresseur, pokemon *poke_env);
int chandle_troc_toserver(SOCKET sock, pokemon *poke_env);


#endif