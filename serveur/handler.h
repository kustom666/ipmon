#ifndef DEF_HANDLER
#define DEF_HANDLER

#include <string.h>
#include "../Core/network.h"
#include "../Core/pokeheader.h"
#include "../Core/pokepacket.h"
#include "../Core/utility.h"
#include "../Core/generator.h"

void handle_nouveau(SOCKET sock, char *buffer, const struct sockaddr *dest_addr, int dest_len, char *recv_ip);
void handle_duel(SOCKET sock, char *buffer, const struct sockaddr *dest_addr, int dest_len, char *recv_ip);


#endif