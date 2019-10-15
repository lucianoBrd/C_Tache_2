#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "client.h"

/* @brief
 * envoi et reception de message
 * envoi et reception de nom du client
 * envoi de l'opération et reception du resultat
 * envoi de couleurs et reception de confirmation
 *
 * @params
 * socketfd : Socket pour communiquer avec le serveur.
 * type : Spécifie quelle fonction lancer (message, nom, calcule ou couleurs).
 * pathname : Chemin de l'image à traiter.
 *
 * @return
 * 0 si tout va bien ou -1 en cas d'erreur.
 */
int envoie_recois_message(
  int 	socketfd,
  char 	*type,
  char  *pathname
){
  if(	strcmp(type, "message")  != 0 &&
    	strcmp(type, "nom") 	   != 0 &&
    	strcmp(type, "calcule")  != 0 &&
    	strcmp(type, "couleurs") != 0 ){
    printf("Type inconnue\n");
    return 0;

  } /* Check if type is correct */

  if(strcmp(type, "couleurs") == 0){
    if(pathname == NULL){
	     printf("Vous devez spécifier une image.\n");
       return 0;

    } /* Check if we have the path of the picture */
    return envoie_couleurs(socketfd, pathname);

  } /* Check if type is couleurs */

  char  data[DATA_SIZE],
	      message[VALEURS_SIZE];
  int   write_status,
	      read_status;

  if( strcmp(type, "message") == 0 ||
      strcmp(type, "nom")     == 0 ){
    /* Message or name case */
    /* Create object */
    message_json *json = new_message_json(1);
    /* Set the code of the message */
    strcpy(json->code, type);

    json->nb_valeurs = 1;

    /* Ask the value of the message */
    printf("Votre %s (max %d caracteres): ", type, VALEURS_SIZE);
    fgets(json->valeurs[0], VALEURS_SIZE, stdin);

    /* Create the string and delete the object */
    create_message_json(data, json);
    delete_message_json(json);

  } else {
    /* Compute case */
    /* Create object */
    message_json *json = new_message_json(3);
    /* Set the code of the message */
    strcpy(json->code, type);

    json->nb_valeurs = 3;

    /* Ask the value of the operator of the message */
    printf("Votre opérateur du %s (max %d caracteres): ", type, VALEURS_SIZE);
    fgets(json->valeurs[0], VALEURS_SIZE, stdin);
    /* Remove the return line */
    json->valeurs[0][strcspn(json->valeurs[0], "\n")] = 0;

    /* Ask the value of the first number of the message */
    printf("Votre premier nombre du %s (max %d caracteres): ", type, VALEURS_SIZE);
    fgets(json->valeurs[1], VALEURS_SIZE, stdin);
    /* Remove the return line */
    json->valeurs[1][strcspn(json->valeurs[1], "\n")] = 0;

    /* Ask the value of the second number of the message */
    printf("Votre second nombre du %s (max %d caracteres): ", type, VALEURS_SIZE);
    fgets(json->valeurs[2], VALEURS_SIZE, stdin);
    /* Remove the return line */
    json->valeurs[2][strcspn(json->valeurs[2], "\n")] = 0;

    /* Create the string and delete the object */
    create_message_json(data, json);
    delete_message_json(json);

  } /* Create the string JSON message */

  /* Send to the server the message */
  write_status = write(socketfd, data, strlen(data));
  if(write_status < 0){
    perror("erreur ecriture");
    exit(EXIT_FAILURE);

  } /* Error write */

  /* Read the response of the server */
  memset(data, 0, sizeof(data));
  read_status = read(socketfd, data, sizeof(data));
  if(read_status < 0){
    perror("erreur lecture");
    return -1;

  } /* Error read */

  /* Print the response */
  printf("Message recu :\n");
  message_json *json = create_object_json(data);
  print_message_json(json);
  delete_message_json(json);

  return 0;

} /* envoie_recois_message */

