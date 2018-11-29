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
	char tampon[TAILLEBUFFER];
	FILE *file;
	if ((file = popen(commande, "r")) == NULL) {
		perror("popen");
		exit(1);
	}
	
	while(fgets(tampon,TAILLEBUFFER, file) != NULL){
		strcat(buffer,tampon);
	}
	pclose(file);
}

void s_put(int id_socket,char* filename,char* buffer){
	FILE* file = fopen(filename,"rb");
	if(file == NULL){
		write(id_socket,"ERROR",TAILLEBUFFER);
		printf("ERREUR fichier inconnue\n");
		printf("arret de l'envoie de fichier\n");
	}else{
		write(id_socket,"NO_ERROR",TAILLEBUFFER);
		int descriptor = fileno(file);
		printf("envoie du fichier %s ...\n",filename);
		write(id_socket,filename,TAILLEBUFFER);
		int lu;
		while( (lu=read(descriptor,buffer,TAILLEBUFFER)) > 0){
        	        write(id_socket,buffer,lu);
        	}
		printf("fichier %s envoiyé\n",filename);
		fclose(file);
	}
}

void s_get(int id_socket,char* buffer){
	char verif[TAILLEBUFFER];
	char filename[TAILLEBUFFER];
	read(id_socket,verif,TAILLEBUFFER);

	if (strcmp(verif,"NO_ERROR") != 0){
		printf("Aucun fichier receptionner\n");
	}else{
		read(id_socket,filename,TAILLEBUFFER);
		printf("debut du telechargement du fichier %s\n", filename);
		FILE* file = fopen(filename,"w");
		int descriptor = fileno(file);
		int octets_lu = read(id_socket,buffer,TAILLEBUFFER);
		while(octets_lu == TAILLEBUFFER){
	        	write(descriptor,buffer,TAILLEBUFFER);
			octets_lu = read(id_socket,buffer,TAILLEBUFFER);
		}
			write(descriptor,buffer,octets_lu);
		fclose(file);
		printf("fin du telecharement\n");
		
	}
}
