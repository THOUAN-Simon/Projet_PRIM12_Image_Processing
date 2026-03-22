/*
 * pixels.h
 * Contient les fonctions de lecture et d'écriture
 * d'un pixel à une position donnée dans une image donnée.
 * Les fonctions opèrent via la ligne, colonne et composante 
 * du pixel qu'on cherche à lire ou écrire
 */

#ifndef PIXELS_H
#define PIXELS_H
#include "pictures.h"

/**
 * Écriture d'un pixel
 * @param [in] p image dans laquelle on veut écrire le nouveau pixel
 * @param [in] i ligne du pixel à écrire
 * @param [in] j colonne du pixel à écrire
 * @param [in] k composante du pixel à écrire
 * @param [in] pixel_content valeur du pixel à écrire
 * @return p, où p(i, j, k) vaut désormais pixel_content
 */
int write_pixel(picture p, int i, int j, int k, byte pixel_content);

/**
 * Lecture d'un pixel
 * @param [in] p image dans laquelle on veut lire le pixel
 * @param [in] i ligne du pixel à lire
 * @param [in] j colonne du pixel à lire
 * @param [in] k composante du pixel à lire
 * @return la valeur du pixel lue, soit p(i, j ,k)
 */
byte read_pixel(picture p, int i, int j, int k);

#endif