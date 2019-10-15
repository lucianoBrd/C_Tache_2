#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "json.h"

/* @brief
 * Creer un message au format json
 * a partir d'un objet message_json.
 *
 * @params
 * data : String ou l'on retourne le message JSON.
 * json : Objet contenant les différents paramètres à mettre dans le message.
 */
void create_message_json(
  char          *data,
  message_json  *json
){
  int i = 0;

  memset(data, 0, sizeof(data));
  strcpy(data, "{\"code\" : \"");
  strcat(data, json->code);
  strcat(data, "\", \"valeurs\" : [");
  for(i; i < json->nb_valeurs; i++){
    strcat(data, "\"");
    strcat(data, json->valeurs[i]);
    strcat(data, "\"");

    if(i < json->nb_valeurs - 1)
      strcat(data, ", ");

  }
  strcat(data, "]}");

} /* create_message_json */

/* @brief
 * Creer un objet message_json à partir d'un String
 * au format JSON.
 *
 * @params
 * message : Message JSON.
 *
 * @return
 * Un nouveau message_json alloué en mémoire.
 */
message_json *create_object_json(
  char *message
){
  int i 	  = 0,
      compt = 0,
      bool 	= 0;

  while(message[i] != '\0'){
    if( i > 7	 		              &&
        message[i]     == 's' 	&&
        message[i - 1] == 'r' 	&&
        message[i - 2] == 'u' 	&&
        message[i - 3] == 'e' 	&&
        message[i - 4] == 'l' 	&&
        message[i - 5] == 'a' 	&&
        message[i - 6] == 'v' 	){
      bool = 1;
      //Dont get the "
      i = i + 2;

    }

    if(bool != 0){
      if(message[i] == '"')
	     compt++;

    }

    i++;

  }

  message_json *json = new_message_json(compt);

  compt = compt / 2;
  i 	  = 0;
  bool  = 0;

  int   bool_string = 0,
        j           = 0;
  char  tmp[30];

  // get the code
  while(message[i] != '\0'){
    if( i > 4	 		              &&
        message[i]     == 'e' 	&&
        message[i - 1] == 'd' 	&&
        message[i - 2] == 'o' 	&&
        message[i - 3] == 'c' 	){
      bool  = 1;
      i     = i + 2;

    }

    if(bool != 0){
      if(message[i] == '"'){
	       if(bool_string == 0){
	          bool_string = 1;
	           i++;
	       } else {
	          bool_string  = 0;
	          tmp[j]       = '\0';
            strcpy(json->code, tmp);
	          break;

	       }

      }

      if(bool_string != 0){
	       tmp[j] = message[i];
	       j++;
      }

    }

    i++;

  }

  i 	        = 0;
  bool 	      = 0;
  bool_string = 0,
  j           = 0;
  compt       = 0;

  // get the valeurs
  while(message[i] != '\0'){
    if( i > 7	 		              &&
        message[i]     == 's' 	&&
        message[i - 1] == 'r' 	&&
        message[i - 2] == 'u' 	&&
        message[i - 3] == 'e' 	&&
        message[i - 4] == 'l' 	&&
        message[i - 5] == 'a' 	&&
        message[i - 6] == 'v' 	){
      //Dont get the "
      i     = i + 2;
      bool  = 1;

    }

    if(bool != 0){
      if(message[i] == '"'){
	       if(bool_string == 0){
	          bool_string = 1;
	          i++;
	       } else {
	          bool_string  = 0;
	          tmp[j]       = '\0';
	          strcpy(json->valeurs[compt], tmp);
	          compt++;
	          j = 0;

	       }

     }

     if(bool_string != 0){
	      tmp[j] = message[i];
	      j++;
     }

    }

    i++;

  }

  return json;

} /* create_object_json */

/* @brief
 * Creer un objet message_json avec n valeurs.
 *
 * @params
 * nb_valeurs : Nombre de valeurs de l'objet.
 *
 * @return
 * Un nouveau message_json alloué en mémoire.
 */
message_json *new_message_json(
  int nb_valeurs
){
  int           i     = 0;
  message_json  *json = calloc(1, sizeof(message_json));

  json->valeurs = calloc(10, sizeof(char **));

  for(i; i < nb_valeurs; i++)
    json->valeurs[i] = malloc(VALEURS_SIZE * sizeof(char));

  json->code        = malloc(10 * sizeof(char));
  json->nb_valeurs  = nb_valeurs;

  return json;

} /* new_message_json */

/* @brief
 * Supprimer un objet message_json.
 *
 * @params
 * json : Objet à supprimer.
 */
void delete_message_json(
  message_json *json
){
  int i = 0;
  free(json->code);
  for(i; i < json->nb_valeurs; i++)
    free(json->valeurs[i]);
  free(json->valeurs);
  free(json);

} /* delete_message_json */

/* @brief
 * Affiche un objet message_json.
 *
 * @params
 * json : Objet à afficher.
 */
void print_message_json(
  message_json *json
){
  int i = 0;
  printf("code : %s\n", json->code);
  printf("%d valeurs :\n", json->nb_valeurs);
  for(i; i < json->nb_valeurs; i++)
    printf("%s\n", json->valeurs[i]);

} /* print_message_json */
