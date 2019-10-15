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

  /* First put the code */
  strcpy(data, "{\"code\" : \"");
  /* Put the value of the code */
  strcat(data, json->code);

  /* Then put the values */
  strcat(data, "\", \"valeurs\" : [");
  for(i; i < json->nb_valeurs; i++){
    /* Put the value */
    strcat(data, "\"");
    strcat(data, json->valeurs[i]);
    strcat(data, "\"");

    /* Don't put , the last iteration */
    if(i < json->nb_valeurs - 1)
      strcat(data, ", ");

  } /* Foreach values */

  /* Close the message */
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
      bool  = 1;
      /* Don't get the first " */
      i     = i + 2;

    } /* When find valeurs */

    if(bool != 0){
      /* Count the number of " */
      if(message[i] == '"')
	     compt++;

    } /* When we are just after the valeurs */

    i++;

  } /* Loop on the message */

  /* There are 2 " for one value */
  compt = compt / 2;

  /* Create the object */
  message_json *json = new_message_json(compt);

  i 	  = 0;
  bool  = 0;

  int   bool_string = 0,
        j           = 0;
  char  tmp[30];

  /* Get the code */
  while(message[i] != '\0'){
    if( i > 4	 		              &&
        message[i]     == 'e' 	&&
        message[i - 1] == 'd' 	&&
        message[i - 2] == 'o' 	&&
        message[i - 3] == 'c' 	){
      bool  = 1;
      /* Don't get the first " */
      i     = i + 2;

    } /* When find code */

    if(bool != 0){
      if(message[i] == '"'){
	       if(bool_string == 0){
	          bool_string = 1;
	           i++;

	       } else {
           /* We finish to get the code */
	         bool_string  = 0;
	         tmp[j]       = '\0';
           strcpy(json->code, tmp);
	         break;

	       }

      } /* Know wich " is it */

      if(bool_string != 0){
	       tmp[j] = message[i];
	       j++;

      } /* Save the value of the code */

    } /* When we are just after code */

    i++;

  } /* Loop on the message */

  i 	        = 0;
  bool 	      = 0;
  bool_string = 0,
  j           = 0;
  compt       = 0;

  /* Get the valeurs */
  while(message[i] != '\0'){
    if( i > 7	 		              &&
        message[i]     == 's' 	&&
        message[i - 1] == 'r' 	&&
        message[i - 2] == 'u' 	&&
        message[i - 3] == 'e' 	&&
        message[i - 4] == 'l' 	&&
        message[i - 5] == 'a' 	&&
        message[i - 6] == 'v' 	){
      /* Don't get the first " */
      i     = i + 2;
      bool  = 1;

    } /* When find valeurs */

    if(bool != 0){
      if(message[i] == '"'){
	       if(bool_string == 0){
	          bool_string = 1;
	          i++;

	       } else {
           /* We finish to get the code */
	         bool_string  = 0;
	         tmp[j]       = '\0';
	         strcpy(json->valeurs[compt], tmp);
	         compt++;
	         j = 0;

	       }

     } /* Know wich " is it */

     if(bool_string != 0){
	      tmp[j] = message[i];
	      j++;

     } /* Save the value of the valeurs */

   } /* When we are just after valeurs */

    i++;

  } /* Loop on message */

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

  /* Allocate memory */
  json->valeurs = calloc(10, sizeof(char **));

  /* Foreach values */
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

  /* Desallocate memory */
  free(json->code);

  /* Foreach values */
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

  /* Print the message */
  printf("code : %s\n", json->code);
  printf("%d valeurs :\n", json->nb_valeurs);

  /* Foreach values */
  for(i; i < json->nb_valeurs; i++)
    printf("%s\n", json->valeurs[i]);

} /* print_message_json */
