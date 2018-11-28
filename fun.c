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

void s_put(int id_socket,char* filename,char* buffer){
	FILE* file = fopen(filename,"r");

	if(file == NULL){
		write(id_socket,"ERROR",1024);
		printf("ERREUR fichier inconnue\n");
		printf("arret de l'envoie de fichier\n");
	}else{
		write(id_socket,"NO_ERROR",1024);

		printf("envoie du fichier %s ...\n",filename);
		write(id_socket,filename,1024);
		
		while(fgets(buffer,1024, file) != NULL){
        	        write(id_socket,buffer,1024);  
        	}
		printf("fichier %s envoiyé\n",filename);
		fclose(file);
	}
}

void s_get(int id_socket,char* buffer){
	char verif[1024];
	char filename[1024];
	read(id_socket,verif,1024);
	verif[1023]='\0';

	if (strcmp(verif,"NO_ERROR") != 0){
		printf("Aucun fichier receptionner\n");
	}else{
		read(id_socket,filename,1024);
		printf("debut du telechargement du fichier %s \n", filename);
		
		FILE* file = fopen(filename,"w");
		int nb = read(id_socket,buffer,1024);
		int result = fputs(buffer,file);
		printf("%d\n",nb);
		while( result != EOF || nb > 0){
			nb = read(id_socket,buffer,1024);
                	result = fputs(buffer,file);
			printf("%d\n",nb);
		}
		printf("fin du telecharement\n");
		fclose(file);
	}

}
