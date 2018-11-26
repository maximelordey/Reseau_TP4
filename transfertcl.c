/******************************************************************************/
/*			Application: ...					*/
/******************************************************************************/
/*									      */
/*			 programme  CLIENT				      */
/*									      */
/******************************************************************************/
/*									      */
/*		Auteurs : ... 					*/
/*									      */
/******************************************************************************/


#include <stdio.h>
#include <curses.h> 		/* Primitives de gestion d'ecran */
#include <sys/signal.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "fun.h"

#define SERVICE_DEFAUT "1111"
#define SERVEUR_DEFAUT "127.0.0.1"

int id_socket;																																	//entier qui endentifi la socket
struct sockaddr_in *p_adr_socket;

void client_appli (char *serveur, char *service);


/*****************************************************************************/
/*--------------- programme client -----------------------*/

int main(int argc, char *argv[])
{
	char *serveur= SERVEUR_DEFAUT; /* serveur par defaut */
	char *service= SERVICE_DEFAUT; /* numero de service par defaut (no de port) */
	
 if(argc != 3){
	 printf("Usage:transfertcl nomServeyr n° port \n");
	 exit(1);
 }
 serveur=argv[1];																																//On recupere la nom du serveur passé en parametre
 service=argv[2];																																//on recupere le numero de port passé en parametre 
 
 client_appli(serveur,service);
}

/*****************************************************************************/
void client_appli (char *serveur,char *service)

/* procedure correspondant au traitement du client de votre application */
{
	id_socket = socket(AF_INET,SOCK_STREAM,0);																		//on cree la socket pour le clien courant
	if(id_socket == -1){
			printf("\nERREUR : creation de socket impossible \n");
			exit(-1);
	}
	printf("Creation du Socket...\n");
	
	
	p_adr_socket = malloc(sizeof(struct sockaddr_in));
	memset((void*)p_adr_socket,0,sizeof(struct sockaddr_in));

	p_adr_socket->sin_family = AF_INET;
	p_adr_socket->sin_port = (unsigned short)strtoul(service,NULL,0);
	p_adr_socket->sin_addr.s_addr = inet_addr(serveur);
	
	int connexion = connect(id_socket,(struct sockaddr*)p_adr_socket,sizeof(struct sockaddr_in));

	if (connexion == 1){
		printf("\nERREUR : connexion impossible");
		exit(-1);
	}
	printf("Connexion en cours...\n");
}

/*****************************************************************************/
