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

unsigned char * serialize_uint32(unsigned char *buffer, uint32_t value)
{
  buffer[0] = value >> 24;
  buffer[1] = value >> 16;
  buffer[2] = value >> 8;
  buffer[3] = value;
  return buffer;
}

unsigned char * serialize_uint8(unsigned char *buffer, uint8_t value)
{
  buffer[0] = value;
  return buffer + 1;
}

uint8_t unserialize_uint8(unsigned char *buffer)
{
	uint8_t value = buffer[0];
	return value;
}

uint32_t unserialize_uint32(unsigned char *buffer)
{
	uint32_t value = 0;
	value |= buffer[0] << 24;
	value |= buffer[1] << 16;
	value |= buffer[2] << 8;
	value |= buffer[3];
	return value;
}