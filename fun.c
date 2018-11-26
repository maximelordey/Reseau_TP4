#include "fun.h"


void init_socket(char* port,struct sockaddr_in *p_adr,char* adrIP){
  
    memset((void *)p_adr, 0, sizeof(struct sockaddr_in));											          //on initialise notre socket a vide
    
    
    p_adr->sin_family = AF_INET;
    p_adr->sin_port = (unsigned short) strtoul(port, NULL, 0);;                 
    p_adr->sin_addr.s_addr = inet_addr(adrIP);                                       //Transforme l'ip en long int
}
