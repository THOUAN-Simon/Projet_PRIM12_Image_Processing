/*
 * pictures.h
 * Contient le type picture, le type byte, et toues les fonctions 
 * exigés ainsi que le premier bonus
 */

#ifndef _PICTURES_H
#define _PICTURES_H
#include <stdio.h>

//Remarque : dans toute la suite, on considère que "bien défini" signifie que les mémoire qui ont besoin d'être allouée l'on été.

/**
 * définition du type byte. On utilise unsigned char pour que byte 
 * appartienne à l'intervalle [0,255]
 */
typedef unsigned char byte;

/**
 * définition du type picture
 *  - h : height : correspond à la hauteur de l'image
 *  - l : length : correspond à la largeur de l'image
 *  - n_channel : correspond au nombre de canaux de l'image : vaut 1 
 *  si l'image est en niveau de gris, 3 si l'image est en couleurs 
 *  (1 composante par couleur primaire)
 */
struct type_picture{
    unsigned int h;
    unsigned int l;
    unsigned int n_channel; 
    byte* pixels;
};

typedef struct type_picture picture;

/**
 * Lecture d'une image
 * @param [in] filename le chemin absolu vers le fichier à lire
 * @return une structure picture bien définie qui contient les informations
 * présentes dans le fichier qui se trouve à l'adresse filename
 */
picture read_picture(char* filename);

/**
 * Écriture d'une image
 * @param [in] p une picture p bien définie
 * @param [in] filename le chemin absolu vers le fichier à écrire
 * @return 0 si il n'y a pas eu de problème lors de l'écriture, un entier 
 * strictement positif sinon
 */
int write_picture(picture p, char* filename);

/**
 * création d'une nouvelle picture
 * @param [in] width un entier non signé correspondant à la largeur de l'image
 * @param [in] height un entier non signé correspondant à la hauteur de l'image
 * @param [in] channels un entier non signé correspondant au nombre de canaux de l'image
 * @return une picture p bien définie qui vérifie p.l = width, p.h = height et p.n_channel = channels
 */
picture create_picture(unsigned int width, unsigned int height, unsigned int channels);

/**
 * Nettoyage d'une picture
 * @param [in] p un pointeur vers une picture p bien définie
 * @return rien
 */
void clean_picture(picture* p);

/**
 * Copie d'une picture
 * @param [in] p une picture bien définie
 * @return p_new nouvelle picture bien définie contenant une copie du 
 * tableau p.values
 */
picture copy_picture(picture p);

/**
 * Test si p est_vide
 * @param [in] p une picture bien définie
 * @return 1 si p est vide, 0 sinon
 */
int is_empty_picture(picture p);

/**
 * Test si p est une image en niveau de gris
 * @param [in] p une picture bien définie
 * @return 1 si p est une image en niveau de gris, 0 sinon
 */
int is_gray_picture(picture p);

/**
 * Test p est une image en couleurs
 * @param [in] p une picture bien définie
 * @return 1 si p est une image en couleurs, 0 sinon
 */
int is_color_picture(picture p);

/**
 * Affiche les informations de p
 * @param [in] p une picture bien définie
 * @return rien
 */
void info_picture(picture p);

/**
 * Convertit p en une image en couleurs
 * @param [in] p une picture bien définie
 * @return une nouvelle picture bien définie convertie en couleur si 
 * p était en niveau de gris, une copie de p si p était déjà 
 * en couleurs
 */
picture convert_to_color_picture(picture p);

/**
 * Convertit p en une image en niveau de gris
 * @param [in] p une picture bien définie
 * @return une nouvelle picture bien définie convertie en niveau de gris si 
 * p était en couleurs, une copie de p si p était déjà 
 * en niveau de gris
 */
picture convert_to_gray_picture(picture p);

/**
 * Décompose p en 3 images en niveau de gris correspondant 
 * au rouge, au vert et au bleu
 * @param [in] p une picture bien définie
 * @return un tableau de pictures bien définies contenant dans l'ordre, 
 * la composante rouge, verte et bleue
 */
