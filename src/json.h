#ifndef __JSON_H__
#define __JSON_H__

/* Size of a valeur of a message_json */
#define VALEURS_SIZE 100

/* Size of a string who contain a JSON message */
#define DATA_SIZE 1024

/* Object contenant les différents paramètres pour creer message JSON */
typedef struct {
  char *code;
  char **valeurs;
  int nb_valeurs;
} message_json;

/* @brief
 * Creer un message au format JSON
 * a partir d'un objet message_json.
 */
void create_message_json(
  char          *data,
  message_json  *json
);

/* @brief
 * Creer un objet message_json à partir d'un String
 * au format JSON.
 */
message_json *create_object_json(
  char *message
);

/* @brief
 * Creer un objet message_json avec n valeurs.
 */
message_json *new_message_json(
  int nb_valeurs
);

/* @brief
 * Supprimer un objet message_json.
 */
void delete_message_json(
  message_json *json
);

/* @brief
 * Affiche un objet message_json.
 */
void print_message_json(
  message_json *json
);

#endif
