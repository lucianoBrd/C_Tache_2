/* Fichier: serveur.c
 * Communication client-serveur
 * Auteurs: John Samuel, ...
 */


#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "serveur.h"

void plot(char *data) {

  //Extraire le compteur et les couleurs RGB 
  FILE *p = popen("gnuplot -persist", "w");
  printf("Plot");
  int count = 0;
  int n;
  char *saveptr = NULL;
  char *str = data;
  fprintf(p, "set xrange [-15:15]\n");
  fprintf(p, "set yrange [-15:15]\n");
  fprintf(p, "set style fill transparent solid 0.9 noborder\n");
  fprintf(p, "set title 'Top 10 colors'\n");
  fprintf(p, "plot '-' with circles lc rgbcolor variable\n");
  while(1) {
    char *token = strtok_r(str, ",", &saveptr);
    if (token == NULL) {
      break;
    }
    str=NULL;
    if (count == 0) {
      n = atoi(token);
    }
    else {
      // Le numéro 36, parceque 360° (cercle) / 10 couleurs = 36
      fprintf(p, "0 0 10 %d %d 0x%s\n", (count-1)*36, count*36, token+1);
    }
    count++;
  }
  fprintf(p, "e\n");
  printf("Plot: FIN\n");
  pclose(p);
}

/* renvoyer un message (*data) au client (client_socket_fd)
 */
int renvoie_message(int client_socket_fd, char *data) {
  int data_size = write (client_socket_fd, (void *) data, strlen(data));
      
  if (data_size < 0) {
    perror("erreur ecriture");
    return(EXIT_FAILURE);
  }
}

/* @brief
 * renvoyer un message du nom (*data) au client (client_socket_fd)
 * 
 * @params
 * client_socket_fd : Socket du client.
 * data : String contenant le message du client (nom).
 * 
 * @return
 * EXIT_FAILURE en cas d'erreur.
 */
int renvoie_nom_client(
  int 	client_socket_fd, 
  char 	*data
) {
  int data_size = write (client_socket_fd, (void *) data, strlen(data));
      
  if (data_size < 0) {
    perror("erreur ecriture");
    return(EXIT_FAILURE);
    
  } /* Error write */
  
} /* renvoie_nom_client */

/* @brief
 * renvoyer le resultat du calcul (*data) au client (client_socket_fd)
 * 
 * @params
 * client_socket_fd : Socket du client.
 * data : String contenant le message du client (opérateur et nombres).
 * 
 * @return
 * EXIT_FAILURE en cas d'erreur.
 */
int recois_numero_calcule(
  int 	client_socket_fd, 
  char 	*data
) 
{
  int 	data_size;
  char 	calcule[100], 
	str[12], 
	code[10], 
	operation[5], 
	number1[20], 
	number2[20];
  
  /* Get different string in data */
  sscanf(data, "%s %s %s %s", code, operation, number1, number2);
  
  if (strstr(number1, ".") != NULL || strstr(number2, ".") != NULL) {
    /* Number float */
    float result = 0.0;
    if(strcmp(operation, "+") == 0.0){
      /* Case + */
      result = atof(number1) + atof(number2);
      
    } else if(strcmp(operation, "-") == 0.0){
      /* Case - */
      result = atof(number1) - atof(number2);
      
    } else if(strcmp(operation, "*") == 0.0){
      /* Case * */
      result = atof(number1) * atof(number2);
      
    } else if(strcmp(operation, "/") == 0.0){
      /* Case / */
      result = atof(number1) / atof(number2);
      
    } else {
      /* Case error */
      result = -1.0;
      
    }
    sprintf(str, "%.2f", result);
    
  } else {
    /* Number int */
    int result = 0;
    if(strcmp(operation, "+") == 0){
      /* Case + */
      result = atoi(number1) + atoi(number2);
      
    } else if(strcmp(operation, "-") == 0){
      /* Case - */
      result = atoi(number1) - atoi(number2);
      
    } else if(strcmp(operation, "*") == 0){
      /* Case * */
      result = atoi(number1) * atoi(number2);
      
    } else if(strcmp(operation, "/") == 0){
      /* Case / */
      result = atoi(number1) / atoi(number2);
      
    } else {
      /* Case error */
      result = -1;
      
    }
    sprintf(str, "%d", result);
  
  }

  strcpy(calcule, "calcule: ");
  strcat(calcule, str);
    
  data_size = write (client_socket_fd, (void *) calcule, strlen(calcule));
      
  if (data_size < 0) {
    perror("erreur ecriture");
    return(EXIT_FAILURE);
    
  } /* Error write */
  
} /* recois_numero_calcule */


