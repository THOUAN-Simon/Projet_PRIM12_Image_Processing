#include <stdlib.h>
#include "pictures.h"
#include "lut.h"

//Remarque : dans toute la suite, on considère que "bien défini" signifie que les mémoire qui ont besoin d'être allouée l'on été.

/**
 *  @requires size est un unsigned int;
 *  @assigns rien;
 *  @ensures result est une LUT bien définie;
 */
lut create_lut(int size){
    lut result;
    result.s = size;
    result.values = malloc(sizeof(byte)*size);
    return result;
}

/**
 *  @requires l est un pointeur vers une LUT l bien définie;
 *  @assigns *l.values est libérée;
 *  @ensures la mémoire allouée à *l.values a bien été libérée;
 */
void clean_lut(lut* l){
    free((*l).values);
}

/**
 *  @requires l est une LUT bien définie et p une picture bien définie;
 *  @assigns chaque pixel de p vaut désormais l(p(i, j, k)) ; la lut l est libérée après utilisation;
 *  @ensures result est l'image p à laquelle on a appliqué la LUT l;
 */
picture apply_lut(lut l, picture p){
    //Create_picture suffit ici, car on a pas besoin de réutiliser les pixels d'origine de p (puisqu'on les modifie tous)
    picture result = create_picture(p.l, p.h, p.n_channel); 
    for(int i=0; i<(p.h*p.l*p.n_channel); i++){
        //Pour la justification de l'opération (p.pixels[i]*l.s)/256, voir le rapport, section 2.2
        result.pixels[i] = l.values[(p.pixels[i]*l.s)/256];
    }
    clean_lut(&l); //une fois appliquée, on n'a plus besoin de la lut
    return result;
}