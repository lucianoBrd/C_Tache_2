#ifndef __SERVER_H__
#define __SERVER_H__

#include "json.h"

#define PORT 8089

int plot(
  int 	       client_socket_fd,
  message_json *json
);

int renvoie_message(
  int           client_socket_fd,
  message_json  *json
);

int renvoie_nom_client(
  int 	client_socket_fd,
  char 	*data
);

int recois_numero_calcule(
  int 	       client_socket_fd,
  message_json *json
);

int recois_envoie_message(
  int socketfd
);



#endif
