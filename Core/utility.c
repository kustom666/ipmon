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
  return buffer;
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

char * pck_type(const char *buffer)
{
	if(buffer[0] == 'L' && buffer[1] == 'O' && buffer[2] == 'G' && buffer[3] == 'I')
	{
		return TAG_LOGI;
	}
	else if(buffer[0] == 'D' && buffer[1] == 'A' && buffer[2] == 'T' && buffer[3] == 'A')
	{
		return TAG_DATA;
	}
	else if(buffer[0] == 'N' && buffer[1] == 'O' && buffer[2] == 'U' && buffer[3] == 'V')
	{
		return TAG_NOUV;
	}
	else if(buffer[0] == 'D' && buffer[1] == 'O' && buffer[2] == 'N' && buffer[3] == 'E')
	{
		return TAG_DONE;
	}
	else if(buffer[0] == 'D' && buffer[1] == 'I' && buffer[2] == 'N' && buffer[3] == 'I')
	{
		return TAG_DINI;
	}
	else if(buffer[0] == 'D' && buffer[1] == 'A' && buffer[2] == 'C' && buffer[3] == 'K')
	{
		return TAG_DACK;
	}
	else if(buffer[0] == 'A' && buffer[1] == 'T' && buffer[2] == 'C' && buffer[3] == 'k')
	{
		return TAG_ATCK;
	}
	else if(buffer[0] == 'I' && buffer[1] == 'S' && buffer[2] == 'S' && buffer[3] == 'U')
	{
		return TAG_ISSU;
	}
	else 
	{
		return NULL;
	}
}