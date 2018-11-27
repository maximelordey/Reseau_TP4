#include "fun.h"


void init_socket(char* port,struct sockaddr_in *p_adr,char* adrIP){
  
    memset((void *)p_adr, 0, sizeof(struct sockaddr_in));											          //on initialise notre socket a vide
    
    
    p_adr->sin_family = AF_INET;
    p_adr->sin_port = (unsigned short) strtoul(port, NULL, 0);;                 
    p_adr->sin_addr.s_addr = inet_addr(adrIP);                                       //Transforme l'ip en long int
}

void  m_fgets(char* commande,int size,FILE* stream){	
	printf("ubuntu@ubuntu:~$");
	fflush(stdout);
        fgets(commande,size,stream);
        commande[strlen(commande)-1] = '\0';
}

void shell(char* commande,char* buffer){
	buffer[0]='\0';
	char tampon[1024];
	FILE *file;
	if ((file = popen(commande, "r")) == NULL) {
		perror("popen");
		exit(1);
	}
	
	while(fgets(tampon,1024, file) != NULL){
		strcat(buffer,tampon);	
	}
	pclose(file);
}
