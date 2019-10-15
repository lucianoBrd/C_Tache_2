#ifndef __JSON_H__
#define __JSON_H__

#define VALEURS_SIZE 100
#define DATA_SIZE 1024
//Structure de données d'un message json
typedef struct {
  char *code;
  char **valeurs;
  int nb_valeurs;
} message_json;

void create_message_json(char *data, message_json *json);
message_json *new_message_json(int nb_valeurs);
void delete_message_json(message_json *json);
message_json *create_object_json(char *message);
void print_message_json(message_json *json);

#endif
