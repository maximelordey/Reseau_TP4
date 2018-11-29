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
#define LOCALHOST "127.0.0.1"																										//on declare une macro qui contient l'adresse locale


int id_main_socket_serveur;                                                     //on déclare la socket "principale" du serveur
int id_socket_serveur_client;                                                   //on decalre une socket pour un client c

char* buffer;                                                                   //on déclare notre tampon

struct sockaddr_in *p_adr_socket;

void serveur_appli (char *service);   /* programme serveur */



/******************************************************************************/
/*---------------- programme serveur ------------------------------*/

int main(int argc,char *argv[])
{
	srand(time(NULL));
	
	char* port;
	/* Permet de passer un nombre de parametre variable a l'executable */
	if(argc == 1){																																//si l'utilisateur n'entre pas de port en argument
		printf("Le port du serveur n'a pas été saisi, le port part défaut sera donc utilisé : %s\n",SERVICE_DEFAUT);	 //on le previens que c'est celui par défaut qui sera utilisé
		port= SERVICE_DEFAUT; /* numero de service par defaut */
	}
	else{																													
		port = argv[1];																															//sinon on affecte celui entré par l'utilisateur
		if(strcmp(port,"0") == 0){																									//si le port saisi est 0
			printf("Impossible d'utiliser le port 0, celui par défaut sera utilisé (%s)\n",SERVICE_DEFAUT); //on averti que le port sera finalement celui par défaut
			port = SERVICE_DEFAUT;
		}
		else{
			printf("Le port utilisé pour la suite de l'execution sera : %s \n",port);
		}
		
	}

	buffer = malloc(TAILLEBUFFER);																								//on allou la mémoire pour notre buffer		
	serveur_appli(port);																													//on execute le corp du serveur
}


/******************************************************************************/
void serveur_appli(char *port)

/* Procedure correspondant au traitemnt du serveur de notre application */

{
	
	printf("Phase 1 : Création de la socket du serveur\n");
	id_main_socket_serveur = socket(AF_INET,SOCK_STREAM,0);                       //on creer la socket pour le serveur qui va utiliser TCP
	int sizeADDRIN = sizeof(struct sockaddr_in);
	p_adr_socket = malloc(sizeADDRIN);																						//on initialise la plage mémoire pour notre socket
	
	if(id_main_socket_serveur == -1){																							//si la socket n'a pas pu être créer
			printf("\nERREUR : creation de socket impossible \n");										//on averti l'utilisateur du problème
			exit(-1);																																	//on arrete l'execution
	}
	
	printf("Phase 2 : Initialisation de la socket du serveur\n");
	init_socket(port,p_adr_socket,LOCALHOST);																			//on appel la fonction init qui remplie la structure de socket
	int resultBind =  bind(id_main_socket_serveur,(struct sockaddr *)p_adr_socket, sizeADDRIN); //on associe la socket aux adresses

	if (resultBind == -1){																												//si problème de bind
		printf("\nERREUR : bind de socket \n");																			//on averti l'utilisateur
		exit(-1);																																		//on stop l'execution
	}
	
	printf("Phase 3 : A l'écoute d'une connexion ...\n");
	int resultListen = listen(id_main_socket_serveur,SOMAXCONN);									//on attend la connexion d'un potentiel client
	
	if (resultListen == -1){
		printf("\nERREUR : listen de socket \n");
		exit(-1);
	}
	
	socklen_t taille_client = sizeof(struct sockaddr_in);  												// Taille de la structure du client
	
	while(1){
		id_socket_serveur_client = accept(id_main_socket_serveur,(struct sockaddr *) p_adr_socket,&taille_client); //on accepte la connexion du client
		printf("Phase 4 : Acceptation de la connexion d'un client\n");
		
		if(id_socket_serveur_client < 0){																						//si problème d'acceptation
			printf("Acceptation du client impossible, abandon\n");										//on averti l'utilisateur
			exit(-1);
		}
		printf("Phase 5 : Connexion établie\n");
		
		if(fork()==0){																															//on créer un fils et si on est dans ce dernier
			printf("Processus fils crée\n");																					// On créer un processus fils
			close(id_main_socket_serveur); 																						//on ferme la sokcet principale du serveur pour le fils seulement

		  char* copiecommande = malloc(TAILLEBUFFER);
		  char* cmd;
		  char* arg;
			char message[TAILLEBUFFER];
			
			while(1){
				read(id_socket_serveur_client,message,TAILLEBUFFER);                    //on lit la commande entrée par l'utilisateur
				strcpy(copiecommande,message);																					//on la copie dans copiecommande
		    cmd = strtok(copiecommande," ");																				//on prend la premiere partie de la commande (partie droite avant le premier espace) dans cmd
	      arg = strtok(NULL," ");																									//on prend la partie gauche dans arg (nom du fichier)
				
				if (strcmp(cmd,"put") == 0){																						//si le client veut upload
					s_get(id_socket_serveur_client,buffer);																//alors on télécharge le fichier envoyé
				}
				else if(strcmp(cmd,"get") == 0){																				//sinon si le client veut récupere un fichier
					s_put(id_socket_serveur_client,arg,buffer);														//on lui envoi celui qu'il a demandé
				}
				else{																																		//sinon si la commande est autre (ls/pwd)
					shell(message,buffer);																								//on execute la commande
					write(id_socket_serveur_client,buffer,TAILLEBUFFER);									//on envoi le resultat au client
				}
				message[0]='\0';																												//on vide notre buffer
			}

			close(id_socket_serveur_client);																					//les echanges avec le client sont terminés, on ferme la socket pour le processus fils
			printf("Fin processus fils\n");
			exit(0);
	}
	 else {
			close(id_socket_serveur_client);																					//on ferme la socket du client pour le serveur
	}
		close(id_socket_serveur_client);																						//on ferme la socket
	}
		close(id_main_socket_serveur);																							//on ferme la socket principale
		
/* A completer ... */
																					
}

/******************************************************************************/
