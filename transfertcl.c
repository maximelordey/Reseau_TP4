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
char* commande;
char* buffer;

void client_appli (char *serveur, char *service);


/*****************************************************************************/
/*--------------- programme client -----------------------*/

int main(int argc, char *argv[])
{
	char *serveur= SERVEUR_DEFAUT; /* serveur par defaut */
	char *service= SERVICE_DEFAUT; /* numero de service par defaut (no de port) */
	
	if(argc == 1){
  	 printf("L'adresse du serveur et le port n'ont pas été précisés, ce sont donc les valeurs par défauts qui seront utilisées : %s , %s \n",SERVEUR_DEFAUT,SERVICE_DEFAUT);
   }
   else if (argc == 2){
  	 printf("Le port n'a pas été précisé, c'est donc celui par défaut qui sera utilisé : %s\n",SERVICE_DEFAUT);
  	 serveur=argv[1];	
   }
   else{
  	 serveur=argv[1];																																//On recupere la nom du serveur passé en parametre
  	 service=argv[2];																																//on recupere le numero de port passé en parametre 
   }
 commande = malloc(TAILLEBUFFER);
 buffer = malloc(TAILLEBUFFER);
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
	printf("Phase 1 : Creation du Socket...\n");
	
	
	p_adr_socket = malloc(sizeof(struct sockaddr_in));
	memset((void*)p_adr_socket,0,sizeof(struct sockaddr_in));

	p_adr_socket->sin_family = AF_INET;
	p_adr_socket->sin_port = (unsigned short)strtoul(service,NULL,0);
	p_adr_socket->sin_addr.s_addr = inet_addr(serveur);
	
	printf("Phase 2 : Tentative de connexion ...\n");
	int connexion = connect(id_socket,(struct sockaddr*)p_adr_socket,sizeof(struct sockaddr_in));

	if (connexion == -1){
		printf("\nERREUR : connexion impossible\n");
		exit(-1);
	}
	printf("Phase 3 :Connexion établie\n");
	char* copiecommande = malloc(TAILLEBUFFER);
	char* cmd;
	char* arg;
	m_fgets(commande,TAILLEBUFFER,stdin);
	while ( strcmp(commande,"exit") != 0){
		if ( strlen(commande) != 0){
			strcpy(copiecommande,commande);
			cmd = strtok(copiecommande," ");
			arg = strtok(NULL," ");
		
			if( write(id_socket,commande,TAILLEBUFFER) != -1){
				if ( strcmp(cmd,"get") == 0){
					s_get(id_socket,buffer);
				}
				else if( strcmp(cmd,"put") == 0){
					s_put(id_socket,arg,buffer);
				}
				else{
					read(id_socket,buffer,TAILLEBUFFER);
					printf("%s\n",buffer);
				}
			}
		}
		m_fgets(commande,TAILLEBUFFER,stdin);
	}
	close(id_socket);
	
}

/*****************************************************************************/

