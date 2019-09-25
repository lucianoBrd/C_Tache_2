#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>  
#include <sys/socket.h>
#include <netinet/in.h>

#include "client.h"
#include "bmp.h"

/* @brief
 * envoi et reception de message
 * envoi et reception de nom du client
 * envoi de l'opération et reception du resultat
 * envoi de couleurs et reception de confirmation
 * 
 * @params
 * socketfd : Socket pour communiquer avec le serveur.
 * type : Spécifie quelle fonction lancer (message, nom, calcule ou couleurs).
 * 
 * @return
 * 0 si tout va bien ou -1 en cas d'erreur.
 */
int envoie_recois_message(
  int 	socketfd, 
  char 	*type
) {
  if(	strcmp(type, "message") != 0 &&
	strcmp(type, "nom") 	!= 0 &&
	strcmp(type, "calcule") != 0 &&
	strcmp(type, "couleurs") != 0
    ) {
    printf("Type inconnue\n");
    return 0;
    
  } /* Check if type is correct */
  
  if(strcmp(type, "couleurs") == 0){
    return envoie_couleurs_simple(socketfd);
    
  } /* Check if type is couleurs */
  
  char 	data[1024],
	message[100];
  int 	write_status,
	read_status;
	
  memset(data, 0, sizeof(data));
  printf("Votre %s (max 1000 caracteres): ", type);
  fgets(message, 1024, stdin);
  
  /* Concatenate the type with the message to send to the serveur */
  strcpy(data, type);
  strcat(data, ": ");
  strcat(data, message);
  
  write_status = write(socketfd, data, strlen(data));
  if ( write_status < 0 ) {
    perror("erreur ecriture");
    exit(EXIT_FAILURE);
    
  } /* Error write */

  memset(data, 0, sizeof(data));
  read_status = read(socketfd, data, sizeof(data));
  if ( read_status < 0 ) {
    perror("erreur lecture");
    return -1;
    
  } /* Error read */

  printf("Message recu: %s\n", data);
 
  return 0;
  
} /* envoie_recois_message */

/* @brief
 * envoi de couleurs et reception de confirmation
 * 
 * @params
 * socketfd : Socket pour communiquer avec le serveur.
 * 
 * @return
 * 0 si tout va bien ou -1 en cas d'erreur.
 */
int envoie_couleurs_simple(
  int socketfd
){
  char 	data[1024],
	message[100];
  int 	n = 0;
  
  memset(data, 0, sizeof(data));
  while(n <= 0 || n >= 30){
    printf("Nombre de couleurs puis vos couleurs (max 1000 caracteres): ");
    fgets(message, 1024, stdin);
    
    /* Check number of colors */
    char number_c[10];
    sscanf(message, "%s", number_c);
    n = atoi(number_c);
  
  } /* while the user dont put a correct number */
  
  strcpy(data, "couleurs: ");
  strcat(data, message);
  
  int write_status = write(socketfd, data, strlen(data));
  if ( write_status < 0 ) {
    perror("erreur ecriture");
    exit(EXIT_FAILURE);
    
  } /* Error write */

  memset(data, 0, sizeof(data));
  int read_status = read(socketfd, data, sizeof(data));
  if ( read_status < 0 ) {
    perror("erreur lecture");
    return -1;
    
  } /* Error read */

  printf("Message recu: %s\n", data);
 
  return 0;
  
} /* envoie_couleurs_simple */

void analyse(char *pathname, char *data) {
  //compte de couleurs
  couleur_compteur *cc = analyse_bmp_image(pathname);

  int count;
  strcpy(data, "couleurs: ");
  char temp_string[10] = "10,";
  if (cc->size < 10) {
    sprintf(temp_string, "%d,", cc->size);
  }
  strcat(data, temp_string);
  
  //choisir 10 couleurs
  for (count = 1; count < 11 && cc->size - count >0; count++) {
    if(cc->compte_bit ==  BITS32) {
      sprintf(temp_string, "#%02x%02x%02x,", cc->cc.cc24[cc->size-count].c.rouge,cc->cc.cc32[cc->size-count].c.vert,cc->cc.cc32[cc->size-count].c.bleu);
    }
    if(cc->compte_bit ==  BITS24) {
      sprintf(temp_string, "#%02x%02x%02x,", cc->cc.cc32[cc->size-count].c.rouge,cc->cc.cc32[cc->size-count].c.vert,cc->cc.cc32[cc->size-count].c.bleu);
    }
    strcat(data, temp_string);
  }

  //enlever le dernier virgule
  data[strlen(data)-1] = '\0';
}

int envoie_couleurs(int socketfd, char *pathname) {
  char data[1024];
  memset(data, 0, sizeof(data));
  analyse(pathname, data);
  
  int write_status = write(socketfd, data, strlen(data));
  if ( write_status < 0 ) {
    perror("erreur ecriture");
    exit(EXIT_FAILURE);
  }

  return 0;
}


int main(int argc, char **argv) {
  int socketfd;
  int bind_status;

  struct sockaddr_in server_addr, client_addr;

  /*
   * Creation d'un socket
   */
  socketfd = socket(AF_INET, SOCK_STREAM, 0);
  if ( socketfd < 0 ) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  //détails du serveur (adresse et port)
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  //demande de connection au serveur
  int connect_status = connect(socketfd, (struct sockaddr *) &server_addr, sizeof(server_addr));
  if ( connect_status < 0 ) {
    perror("connection serveur");
    exit(EXIT_FAILURE);
  }
  
  char 	type[50],
	code[10];
  /*printf("Quelle fonction lancer (message, nom, calcule ou couleurs): ");
  fgets(type, 1024, stdin);
  
  sscanf(type, "%s", code);
  
  envoie_recois_message(socketfd, code);*/
  
  envoie_couleurs(socketfd, argv[1]);

  close(socketfd);
  
} /* main */
