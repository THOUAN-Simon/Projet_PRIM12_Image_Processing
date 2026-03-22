#include <stdio.h>
#include "pictures.h"
#include "pixels.h"
#define MAX_BYTE 255

//Remarque : dans toute la suite, on considère que "bien défini" signifie que les mémoire qui ont besoin d'être allouée l'on été.

/**
 *  @requires p est une picture bien définie, i est strictement plus petit que p.h,
 *  j est strictmeent plus petit que p.l et k est strictmeent plus petit que p.n_channel
 *  @assigns p(i, j ,k) reçoit la valeur pixel_content;
 *  @ensures p(i, j ,k) vaut désormais pixel_content;
 */
int write_pixel(picture p, int i, int j, int k, byte pixel_content){
    if(pixel_content <= MAX_BYTE){
        p.pixels[((i*p.l+j)*p.n_channel)+k] = pixel_content;
        return 0;
    }
    else{
        printf("pixel_content > MAX_BYTE \n");
        return 1;
    }
}

/**
 *  @requires p est une picture bien définie, i est strictement plus petit que p.h,
 *  j est strictmeent plus petit que p.l et k est strictmeent plus petit que p.n_channel
 *  @assigns rien;
 *  @ensures la valeur de retour est p(i, j ,k);
 */
byte read_pixel(picture p, int i, int j, int k){
    byte result;
    result = p.pixels[((i*p.l+j)*p.n_channel)+k];
    return result;
}