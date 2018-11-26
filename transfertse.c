/******************************************************************************/
/*			Application: ....			              */
/******************************************************************************/
/*									      */
/*			 programme  SERVEUR 				      */
/*									      */
/******************************************************************************/
/*									      */
/*		Auteurs :  ....						      */
/*		Date :  ....						      */
/*									      */
/******************************************************************************/

#include<stdio.h>
#include<string.h>
#include <curses.h>

#include<sys/signal.h>
#include<sys/wait.h>
#include<stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>

#include "time.h"
#include "fun.h"

#define SERVICE_DEFAUT "1111"
#define TAILLEMAX 27
#define LOCALHOST "127.0.0.1"																									//on declare une macro qui contient l'adresse locale



int id_main_socket_serveur;                                                     //on déclare la socket "principale" du serveur
int id_socket_serveur_client;                                                   //on decalre une socket pour un client c

char* tampon;                                                                   //on déclare notre tampon



struct sockaddr_in *p_adr_socket;

const int TAILLE_BUFFER = 500*sizeof(char);




void serveur_appli (char *service);   /* programme serveur */



/******************************************************************************/
/*---------------- programme serveur ------------------------------*/

int main(int argc,char *argv[])
{
	srand(time(NULL));
	
	char* port;


	/* Permet de passer un nombre de parametre variable a l'executable */
	if(argc == 1){
		printf("Le port du serveur n'a pas été saisi, le port part défaut sera donc utilisé : %s\n",SERVICE_DEFAUT);
		port= SERVICE_DEFAUT; /* numero de service par defaut */
	}
	else{
		port = argv[1];
		if(strcmp(port,"0") == 0){
			printf("Impossible d'utiliser le port 0, celui par défaut sera utilisé (%s)\n",SERVICE_DEFAUT);
			port = SERVICE_DEFAUT;
		}
		else{
			printf("Le port utilisé pour la suite de l'execution sera : %s \n",port);
		}
		
	}
	
	
	/* service est le numero de port auquel sera affecte
	ce serveur*/

	serveur_appli(port);
}


/******************************************************************************/
void serveur_appli(char *port)

/* Procedure correspondant au traitemnt du serveur de notre application */

{
	
	printf("Phase 1 : Création de la socket du serveur\n");
	id_main_socket_serveur = socket(AF_INET,SOCK_STREAM,0);                       //on creer la socket pour le serveur qui va utiliser TCP
	int sizeADDRIN = sizeof(struct sockaddr_in);
	p_adr_socket = malloc(sizeADDRIN);																						//on initialise la plage mémoire pour notre socket
	
	if(id_main_socket_serveur == -1){
			printf("\nERREUR : creation de socket impossible \n");
			exit(-1);
	}
	
	printf("Phase 2 : Initialisation de la socket du serveur\n");
	init_socket(port,p_adr_socket,LOCALHOST);																			//on appel la fonction init qui remplie la structure de socket
	int resultBind =  bind(id_main_socket_serveur,(struct sockaddr *)p_adr_socket, sizeADDRIN);

	if (resultBind == -1){
		printf("\nERREUR : bind de socket \n");
		exit(-1);
	}
	
	printf("Phase 3 : A l'écoute d'une connexion ...\n");
	int resultListen = listen(id_main_socket_serveur,SOMAXCONN);									//on attend la connexion d'un potentiel client
	
	if (resultListen == -1){
		printf("\nERREUR : listen de socket \n");
		exit(-1);
	}
	
	socklen_t taille_client = sizeof(struct sockaddr_in);  // Taille de la structure du client
	
	while(1){
		id_socket_serveur_client = accept(id_main_socket_serveur,(struct sockaddr *) p_adr_socket,&taille_client); //on accepte la connexion du client
		printf("Phase 4 : Acceptation de la connexion d'un client\n");
		
		if(id_socket_serveur_client < 0){
			printf("Acceptation du client impossible, abandon\n");
			exit(-1);
		}
		printf("Phase 5 : Connexion établie\n");
		
		if(fork()==0){
			printf("Processus fils crée\n");																									// On créer un processus fils
			close(id_main_socket_serveur); 																										//on ferme la sokcet principale du serveur pour le fils seulement
			
			char message[10];

			while(1){
					//read(id_socket_serveur_client,message,4);
					//printf("%s\n",message);
			}

			close(id_socket_serveur_client);																									//les echanges avec le client sont terminés, on ferme la socket pour le processus fils
			printf("Fin processus fils\n");
			exit(0);
	}
	 else {
			close(id_socket_serveur_client);																									//on ferme la socket du client pour le serveur
	}
		close(id_socket_serveur_client);
	}
		close(id_main_socket_serveur);																											//on ferme la socket principale
		
/* A completer ... */
																					
}

/******************************************************************************/
