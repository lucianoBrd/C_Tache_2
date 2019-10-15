# Rapport d’auto-évaluation tache 2

Lien du git : [Projet](https://github.com/lucianoBrd/C_Tache_2)

## Objectif: Quel est l’objectif de votre projet?

L'objectif de ce projet est de créer un environnement client-serveur pour l'analyse et la
gestion des images et de leurs métadonnées.
Dans la [Tache1](https://github.com/lucianoBrd/C_Tache_1), nous avons géré :
  * L'envoie du nom du client et le serveur renvoie le même nom, en guise d'accusé
   de réception.
  * L'envoie d'un simple message envoyé par le client, auquel le serveur renvoie une
   réponse.
  * Le client envoie deux numéros et un opérateur mathématique et le serveur
   répond le résultat de l'opération.
  * Le client envoie N couleurs et le serveur les enregistre dans un fichier.
  
Dans la tache 2, nous avons géré :
  * L'envoie et la réception de message entre le client et le serveur au format JSON.
  * Nous avons utilisé le format d'image bmp.

## Fichiers: Noms de fichiers

Il y a 2 fichiers pour la partie client :
```
client.c
client.h
```

Il y a 2 fichiers pour la partie serveur :
```
serveur.c
serveur.h
```

Il y a 2 fichiers pour la partie message JSON :
```
json.c
json.h
```
Nous avons créer un nouveau type de variable : message_json.
Cette structure se compose d'un code, d'un tableau de valeurs et de la taille de ce tableau.
Cela nous permet de :
  * Creer un message au format JSON à partir d'un objet message_json.
  * Creer un objet message_json à partir d'un String.
  * Creer un objet message_json avec n valeurs.
  * Supprimer un objet message_json.
  * Affiche un objet message_json.

Il y a 2 fichiers pour la partie image BPM :
```
bpm.c
bpm.h
```

Il y a 2 fichiers pour la partie couleur :
```
couleur.c
couleur.h
```

Il y a 1 fichier pour compiler le projet :
```
Makefile
```

Il y a 1 fichier pour de sauvegarde des couleurs (optionnel) :
```
save.txt
```

## Bibliothèques: les bibliothèques standards

Dans ce projet, nous utilisons plusieurs bibliothèques standards :
  * string.h
  * stdio.h
  * stdlib.h
  * unistd.h
  * sys/types.h
  * sys/socket.h
  * netinet/in.h
  * sys/epoll.h

## Références: les URLs, les numéros de groupes

##  Difficulté: niveau de difficulté (facile, moyenne, difficile)

Le plus délicat fut de trouver une structure permettant d'externaliser
la gestion des messages JSON. De plus, la gestion de la mémoire pour
allouer ou libérer de la mémoire pour notre structure était obligatoire.
La difficulté était donc moyenne.

## Commentaires (optionnels): remarques etc.
