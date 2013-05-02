#include "utility.h"

void range_strcpy(char *s1, const char *s2, int rb, int re)
{
	while( (rb <= re) && (*s1++ = s2[rb]) ) rb++;
    *s1 = 0;
}