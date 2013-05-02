#include "utility.h"

void range_strcpy(char *s1, const char *s2, int rb, int re)
{
	int j = 0;
	for (int i = rb; i <= re; i++)
	{
		s1[j] = s2[i];
		j++;
	}
}