/*
 * lut.h
 * Contient la définition d'une LUT (Look Up Table)
 * et des fonctions pour en créer, les appliquer 
 * a des images et 
 * libérer la mémoire qui leur a été allouée
 */

#ifndef LUT_H
#define LUT_H
#include "pictures.h"

/**
 * définition du type lut
 *  - s : taille de la lut (correspond à la taille du tableau qui contiendra les valeurs de la LUT)
 *  - values : tableau qui contient les valeurs de la LUT : values[i] correspond à lut(i)
 */
struct type_lut{
    unsigned int s;
    byte* values;
};

typedef struct type_lut lut;

/**
 * Création d'une LUT
 * @param [in] size la taille de la LUT
 * @return la LUT effectivement crée, 
 * de taille size et où la mémoire utilisée par le  tableau "values" a été allouée
 */
lut create_lut(int size);

/**
 * nettoyage d'une LUT
 * @param [in] l pointeur vers la LUT à nettoyer
 * @return rien
 *
 */
void clean_lut(lut* l);

/**
 * Application d'une LUT à une image
 * @param [in] l la LUT à appliquer
 * @param [in] p l'image à modifier
 * @return l'image p à laquelle on a appliqué la LUT l
 * 
 */
picture apply_lut(lut l, picture p);

#endif