/* @brief
 * envoyer la confirmation d'enregistrement des couleurs (*data) 
 * au client (client_socket_fd)
 * 
 * @params
 * client_socket_fd : Socket du client.
 * data : String contenant le message du client (nombre de couleurs et les couleurs).
 * 
 * @return
 * EXIT_FAILURE en cas d'erreur.
 */
int recois_couleurs(
  int 	client_socket_fd, 
  char 	*data
) 
{
  char 	code[10], 
	save[20], 
	str[10],
	number_c[10];
  int 	i,
	n;
  FILE 	*fp;

  /* Get number of colors */
  sscanf(data, "%s %s", code, number_c);
  n = atoi(number_c);
  
  /* Open the save file */
  fp = fopen("save.txt", "a+");
  
  /* Go to the first color */
  while(*data != ','){
    data++;
    
  } /* While we dont find a "," */
  data++;
  data++;
  
  for(i = 0; i < n; i++){
    int j = 0;
    while(*data != ',' && *data != '\0'){
      if(j <= 6){
	str[j] = *data;
	
      } /* We save just 7 char because a color has 7 char */
      data++;
      j++;
    
    } /* While we dont find a "," or "\0" */
    if(i < n-1){
      data++;
      data++;
      
    } /* If the last we dont increment */
    
    /* Put the color into the file */
    fputs(str, fp);
    fputs("\n", fp);
    
  } /* For each color */
  
  fclose(fp);
  strcpy(save, "couleurs: enregistré");
    
  int data_size = write (client_socket_fd, (void *) save, strlen(save));
      
  if (data_size < 0) {
    perror("erreur ecriture");
    return(EXIT_FAILURE);
    
  } /* Error write */
  
} /* recois_couleurs */

/* accepter la nouvelle connection d'un client et lire les données
 * envoyées par le client. En suite, le serveur envoie un message
 * en retour
 */
int recois_envoie_message(int socketfd) {
  struct sockaddr_in client_addr;
  char data[1024];

  int client_addr_len = sizeof(client_addr);
 
  // nouvelle connection de client
  int client_socket_fd = accept(socketfd, (struct sockaddr *) &client_addr, &client_addr_len);
  if (client_socket_fd < 0 ) {
    perror("accept");
    return(EXIT_FAILURE);
  }

  memset(data, 0, sizeof(data));

  //lecture de données envoyées par un client
  int data_size = read (client_socket_fd, (void *) data, sizeof(data));
      
  if (data_size < 0) {
    perror("erreur lecture");
    return(EXIT_FAILURE);
  }
  
  printf ("Message recu: %s\n", data);
  char code[10];
  sscanf(data, "%s:", code);
  
  if (strcmp(code, "message:") == 0) {
    /* Si le message commence par le mot: 'message:' */
    char message[100];
    printf("Votre message (max 1000 caracteres): ");
    fgets(message, 1024, stdin);
    strcpy(data, "message: ");
    strcat(data, message);
    
    renvoie_message(client_socket_fd, message);
    
  } else if(strcmp(code, "nom:") == 0){
    /* Si le message commence par le mot: 'nom:' */
    renvoie_nom_client(client_socket_fd, data);
    
  } else if(strcmp(code, "calcule:") == 0){
    /* Si le message commence par le mot: 'calcule:' */
    recois_numero_calcule(client_socket_fd, data);
    
  } else if(strcmp(code, "couleurs:") == 0){
    /* Si le message commence par le mot: 'couleurs:' */
    recois_couleurs(client_socket_fd, data);
    
  } else {
    /* Plot the image */
    plot(data);
    
  }

  //fermer le socket 
  close(socketfd);
}

int main() {

  int socketfd;
  int bind_status;
  int client_addr_len;

  struct sockaddr_in server_addr, client_addr;

  /*
   * Creation d'un socket
   */
  socketfd = socket(AF_INET, SOCK_STREAM, 0);
  if ( socketfd < 0 ) {
    perror("Unable to open a socket");
    return -1;
  }

  int option = 1;
  setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

  //détails du serveur (adresse et port)
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  bind_status = bind(socketfd, (struct sockaddr *) &server_addr, sizeof(server_addr));
  if (bind_status < 0 ) {
    perror("bind");
    return(EXIT_FAILURE);
  }
 
  listen(socketfd, 10);
  recois_envoie_message(socketfd);

  return 0;
}
