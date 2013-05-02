#ifndef DEF_NETWORK
#define DEF_NETWORK

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>

#include <arpa/inet.h>
#include <unistd.h> 
#include <netdb.h> 

// DEFINE du port et de la compatibilité ascendante de h_addr
#define l_port 1151
#define h_addr  h_addr_list[0]


// Typedefs pratiques pour recconnaitre les types
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;


SOCKET setup_socket();
SOCKADDR_IN setup_addr();
SOCKADDR_IN setup_send_addr();

#endif