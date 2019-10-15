#ifndef __SERVER_H__
#define __SERVER_H__

#include "json.h"

/* Port de communication avec le client */
#define PORT 8089

/* @brief
 * Affiche les couleurs sous forme de graphique.
 */
int plot(
  int 	       client_socket_fd,
  message_json *json
);

/* @brief
 * Renvoie un message JSON au client.
 */
int renvoie_message(
  int           client_socket_fd,
  message_json  *json
);

/* @brief
 * Renvoie un message JSON avec le nom du client.
 */
int renvoie_nom_client(
  int 	client_socket_fd,
  char 	*data
);

/* @brief
 * Renvoie le resultat du calcule au client.
 */
int recois_numero_calcule(
  int 	       client_socket_fd,
  message_json *json
);

/* @brief
 * Accepter la nouvelle connection d'un client et lire les données
 * envoyées par le client. En suite, le serveur envoie un message
 * en retour.
 */
int recois_envoie_message(
  int socketfd
);



#endif
