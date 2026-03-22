# Image Processing Engine (PGM & PPM)Projet de Programmation Impérative en C

## Présentation
L'objectif de ce projet est de créer une structure de données permettant la manipulation d'images aux formats PGM (niveaux de gris) et PPM (couleurs). Le moteur permet d'effectuer des opérations allant de la simple lecture/écriture binaire à des transformations algorithmiques avancées.

## Fonctionnalités Implémentées
Le programme est structuré en modules pour séparer la logique de bas niveau du traitement d'image :
- Module Pixels : Fonctions de lecture et d'écriture d'un pixel unique pour faciliter la manipulation par coordonnées $(i, j)$.
- Module LUT (Look Up Table) : Création, nettoyage et application de tables de correspondance pour optimiser les calculs de couleurs.
- Traitement & Algorithmique :
 - Redimensionnement : Algorithmes Nearest Neighbor et Bilinear (gestion de l'interpolation).
 - Dynamique : Maximisation de la dynamique et application de fonctions de transfert.
 - Effets spéciaux : Inversion, mélange d'images (mix), effet de "fonte" (melt) et séparation de canaux.


## Défis Techniques & Apprentissages
Ce projet en C a nécessité une gestion rigoureuse des ressources système :
- E/S Binaires : Utilisation de fseek et du mode rb/wb pour manipuler les fichiers binaires tout en ignorant les commentaires dans les headers.
- Gestion Mémoire : Implémentation de nettoyages spécifiques (notamment pour les tableaux de LUT) afin d'éviter les fuites mémoires.
- Précision des Types : Utilisation du type unsigned char pour garantir des valeurs de pixels entre $0$ et $255$ et éviter les bruits visuels.

## Compilation
Le projet inclut un Makefile pour la compilation séparée avec l'usage de la commande make all
