#include "pokemon.h"

void unserialize_pokemon(char *input, pokemon *pkm)
{
	int buffspecs[18];
	char *intermediate = (char *) malloc(512*sizeof(char));
	strcpy(intermediate, input);

	char *storer = (char*) malloc(64*sizeof(char));
	char *buffstrtok = (char*)malloc(512*sizeof(char));
	buffstrtok = strtok_r(intermediate, ":", &storer);
	buffspecs[0] = atoi(buffstrtok);
	for(int i=1; i< 18; i++)
	{
		buffstrtok = strtok_r(NULL, ":", &storer);
		buffspecs[i] = atoi(buffstrtok);
	}
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
	free(storer);
	free(buffstrtok);
	free(intermediate);
}