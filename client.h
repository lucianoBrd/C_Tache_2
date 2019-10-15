#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "bmp.h"
#include "json.h"

/* Port we use to communicate */
#define PORT 8089

/* @brief
 * envoi et reception de message
 * envoi et reception de nom du client
 * envoi de l'opération et reception du resultat
 * envoi de couleurs et reception de confirmation
 */
int envoie_recois_message(
  int 	socketfd,
  char 	*type,
  char  *pathname
);

/* @brief
 * Analyse une image. Demande à l'utilisateur
 * combien de couleurs il veut récupérer, puis,
 * on envoie au serveur ces couleurs.
 */
void analyse(
  char *pathname,
  char *data
);

/* @brief
 * Envoie au serveur n couleurs.
 */
int envoie_couleurs(
  int   socketfd,
  char  *pathname
);

#endif
