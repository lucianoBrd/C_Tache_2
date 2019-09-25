#ifndef __CLIENT_H__
#define __CLIENT_H__

#define PORT 8089

/* envoi et reception de message
 * envoi et reception de nom du client
 * envoi de l'opération et reception du resultat
 * envoi de couleurs et reception de confirmation
 */
int envoie_recois_message(int socketfd, char *type);

/* envoi de couleurs et reception de confirmation
 */
int envoie_couleurs_simple(int socketfd);

#endif
