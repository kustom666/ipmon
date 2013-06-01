#ifndef DEF_POKEMON
#define DEF_POKEMON
#include <stdio.h>  
#include <stdlib.h>
#include <string.h>

typedef struct pokemon{
	int id;
	int spec;
	int hp;
	int hpmax;
	int xp;
	int speed;
	int a_f;
	int a_eau;
	int a_ele;
	int a_pl;
	int a_air;
	int a_pi;
	int d_f;
	int d_eau;
	int d_ele;
	int d_pl;
	int d_air;
	int d_pi;
}pokemon;

void poke_dump(pokemon pkm);
void unserialize_pokemon(char *input, pokemon *pkm);
void serialize_pokemon(pokemon pkm, char **output);
#endif