#ifndef DEF_UTILITY
#define DEF_UTILITY
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define TAG_LOGI "LOGI"
#define TAG_DATA "DATA"
#define TAG_NOUV "NOUV"
#define TAG_DONE "DONE"
#define TAG_DINI "DINI"
#define TAG_DACK "DACK"
#define TAG_ATCK "ATCK"
#define TAG_ISSU "ISSU"

void range_strcpy(char *s1, const char *s2, int rb, int re);
unsigned char * serialize_uint32(unsigned char *buffer, uint32_t value);
unsigned char * serialize_uint8(unsigned char *buffer, uint8_t value);
uint32_t unserialize_uint32(unsigned char *buffer);
char * pck_type(const char *buffer);

#endif
