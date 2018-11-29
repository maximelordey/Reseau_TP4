

#include <stdio.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

#include <netinet/in.h>
#include <netdb.h>
#define TAILLEBUFFER 256



void init_socket(char* port,struct sockaddr_in *p_adr_ser,char* adrIP);
void m_fgets(char* commande,int size,FILE* stream);
void shell(char* commande,char* buffer);
void s_put(int socket_id,char* filename,char* buffer);
void s_get(int socket_id,char* buffer);
