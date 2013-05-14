#ifndef DEF_NETWORK
#define DEF_NETWORK

#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32 

#include <winsock2.h> 

#elif defined (linux)


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>

#include <arpa/inet.h>
#include <unistd.h> 
#include <netdb.h> 

// Typedefs pratiques pour recconnaitre les types (portage windows)
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;


#else


#endif

#include "pokeheader.h"
#include "utility.h"
// DEFINE du port et de la compatibilité ascendante de h_addr
#define l_port 1151
#define h_addr  h_addr_list[0]


SOCKET setup_socket();
SOCKADDR_IN setup_addr();
SOCKADDR_IN setup_send_addr();
int send_pokeheader(SOCKET socket, const SOCKADDR *dest, int dlen, pokeheader *header);
void winsock_init();
void winsock_end();

#endif