picture* split_picture(picture p);

/**
 * Fabrique une nouvelle image en couleurs à partir des 
 * composantes rouge, verte et bleue passée en argument
 * @param [in] red la composante en niveau de gris correspondant au rouge
 * @param [in] green la composante en niveau de gris correspondant au vert
 * @param [in] blue la composante en niveau de gris correspondant au bleue
 * @return une nouvelle picture bien définie en couleur composée à partir
 *  des trois composantes en niveau de gris passées en argument
 */
picture merge_picture(picture red, picture green, picture blue);

/**
 * Éclaircit p d'un facteur factor
 * @param [in] p la picture à éclaircir, bien définie
 * @param [in] factor un flottant de type double
 * @return une nouvelle picture bien définie eclaircie d'un facteur factor
 */
picture brighten_picture(picture p, double factor);

/**
 * Fait fondre l'image p
 * @param [in] p la picture à faire fondre, bien définie
 * @param [in] number un entier non signé
 * @return une nouvelle picture bien définie dont on a fait "fondre" 
 * number pixels
 */
picture melt_picture(picture p, int number);

/**
 * inverser l'image p
 * @param [in] p la picture à inverser, bien définie
 * @return une nouvelle picture bien définie qui est l'inverse de p
 */
picture inverse_picture(picture p);

/**
 * maximise la dynamique de l'image p
 * @param [in] p la picture à normaliser, bien définie
 * @return une nouvelle picture bien définie qui correspond à p normalisée
 */
picture normalize_dynamic_picture(picture p);

/**
 * Réduction du nombre de pixels de l'image p
 * @param [in] p la picture à réduire, bien définie
 * @return une nouvelle picture bien définie qui correspond à p dont on a 
 * réduit le nombre de pixels
 */
picture set_levels_picture(picture p, byte nb_levels);

/**
 * Différence absolue entre p1 et p2
 * @param [in] p1 picture bien définie
 * @param [in] p2 picture bien définie
 * @return une nouvelle picture bien définie qui correspond à |p1-p2|
 */
picture diff_picture(picture p1, picture p2);

/**
 * Produit entre p1 et p2
 * @param [in] p1 picture bien définie
 * @param [in] p2 picture bien définie
 * @return une nouvelle picture bien définie qui correspond à p1*p2
 */
picture mult_picture(picture p1, picture p2);

/**
 * calcule la mixture entre p1 et p2 selon p3
 * @param [in] p1 picture bien définie
 * @param [in] p2 picture bien définie
 * @param [in] p3 picture bien définie
 * @return une nouvelle picture bien définie qui correspond à la mixture
 * de p1 et p2 selon p3 (voir énoncé pour une meilleure description)
 */
picture mix_picture(picture p1, picture p2, picture p3);

/**
 * Re-dimensionne image aux nouvelles dimensions width et height 
 * @param [in] width entier non signé correspondant à la nouvelle largeur
 * @param [in] height entier non signé correspondant à la nouvelle hauteur
 * @return une nouvelle picture p bien définie redimensionée en utilisant 
 * l'algorithme des plus proche voisins et vérifiant p.l = width et p.h = height
 */
picture resample_picture_nearest(picture image, unsigned int width, unsigned int height);

/**
 * Re-dimensionne image aux nouvelles dimensions width et height
 * @param [in] width entier non signé correspondant à la nouvelle largeur
 * @param [in] height entier non signé correspondant à la nouvelle hauteur
 * @return une nouvelle picture p bien définie redimensionée en utilisant 
 * l'algorithme d'interpolation bilinéaire et vérifiant p.l = width et p.h = height
 */
picture resample_picture_bilinear(picture image, unsigned int width, unsigned int height);

/* BONUS */

/**
 * Éclaircit p d'un facteur factor
 * @param [in] p la picture à éclaircir, bien définie
 * @param [in] factor un flottant de type double
 * @return une nouvelle picture bien définie eclaircie d'un facteur 
 * factor en utilisant une LUT
 */
picture brighten_picture_lut(picture p, double factor);

#endif