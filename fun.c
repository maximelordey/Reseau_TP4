#include "fun.h"


void init_socket(char* port,struct sockaddr_in *p_adr,char* adrIP){             //méthode qui permet d'initialise le contenus d'une socket
  
    memset((void *)p_adr, 0, sizeof(struct sockaddr_in));											  //on initialise notre socket a vide
    p_adr->sin_family = AF_INET;                                                //on utilise des adresses IPV4
    p_adr->sin_port = (unsigned short) strtoul(port, NULL, 0);;                  
    p_adr->sin_addr.s_addr = inet_addr(adrIP);                                  //Transforme l'ip en long int
}

void  m_fgets(char* commande,int size,FILE* stream){                            //méthode utilisée par le client
	printf("ubuntu@ubuntu:~$");                                                   //on affiche le début de l'invite de commande
	fflush(stdout);                                                               //on force l'affichage et on vide la sortie standart
  fgets(commande,size,stream);                                                  //on récupère la commande entrée par le client
  commande[strlen(commande)-1] = '\0';                                          //on met le caractère final a la fin de cette commande
}

void shell(char* commande,char* buffer){                                        //méthode utilisée pour le serveur, par le processus fils
	buffer[0]='\0';                                                               //on vide le buffer
	char tampon[TAILLEBUFFER];                                                    //on initialise un tampon avec la même taille que le buffer
	FILE *file;                                                                   //on déclare un fichier 
	if ((file = popen(commande, "r")) == NULL) {                                  // execute la fonction contenue dans commande
		perror("popen");
		exit(1);
	}
	
	while(fgets(tampon,TAILLEBUFFER, file) != NULL){                              //tant qu'on a pas fini de lire le contenu de file suite à l'éxecution de la commande
		strcat(buffer,tampon);                                                      //on concatène le contenu du tampon avec le buffer "global"
	}
	pclose(file);                                                                 //on a fini de lire le résultat de la commande, on ferme le fichier
}

void s_put(int id_socket,char* filename,char* buffer){                          //méthode utilisée pour envoyé un fichier au destinataire
	FILE* file = fopen(filename,"rb");                                            //on ouvre ouvre le fichier dont le nom a été passé en paramètre en lecture
	if(file == NULL){                                                             //si l'ouverture n'a pas eu lieu
		write(id_socket,"ERROR",TAILLEBUFFER);                                      //on averti l'interlocuteur du problème
		printf("ERREUR fichier inconnue\n");                                        //on indique le problème à l'utilisateur
		printf("Arret de l'envoie de fichier\n");
	}else{                                                                        //sinon, si l'ouverture à eu lieu
		write(id_socket,"NO_ERROR",TAILLEBUFFER);                                   //on avertie l'interlocuteur que l'ouverture a eu lieu
		int descriptor = fileno(file);                                              //on recupere le descripteur du fichier ouvert
		printf("Envoi du fichier %s ...\n",filename);                               //on écrite qu'on commence l'envoi
		write(id_socket,filename,TAILLEBUFFER);                                     
		int lu;
		while( (lu=read(descriptor,buffer,TAILLEBUFFER)) > 0){                      //tant qu'on lit des octets
        	        write(id_socket,buffer,lu);                                   //on envoi à l'interlocuteur la partie qui vient d'être lu
    }
		printf("Fichier %s envoyé\n",filename);                                     //l'envoi est terminé, on averti
		fclose(file);                                                               //on ferme le fichier
	}
}

void s_get(int id_socket,char* buffer){                                         //méthode pour récuperer un fichier
	char verif[TAILLEBUFFER];                                                     //on declare une chaine pour controler les erreurs
	char filename[TAILLEBUFFER];                                                  //on déclare une chaine pour stocker le nom du fichier
	read(id_socket,verif,TAILLEBUFFER);                                           //on lit ce que l'interlocuteur nous a envoyé et on le stock dans verif

	if (strcmp(verif,"NO_ERROR") != 0){                                           //si verif ne correspond pas à "NO_ERROR", c'est que le fichier na pas pu être envoyé
		printf("Aucun fichier recu\n");                                             //on préviens l'utilisateur
	}else{                                                                        //Sinon, la recpetion peut commencer
		read(id_socket,filename,TAILLEBUFFER);                                      //on lit le nom de fichier envoyé par l'interlocuteur
		printf("debut du téléchargement du fichier %s\n", filename);                //on préviens l'utilisateur du début du téléchargement
		FILE* file = fopen(filename,"w");                                           //on ouvre le fichier de meme nom (créer sinon) en mode écriture
		int descriptor = fileno(file);                                              //on récupere le descripteur de fichier du fichier ouver
		int octets_lu = read(id_socket,buffer,TAILLEBUFFER);                        //on lit le contenu du fichier que l'on stock dans le buffer (on sauvegarde le nb d'octets lu)
		while(octets_lu == TAILLEBUFFER){                                           //tant qu'on lit des octets
	    write(descriptor,buffer,TAILLEBUFFER);                                    //on écrit dans le fichier le contenu du buffer
			octets_lu = read(id_socket,buffer,TAILLEBUFFER);                          // on lit la suite du fichier
		}
		write(descriptor,buffer,octets_lu);                                         //on écrit les dernier octes lus contenus dans le buffer
		fclose(file);                                                               //on ferme le fichier receptionner    
		printf("Fin du téléchargement\n");                                          //on previent l'utilisateur que le téléchargement est terminé
		
	}
}
