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
	char *port= SERVICE_DEFAUT; /* numero de service par defaut */


	/* Permet de passer un nombre de parametre variable a l'executable */
	if(argc != 2){
		printf("Usage:transfertse n° port \n");
		exit(1);
	}
	port = argv[1];
	
	/* service est le numero de port auquel sera affecte
	ce serveur*/

	serveur_appli(port);
}


/******************************************************************************/
void serveur_appli(char *port)

/* Procedure correspondant au traitemnt du serveur de notre application */

{
	id_main_socket_serveur = socket(AF_INET,SOCK_STREAM,0);                       //on creer la socket pour le serveur qui va utiliser TCP
	int sizeADDRIN = sizeof(struct sockaddr_in);
	p_adr_socket = malloc(sizeADDRIN);																						//on initialise la plage mémoire pour notre socket
	
	if(id_main_socket_serveur == -1){
			printf("\nERREUR : creation de socket impossible \n");
			exit(-1);
	}
	
	init_socket(port,p_adr_socket,LOCALHOST);
	int resultBind =  bind(id_main_socket_serveur,(struct sockaddr *)p_adr_socket, sizeADDRIN);

	if (resultBind == -1){
		printf("\nERREUR : bind de socket \n");
		exit(-1)
	}
	
	int resultListen = listen(id_main_socket_serveur,SOMAXCONN);
	
	if (resultListen == -1){
		printf("\nERREUR : listen de socket \n");
		exit(-1)
	}
/* A completer ... */
																					
}

/******************************************************************************/
