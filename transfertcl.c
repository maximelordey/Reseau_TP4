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
	char *serveur= SERVEUR_DEFAUT; 																								/* serveur par defaut */
	char *service= SERVICE_DEFAUT; 																								/* numero de service par defaut (no de port) */
	
	if(argc == 1){																																//si l'utilisateur n'a pas entré d'arguments
  	 printf("L'adresse du serveur et le port n'ont pas été précisés, ce sont donc les valeurs par défauts qui seront utilisées : %s , %s \n",SERVEUR_DEFAUT,SERVICE_DEFAUT); //on averti que l'adresse et le port seront ceux par défaut
   }
   else if (argc == 2){																													//si l'utilisateur n'a pas entré le port
  	 printf("Le port n'a pas été précisé, c'est donc celui par défaut qui sera utilisé : %s\n",SERVICE_DEFAUT); //on averti que c'est celui par défaut qui sera utilsé
  	 serveur=argv[1];	
   }
   else{																																				//sinon c'est qu'il a entré les deux arguments
  	 serveur=argv[1];																														//On recupere la nom du serveur passé en parametre
  	 service=argv[2];																														//on recupere le numero de port passé en parametre 
   }
 commande = malloc(TAILLEBUFFER);																								//on allou la mémoire la chaine qui contiendra la commande voulant etre executée
 buffer = malloc(TAILLEBUFFER);																									//on allou la mémoire pour notre buffer
 client_appli(serveur,service);																									//on commence à executer le corp du client
}

/*****************************************************************************/
void client_appli (char *serveur,char *service)

/* procedure correspondant au traitement du client de votre application */
{
	id_socket = socket(AF_INET,SOCK_STREAM,0);																		//on cree la socket pour le clien courant
	if(id_socket == -1){																													//si la socket n'a pas été crée
			printf("\nERREUR : creation de socket impossible \n");										//on averti l'utilisateur
			exit(-1);																																	//on termine l'execution
	}
	printf("Phase 1 : Creation du Socket...\n");
	
	
	p_adr_socket = malloc(sizeof(struct sockaddr_in));														//on allou la mémore pour la 
	
	init_socket(service,p_adr_socket,serveur);																		//on appel la méthode qui initialise le contenu de la socket
	
	printf("Phase 2 : Tentative de connexion ...\n");
	int connexion = connect(id_socket,(struct sockaddr*)p_adr_socket,sizeof(struct sockaddr_in));	//on tente de se connecter au serveur

	if (connexion == -1){																													//si la connexion à échoué
		printf("\nERREUR : connexion impossible\n");																//on averti l'utilisateur
		exit(-1);																																		//on stop l'éxecution
	}
	printf("Phase 3 :Connexion établie\n");
	char* copiecommande = malloc(TAILLEBUFFER);																		//on alou la mémoire pour la commande entrée par l'utilisateur
	char* cmd;																																		//on déclare une chaine qui sera le nom de la commande
	char* arg;																																		//on déclare une chaine qui contientra le nom du fichier à put/get
	m_fgets(commande,TAILLEBUFFER,stdin);																					//on récupère la commande entrée par l'utilisateur						
	while ( strcmp(commande,"quit") != 0){																				//tant que l'utilisateur ne rentre pas quit
		if ( strlen(commande) != 0){																								// si la commande entrée n'est pas nulle (!= retour chariot)
			strcpy(copiecommande,commande);																						//on copie la commande dans copiecommande
			cmd = strtok(copiecommande," ");																					// on affecte à commande la premiere partie de la commande dans cmd (partie avant le "");
			arg = strtok(NULL," ");																										//on affecte à arg la seconde partie de la commande (le nom du fichier)
		
			if( write(id_socket,commande,TAILLEBUFFER) != -1){												//si l'envoi de la commande à fonctionner
				if ( strcmp(cmd,"get") == 0){																						// si la commande est get
					s_get(id_socket,buffer);																							// on receptionne le fichier
				}	
				else if( strcmp(cmd,"put") == 0){																				//sinon si la commande est put
					s_put(id_socket,arg,buffer);																					//on "upload" le fichier sur le serveur
				}
				else{																																		//si la commande est une autre commande (ls / pwd)
					read(id_socket,buffer,TAILLEBUFFER);																	//on recupere lexecution de cette dernière
					printf("%s\n",buffer);																								//on affiche le resultat
				}
			}
		}
		m_fgets(commande,TAILLEBUFFER,stdin);																				// on recupere la nouvelle commande entrée par l'utilisateur
	}
	close(id_socket);																															// l'utilisateur a entré "quit", on ferme la socket du client
	
}

/*****************************************************************************/