/* @brief
 * Analyse une image. Demande à l'utilisateur
 * combien de couleurs il veut récupérer, puis,
 * on stocke ces couleurs dans une string au format JSON.
 *
 * @params
 * pathname : Chemin de l'image à traiter.
 * data : String ou l'on stocke le message JSON.
 */
void analyse(
  char *pathname,
  char *data
){
  couleur_compteur  *cc   = analyse_bmp_image(pathname);
  int 	            n     = -1,
                    count;
  message_json      *json;
  char 	            number_c[10],
                    temp_string[10];

  /* Check number of colors */
  while(n <= 0 || n >= 30){
    /* Ask the user how many colors */
    printf("\nCombien de couleurs ? ");
    fgets(number_c, 10, stdin);

    n = atoi(number_c);

  } /* while the user dont put a correct number */

  /* New object */
  json = new_message_json(n);
  strcpy(json->code, "couleurs");

  for (count = 1; count < (n + 1) && cc->size - count > 0; count++){
    if(cc->compte_bit ==  BITS32){
      sprintf(json->valeurs[count - 1], "#%02x%02x%02x", cc->cc.cc24[cc->size-count].c.rouge,cc->cc.cc32[cc->size-count].c.vert,cc->cc.cc32[cc->size-count].c.bleu);

    } /* Bit 32 case */
    if(cc->compte_bit ==  BITS24){
      sprintf(json->valeurs[count - 1], "#%02x%02x%02x", cc->cc.cc32[cc->size-count].c.rouge,cc->cc.cc32[cc->size-count].c.vert,cc->cc.cc32[cc->size-count].c.bleu);

    } /* Bit 24 case */

  } /* For n colors that the user whant */

  /* Create the string and delete the object */
  create_message_json(data, json);
  delete_message_json(json);

} /* analyse */

/* @brief
 * Envoie au serveur n couleurs.
 *
 * @params
 * socketfd : Socket pour communiquer avec le serveur.
 * pathname : Chemin de l'image à traiter.
 *
 * @return
 * 0 si tout va bien ou -1 en cas d'erreur.
 */
int envoie_couleurs(
  int   socketfd,
  char  *pathname
){
  char  data[DATA_SIZE];
  int   write_status,
        read_status;
  memset(data, 0, sizeof(data));
  analyse(pathname, data);

  /* Send to the server the message */
  write_status = write(socketfd, data, strlen(data));
  if(write_status < 0){
    perror("erreur ecriture");
    exit(EXIT_FAILURE);

  } /* Error write */

  /* Read the response of the server */
  memset(data, 0, sizeof(data));
  read_status = read(socketfd, data, sizeof(data));
  if(read_status < 0){
    perror("erreur lecture");
    return -1;

  } /* Error read */

  /* Print the response */
  printf("Message recu :\n");
  message_json *json = create_object_json(data);
  print_message_json(json);
  delete_message_json(json);

  return 0;

} /* envoie_couleurs */

/* @brief
 * Main fonction.
 */
int main(
  int   argc,
  char  **argv
){
  int socketfd,
      bind_status;

  struct sockaddr_in server_addr, client_addr;

  /* Create the socket */
  socketfd = socket(AF_INET, SOCK_STREAM, 0);
  if(socketfd < 0){
    perror("socket");
    exit(EXIT_FAILURE);

  }

  /* Put information of the server */
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family      = AF_INET;
  server_addr.sin_port        = htons(PORT);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  /* Ask the server */
  int connect_status = connect(socketfd, (struct sockaddr *) &server_addr, sizeof(server_addr));
  if(connect_status < 0){
    perror("connection serveur");
    exit(EXIT_FAILURE);

  } /* Error connect */

  char type[DATA_SIZE];

  /* Ask wich function execute */
  printf("Quelle fonction lancer (message, nom, calcule ou couleurs): ");
  fgets(type, DATA_SIZE, stdin);

  /* Remove the return line */
  type[strcspn(type, "\n")] = 0;

  envoie_recois_message(socketfd, type, argv[1]);

  /* Close the socket */
  close(socketfd);

} /* main */
