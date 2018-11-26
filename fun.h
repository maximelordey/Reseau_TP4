

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


void init_socket(char* port,struct sockaddr_in *p_adr_ser,char* adrIP);