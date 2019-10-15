#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "serveur.h"
#include "json.h"

int plot(
  int 	       client_socket_fd,
  message_json *json
){
  //Extraire le compteur et les couleurs RGB
  FILE          *p  = popen("gnuplot -persist", "w"),
                *fp = fopen("save.txt", "a+");
  int           i   = 0,
                data_size;
  char          save[DATA_SIZE],
                *tmp;
  message_json  *json_save;

  fprintf(p, "set xrange [-15:15]\n");
  fprintf(p, "set yrange [-15:15]\n");
  fprintf(p, "set style fill transparent solid 0.9 noborder\n");
  fprintf(p, "set title 'Top %d colors'\n", json->nb_valeurs);
  fprintf(p, "plot '-' with circles lc rgbcolor variable\n");

  for(i; i < json->nb_valeurs; i++){
      tmp = json->valeurs[i];
      tmp++;

      // Le numéro : 360° (cercle) / num couleurs
      fprintf(p, "0 0 10 %lf %lf 0x%s\n", (float)(i)*(float)(360/(float)json->nb_valeurs), (float)(i + 1)*(float)(360/(float)json->nb_valeurs), tmp);

      /* Put the color into the file */
      fputs(json->valeurs[i], fp);
      fputs("\n", fp);

  }

  fprintf(p, "e\n");
  pclose(p);
  fclose(fp);
  json_save = new_message_json(1);

  strcpy(json_save->code, "couleurs");
  strcpy(json_save->valeurs[0], "Couleurs enregistrées");

  create_message_json(save, json_save);
  delete_message_json(json_save);

  data_size = write (client_socket_fd, (void *) save, strlen(save));

  if(data_size < 0){
    perror("erreur ecriture");
    return(EXIT_FAILURE);

  } /* Error write */

}

/* renvoyer un message (*data) au client (client_socket_fd)
 */
int renvoie_message(
  int           client_socket_fd,
  message_json  *json
){
  char  data[DATA_SIZE];
  int   data_size;

  create_message_json(data, json);
  data_size = write (client_socket_fd, (void *) data, strlen(data));

  if(data_size < 0){
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
){
  int data_size = write (client_socket_fd, (void *) data, strlen(data));

  if(data_size < 0){
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
  int 	       client_socket_fd,
  message_json *json
)
{
  int 	        data_size;
  char 	        *operation    = json->valeurs[0],
                save[DATA_SIZE];
  float         result        = 0.0,
                number1       = atof(json->valeurs[1]),
                number2       = atof(json->valeurs[2]);
  message_json  *json_save;

  if(strcmp(operation, "+") == 0.0){
    /* Case + */
    result = number1 + number2;

  } else if(strcmp(operation, "-") == 0.0){
    /* Case - */
    result = number1 - number2;

  } else if(strcmp(operation, "*") == 0.0){
    /* Case * */
    result = number1 * number2;

  } else if(strcmp(operation, "/") == 0.0){
    /* Case / */
    result = number1 / number2;

  } else {
    /* Case error */
    result = -1.0;

  }

  json_save = new_message_json(1);
  strcpy(json_save->code, "calcule");

  if (  strstr(json->valeurs[1], ".") != NULL ||
        strstr(json->valeurs[2], ".") != NULL ){
    /* Number float */
    printf("Le resultat du %s vaut %.2f\n", json->code, result);
    sprintf(json_save->valeurs[0], "%.2f", result);

  } else {
    /* Number int */
    printf("Le resultat du %s vaut %d\n", json->code, (int)result);
    sprintf(json_save->valeurs[0], "%d", (int)result);

  }

  create_message_json(save, json_save);
  delete_message_json(json_save);

  data_size = write (client_socket_fd, (void *) save, strlen(save));

  if(data_size < 0){
    perror("erreur ecriture");
    return(EXIT_FAILURE);

  } /* Error write */

} /* recois_numero_calcule */

/* accepter la nouvelle connection d'un client et lire les données
 * envoyées par le client. En suite, le serveur envoie un message
 * en retour
 */
int recois_envoie_message(
  int socketfd
){
  struct sockaddr_in client_addr;

  char          data[DATA_SIZE];
  int           client_addr_len = sizeof(client_addr),
                client_socket_fd,
                data_size;
  message_json  *json;

  // nouvelle connection de client
  client_socket_fd = accept(socketfd, (struct sockaddr *) &client_addr, &client_addr_len);
  if(client_socket_fd < 0 ){
    perror("accept");
    return(EXIT_FAILURE);

  }

  memset(data, 0, sizeof(data));

  //lecture de données envoyées par un client
  data_size = read (client_socket_fd, (void *) data, sizeof(data));

  if(data_size < 0){
    perror("erreur lecture");
    return(EXIT_FAILURE);

  }

  printf("Message recu :\n");
  json = create_object_json(data);
  print_message_json(json);

  if(strcmp(json->code, "message") == 0){
    /* Si le message commence par le mot: 'message:' */
    message_json *json_return = new_message_json(1);
    printf("Votre message (max 100 caracteres): ");
    fgets(json_return->valeurs[0], VALEURS_SIZE, stdin);

    strcpy(json_return->code, "message");

    renvoie_message(client_socket_fd, json_return);

  } else if(strcmp(json->code, "nom") == 0){
    /* Si le message commence par le mot: 'nom:' */
    renvoie_nom_client(client_socket_fd, data);

  } else if(strcmp(json->code, "calcule") == 0){
    /* Si le message commence par le mot: 'calcule:' */
    recois_numero_calcule(client_socket_fd, json);

  } else if(strcmp(json->code, "couleurs") == 0){
    /* Si le message commence par le mot: 'couleurs:' */
    /* Plot the image */
    plot(client_socket_fd, json);

  }
  delete_message_json(json);
  //fermer le socket
  close(socketfd);

}

int main() {

  int socketfd,
      bind_status,
      client_addr_len,
      option = 1;

  struct sockaddr_in server_addr, client_addr;

  /*
   * Creation d'un socket
   */
  socketfd = socket(AF_INET, SOCK_STREAM, 0);
  if(socketfd < 0){
    perror("Unable to open a socket");
    return -1;

  }

  setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

  //détails du serveur (adresse et port)
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family      = AF_INET;
  server_addr.sin_port        = htons(PORT);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  bind_status = bind(socketfd, (struct sockaddr *) &server_addr, sizeof(server_addr));
  if(bind_status < 0){
    perror("bind");
    return(EXIT_FAILURE);

  }

  listen(socketfd, 10);
  recois_envoie_message(socketfd);

  return 0;

}
