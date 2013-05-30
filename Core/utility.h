#ifndef DEF_UTILITY
#define DEF_UTILITY
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define TAG_LOGI "LOGI" //Tag login
#define TAG_DATA "DATA" //Tag ??
#define TAG_NOUV "NOUV" //Tag nouveau joueur
#define TAG_DONE "DONE" //Tag duel fini
#define TAG_DINI "DINI" //Tag nouveau duel
#define TAG_DACK "DACK" //Tag Duel attack
#define TAG_ATCK "ATCK" //Tag doublon
#define TAG_ISSU "ISSU" //Tag issue

typedef struct joueur{
	char *nickname;
	char *ip;
}joueur;

void range_strcpy(char *s1, const char *s2, int rb, int re);
char * serialize_uint32( char *buffer, uint32_t value);
char serialize_uint8( char buffer, uint8_t value);
uint32_t unserialize_uint32( char *buffer);
char * pck_type(const char *buffer);

#endif
