#include "pokemon.h"

void unserialize_pokemon(char *input, pokemon *pkm)
{
	int buffspecs[18];
	char *intermediate = (char *) malloc(512*sizeof(char));
	strcpy(intermediate, input);

	char *buffstrtok = strtok(intermediate, ":");
	buffspecs[0] = atoi(buffstrtok);
	for(int i=1; i< 18; i++)
	{
		buffstrtok = strtok(NULL, ":");
		buffspecs[i] = atoi(buffstrtok);
	}
	free(intermediate);
	//memset(buffstrtok, 0, 512);
	//free(buffstrtok);
	pkm->id = buffspecs[0];
	pkm->spec = buffspecs[1];
	pkm->hp = buffspecs[2];
	pkm->hpmax = buffspecs[3];
	pkm->xp = buffspecs[4];
	pkm->speed = buffspecs[5];
	pkm->a_f = buffspecs[6];
	pkm->a_eau = buffspecs[7];
	pkm->a_ele = buffspecs[8];
	pkm->a_pl = buffspecs[9];
	pkm->a_air = buffspecs[10];
	pkm->a_pi = buffspecs[11];
	pkm->d_f = buffspecs[12];
	pkm->d_eau = buffspecs[13];
	pkm->d_ele = buffspecs[14];
	pkm->d_pl = buffspecs[15];
	pkm->d_air = buffspecs[16];
	pkm->d_pi = buffspecs[17];
}

void serialize_pokemon(pokemon pkm, char **output)
{
	sprintf(*output, "%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d",pkm.id ,pkm.spec ,pkm.hp ,pkm.hpmax ,pkm.xp ,pkm.speed ,pkm.a_f ,pkm.a_eau ,pkm.a_ele ,pkm.a_pl ,pkm.a_air , pkm.a_pi , pkm.d_f , pkm.d_eau , pkm.d_ele , pkm.d_pl , pkm.d_air , pkm.d_pi);
}

void poke_dump(pokemon pkm)
{
	printf("Pokémon actuel : \nID : %d\nEspèce : %d\nVie : %d\nVie max : %d\nExperience : %d\nRapidité : %d\n", 
		pkm.id, pkm.spec,pkm.hp, pkm.hpmax, pkm.xp, pkm.speed);
}