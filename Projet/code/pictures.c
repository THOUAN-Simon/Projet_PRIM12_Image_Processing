#include <stdio.h>
#include <stdlib.h>
#include <errno.h> //For perror
#include <string.h> //For strcmp
#include "filename.h"
#include "lut.h"
#include "pixels.h"
#include "pictures.h"

//Remarque : dans toute la suite, on considère que "bien défini" signifie que les mémoire qui ont besoin d'être allouée l'on été.

//Fonction utile dans read_picture, pour connaitre le nombre de caractères lus : 

/**
 *  @requires int est un entier non signé
 *  @assigns n décrémente de 10 à chaque itératio nde la boucle while
 *  @ensures result est le nombre de chiffres de n
 */
int digit_number(int n){
    int result = 1;
    if (n <= 9){
        return result;
    }
    else{
        while(n/10 >= 1){
            n = n/10;
            ++result;
        }
    }
    return result;
}

//Fonction servant à éviter les commentaires des fichiers .pgm ou .ppm, et met à jour la variable de la fonction read_picture qui permet de se placer
//juste avant les données binaires avant la lecture de celles-ci.

/**
 *  @requires file un pointeur vers un descripteur de fichier, counter un pointeur vers un entier bien défini
 *  @assigns ajoute à counter 1 si on lit un nouveau caractère
 *  @ensures counter est incrémenté du nombre de caractère lu au total, le curseur dans file a avancé si on a sauté des commentaires
 */
void skip_comments(FILE *file, int* counter){
    char first_char;
    char current_char;
    if(fseek(file, 1, SEEK_CUR) != 0){ 
        perror("[skip_comments] : can't move the cursor to the beginning of the commentary line");
        exit(1);
    }
    if(fread(&first_char, 1, 1, file) != 1){
        perror("[skip_comments] : can't read the first character");
        exit(2);
    }
    ++(*counter);
    if(first_char == '#'){
        while(first_char == '#'){
            while(current_char != '\n'){
                if(fread(&current_char, 1, 1, file) != 1){
                    perror("[skip_comments] : can't read the other characters");
                    exit(3);
                }
                ++(*counter);   
            }
            current_char = 'A'; //pour qu'il soit différent de '\n' au prochain tour de boucle si first_char vaut à nouveau '#'
            if(fread(&first_char, 1, 1, file) != 1){
                perror("[skip_comments] : can't read the first character");
                exit(4);
            }
            ++(*counter);
        }
        if(fseek(file, -1, SEEK_CUR) != 0){ //Après la lecture des commentaires, on recule le curseur qui était juste après le premier caractère de la première ligne après les commentaires
            perror("[skip_comments] : can't move the cursor back to the beginning of the uncommented line");
            exit(5);
        }
    }
    else{
        if(fseek(file, -1, SEEK_CUR) != 0){ //S'il n'y avait pas de commentaires, on se remet au début de la ligne qui nous intéresse
            perror("[skip_comments] : can't move the cursor back to the beginning of the uncommented line");
            exit(6);
        }
    }
}

/**
 *  @requires filename un chemin absolu
 *  @assigns rien
 *  @ensures la picture renvoyée correspond aux données 
 * présentes dans le fichier à l'adresse filename
 */
picture read_picture(char* filename){
    picture result;
    int c_number_read = 0; //On stocke ici le nombre de caractères lus, pour se ramener au début des donénes binaires après la lecture des paramètres
    char* magic_number = malloc(2);
    char* extension = ext_from_path(filename);
    int max_pixel;
    FILE* file = fopen(filename, "rb");
    if(file == NULL){
        perror("[read_picture] : file can't be opened");
        exit(1);
    }
    if(fread(magic_number, 1, 2, file) != 2){ //Lecture du magic number
        perror("[read_picture] : cannot read the magic number");
        exit(2);
    }
    c_number_read += 2;
    if(magic_number[0] == 'P' && magic_number[1] == '5'){ //niveau de gris
        result.n_channel = 1;
    }
    else if(magic_number[0] == 'P' && magic_number[1] == '6'){ //en couleurs
        result.n_channel = 3;
    }
    else{
        perror("[read_picture] : file has invalid magic number");
        exit(3);
    }
    skip_comments(file, &c_number_read);
    if(fscanf(file, "%d %d", &result.l, &result.h) <= 0){ //Lecture de la taille de l'image
        perror("[read_picture] : cannot read the size");
        exit(4);
    }
    c_number_read += digit_number(result.l)+digit_number(result.h)+1;
    skip_comments(file, &c_number_read);
    if(fscanf(file, "%d", &max_pixel) <= 0){ //Lecture de la valeur maximum d'un pixel
        perror("[read_picture] : cannot read the maximum value of a pixel");
        exit(5);
    }
    c_number_read += digit_number(max_pixel);
    skip_comments(file, &c_number_read);
    int data_size = result.l*result.h*result.n_channel;
    if(data_size == 0 || max_pixel == 0){
        perror("[read_picture] : one of the measures of the file is equal to 0");
        exit(6);
    }
    result.pixels = malloc(sizeof(byte)*data_size);
    if (result.pixels == NULL) {
        perror("[read_picture] : memory allocation failed");
        exit(7);
    }
    if(fseek(file, c_number_read, SEEK_SET) != 0){ //Assure de positionner le curseur au début des données binaires
        perror("[read_picture] : can't move the cursor to the begining of the binary data");
        exit(8);
    }
    int bytes_read = fread(result.pixels, sizeof(byte), data_size, file);
    if (bytes_read != data_size){
        fprintf(stderr, "[read_picture] : expected %d bytes, but read %d bytes\n", data_size, bytes_read);
        if (feof(file)) {
            fprintf(stderr, "[read_picture] : unexpected end of file\n");
        }
        if (ferror(file)) {
            perror("[read_picture] : file read error");
        }
        exit(9);
    }
    for(int i=0; i<data_size; i++){ 
        result.pixels[i] = (255/max_pixel)*result.pixels[i]; //permet à result.pixels[i] d'appartenir à [0,255]
    }
    free(extension);
    free(magic_number);
    fclose(file);
    return result;
}

/**
 *  @requires filename un chemin absolu, p la picture à écrire, bien définie
 *  @assigns rien
 *  @ensures écrit l'image correspondant aux données de la 
 *  picture p à l'adresse filename
 */
int write_picture(picture p, char* filename){
    char* extension = ext_from_path(filename);
    if(strcmp(extension, "pgm") != 0 && strcmp(extension, "ppm") != 0){
        perror("[write_picture] : filename has invalid extension");
        return 1;
    }
    FILE* file = fopen(filename, "wb");
    if(file == NULL){
        perror("[write_picture] : file can't be opened");
        return 2;
    }
    if(p.n_channel == 1){
        if(fprintf(file, "P5\n%d %d\n255\n", p.l, p.h) <= 0){
            perror("[write_picture] : can't write the picture info");
            return 3;
        }
    }
    else if(p.n_channel == 3){
        if(fprintf(file, "P6\n%d %d\n255\n", p.l, p.h) <= 0){
            perror("[write_picture] : can't write the picture info");
            return 4;
        }
    }
    if(fseek(file, 15, SEEK_SET) != 0){ //Assure de positionner le curseur là où il faut écrire les données binaires
        perror("[write_picture] : can't move the cursor to the begining of the binary data");
        return 5;
    }
    int data_size = p.l*p.h*p.n_channel;
    if(fwrite(p.pixels, sizeof(byte), data_size, file) != data_size){ //Lecture des pixels
        perror("[write_picture] : problem while writing the pixels");
        return 6;
    }
    free(extension);
    fclose(file);
    return 0;
}

/**
 *  @requires width, height et channels non signés, channels vaut 1 ou 3
 *  @assigns rien
 *  @ensures renvoie une picture bien définie, p.l = width, p.h = height 
 *  et p.n_channel = channels
 */
picture create_picture(unsigned int width, unsigned int height, unsigned int channels){
    picture result;
    result.l = width;
    result.h = height;
    result.n_channel = channels;
    result.pixels = malloc(sizeof(byte)*width*height*channels);
    if(result.pixels == NULL){
        perror("[create_picture] : memory allocation failed");
        exit(1);
    }
    return result;
}


/**
 *  @requires p pointeur vers une picture bien définie
 *  @assigns p.values est libéré, p prend la valeur NULL
 *  @ensures libère la mémoire allouée à p
 */
void clean_picture(picture* p){
    free(p->pixels);
    p->h = 0;
    p->l = 0;
    p->n_channel = 0;
    p->pixels = NULL;
}


/**
 *  @requires picture p bien définie
 *  @assigns rien
 *  @ensures renvoie une copie de p bien définie, où p n'est pas modifiée
 */
picture copy_picture(picture p){
    if(p.pixels == NULL){
        perror("[copy_picture] : empty picture");
        exit(1);
    }
    picture result = create_picture(p.h, p.l, p.n_channel);
    for(int i=0; i<p.h*p.l*p.n_channel; i++){
        result.pixels[i] = p.pixels[i];
    }
    return result;
}


/**
 *  @requires p picture bien définie
 *  @assigns rien
 *  @ensures 1 si p est vide, 0 sinon
 */
int is_empty_picture(picture p){
    if(p.l == 0 || p.h == 0 || p.n_channel == 0 || p.pixels == NULL){
        return 1;
    }
    return 0;
}

/**
 *  @requires p picture bien définie
 *  @assigns rien
 *  @ensures 1 si p est une image en niveau de gris, 0 sinon
 */
int is_gray_picture(picture p){
    if(p.n_channel == 1){
        return 1;
    }
    return 0;
}

/**
 *  @requires p picture bien définie
 *  @assigns rien
 *  @ensures 1 si p est une image en couleurs, 0 sinon
 */
int is_color_picture(picture p){
    if(p.n_channel == 3){
        return 1;
    }
    return 0;
}

/**
 *  @requires p picture bien définie
 *  @assigns rien
 *  @ensures imprime les informations de p sous la forme
 *  (p.l x p.h x p.n_channel)
 */
void info_picture(picture p){
    printf("(%d x %d x %d)", p.l, p.h, p.n_channel);
}

//Donne un résultat en niveau de gris, ce qui est attendu car si R=V=B, la couleur est à la fin un niveau de gris.

/**
 *  @requires p picture bien définie
 *  @assigns rien
 *  @ensures une nouvelle picture bien définie convertie en couleur si 
 * p était en niveau de gris, une copie de p si p était déjà 
 * en couleurs
 */
picture convert_to_color_picture(picture p){
    if(is_color_picture(p) != 0){
        return copy_picture(p);
    }
    else if(is_empty_picture(p)){
        perror("[convert_to_color_picture] : empty picture");
        exit(1);
    }
    else{
        picture result = create_picture(p.l, p.h, 3);
        for(int i=0; i<p.l*p.h*3; i=i+3){
            result.pixels[i] = p.pixels[i/3];
            result.pixels[i+1] = p.pixels[i/3];
            result.pixels[i+2] = p.pixels[i/3];     
        }
        return result;
    }
}

/**
 *  @requires p picture bien définie
 *  @assigns rien
 *  @ensures une nouvelle picture bien définie convertie en niveau de gris si 
 * p était en couleurs, une copie de p si p était déjà 
 * en niveau de gris
 */
picture convert_to_gray_picture(picture p){
    if(is_gray_picture(p) != 0){
        return copy_picture(p);
    }
    else if(is_empty_picture(p)){
        perror("[convert_to_gray_picture] : empty picture");
        exit(1);
    }
    else{
        picture result = create_picture(p.l, p.h, 1);
        for(int i=0; i<p.l*p.h; i++){
            result.pixels[i] = (int)(0.299*p.pixels[3*i]) + (int)(0.587*p.pixels[3*i+1]) + (int)(0.114*p.pixels[3*i+2]);
        }
        return result;
    }
}

/**
 *  @requires p picture bien définie
 *  @assigns rien
 *  @ensures un tableau de pictures bien définies contenant dans l'ordre, 
 *  la composante rouge, verte et bleue
 */
picture* split_picture(picture p){
    if(p.n_channel == 1){
        picture* result_gray = malloc(sizeof(picture));
        *result_gray = copy_picture(p);
        return result_gray;
    }
    else if(is_empty_picture(p)){
        return NULL;
    }
    else{
        picture* result = malloc(sizeof(picture)*3);
        result[0] = create_picture(p.l, p.h, 1);
        result[1] = create_picture(p.l, p.h, 1);
        result[2] = create_picture(p.l, p.h, 1);
        for(int i=0; i<p.l*p.h; i++){
            result[0].pixels[i] = p.pixels[3*i];
            result[1].pixels[i] = p.pixels[3*i+1];
            result[2].pixels[i] = p.pixels[3*i+2];     
        }
        return result;
    }
}

/**
 *  @requires red, green et blue pictures bien définies. Pour les hypothèses 
 *  faites sur red, green et blue, voir le rapport, section 2.3
 *  @assigns rien
 *  @ensures une nouvelle picture bien définie en couleur composée à partir
 *  des trois composantes en niveau de gris passées en argument
 */
picture merge_picture(picture red, picture green, picture blue){
    if(red.n_channel != 1 || green.n_channel != 1 || blue.n_channel != 1){
        picture result = create_picture(red.l, red.h, 3);
        clean_picture(&result);
        return result;
    }
    else if(red.h != green.h || red.h != blue.h || red.l != green.l || red.l != blue.l){
        picture result = create_picture(red.l, red.h, 3);
        clean_picture(&result);
        return result;
    }
    picture result = create_picture(red.l, red.h, 3);
    for(int i=0; i<red.l*red.h*3; i++){
        if(i%3 == 0){
            result.pixels[i] = red.pixels[i/3];
        }
        else if(i%3 == 1){
            result.pixels[i] = green.pixels[i/3];
        }
        else{
            result.pixels[i] = blue.pixels[i/3];
        }
    }
    return result;
}

/**
 *  @requires p picture bien définie, factor flottant de type double
 *  @assigns rien
 *  @ensures une nouvelle picture bien définie correspondant à p éclaircie d'un facteur factor
 */
picture brighten_picture(picture p, double factor){
    if(is_empty_picture(p)){
        perror("[brighten_picture] : empty picture");
        exit(1);
    }
    picture result = create_picture(p.l, p.h, p.n_channel);
    for(int i=0; i<p.h*p.l*p.n_channel; i++){
        int pixel_mod = (int)(p.pixels[i]*factor);
        if(pixel_mod > 255){
            result.pixels[i] = 255;
        }
        else{
            result.pixels[i] = pixel_mod;
        }
    }
    return result;
}

/**
 *  @requires p picture bien définie, number un entier non signé
 *  @assigns rien
 *  @ensures une nouvelle picture bien définie correspondant à p dont on a 
 *  fait fondre number pixels
 */
picture melt_picture(picture p, int number){
    int random_number = 0;
    int pixel_number = p.h*p.l*p.n_channel;
    unsigned int l, c, k;
    picture result = copy_picture(p);
    for(int i=0; i<number; i++){
        random_number = rand()*rand()%(pixel_number+1);
        k = random_number%p.n_channel; //composante du pixel à la position random_number
        c = ((random_number-k)/p.n_channel)%p.l; //colonne du pixel à la position random_number
        l = (((random_number-k)/p.n_channel)-c)/p.h; //ligne du pixel à la position random_number
        if(l>0){//On ne veut regarder le pixel du haut que s'il existe
            if(read_pixel(p, l, c, k) > read_pixel(p, l-1, c, k)){
                write_pixel(result, l, c, k, read_pixel(result, l-1, c, k));
            }
        }
    }
    return result;
}

/**
 *  @requires p picture bien définie
 *  @assigns rien
 *  @ensures une nouvelle picture bien définie correspondant à l'inverse de p
 */
picture inverse_picture(picture p){
    lut inv = create_lut(256);
    for(int i=0; i<256; i++){
        inv.values[i] = 255-i;
    }
    picture result = apply_lut(inv,p);
    return result;
}

//Retourne un tableau de deux entiers, le premier est le pixel de plus petite valeur de p, le deuxième est celui de plus grande valeur

/**
 *  @requires p picture bien définie
 *  @assigns rien
 *  @ensures retourne un tableau de 2 entiers, où le premier correspond à la plus grande 
 *  valeur de p.values, et le second à la plus petite valeur de p.values
 */
int* extrema_pixel(picture p){
    int* result = malloc(sizeof(int)*2);
    result[0] = p.pixels[0];
    result[1] = p.pixels[1];
    for(int i=0; i<p.l*p.h*p.n_channel; i++){
        if(p.pixels[i] < result[0]){
            result[0] = p.pixels[i]; 
        }
        else if(p.pixels[i] > result[1]){
            result[1] = p.pixels[i];
        }
    }
    return result;
}

/**
 *  @requires p picture bien définie
 *  @assigns rien
 *  @ensures une nouvelle picture bien définie correspondant à l'image p normalisée
 */
picture normalize_dynamic_picture(picture p){
    lut norm = create_lut(256);
    int* extrema = extrema_pixel(p);
    int a = 255/(extrema[1]-extrema[0]);    //Voir le rapport, section 2.3 pour l'explication de ce calcul
    int b = -a*extrema[0];                  //Voir le rapport, section 2.3 pour l'explication de ce calcul      
    for(int i=0; i<=extrema[0]; i++){
        norm.values[i] = 0;
    }
    for(int i=extrema[0]+1; i<extrema[1]; i++){
        norm.values[i] = a*i+b;
    }
    for(int i=extrema[1]; i<256; i++){
        norm.values[i] = 255;
    }
    picture result = apply_lut(norm,p);
    return result;
}

/**
 *  @requires p picture bien définie, nb_levels entier non signé
 *  @assigns rien
 *  @ensures une nouvelle picture bien définie correspondant à p auquel on a réduit les pixels à nb_levels niveaux
 */
picture set_levels_picture(picture p, byte nb_levels){
    lut levels = create_lut(nb_levels);
    for(int i=0; i<nb_levels-1; i++){
        levels.values[i] = 255-(255/(nb_levels-1))*(nb_levels-2-i); //On calcule ici simplement la LUT de taille 8 correspondant à lut(i) = i+1
    }
    levels.values[nb_levels-1] = 255;
    picture result = apply_lut(levels,p);
    return result;
}

//Sert dans diff_pictures. Retoure |i|

/**
 *  @requires rien
 *  @assigns rien
 *  @ensures retourne |i|
 */
int abs_int(int i){
    if(i >= 0){
        return i;
    }
    return -1*i;
}

/**
 *  @requires p1 et p2 pictures bien définies
 *  @assigns rien
 *  @ensures une nouvelle picture bien définie correspondant à |p1-p2|
 */
picture diff_picture(picture p1, picture p2){
    if(p1.h != p2.h || p1.l != p2.l || p1.n_channel != p2.n_channel){
        perror("[diff_picture] : Incompatible dimensions");
        exit(1);
    }
    else if(is_empty_picture(p1) || is_empty_picture(p1)){
        perror("[diff_picture] : one of the pictures is empty");
        exit(2);
    }
    picture result = create_picture(p1.l,p1.h,p1.n_channel);
    for(int i=0; i<p1.h*p1.l*p1.n_channel; i++){
        result.pixels[i] = abs_int(p1.pixels[i]-p2.pixels[i]);
    }
    return result;
}

/**
 *  @requires p1 et p2 pictures bien définies
 *  @assigns rien
 *  @ensures une nouvelle picture bien définie correspondant à p1*p2
 */
picture mult_picture(picture p1, picture p2){
    if(p1.h != p2.h || p1.l != p2.l || p1.n_channel != p2.n_channel){
        perror("[mult_picture] : Incompatible dimensions or type of picture");
        exit(1);
    }
    else if(is_empty_picture(p1) || is_empty_picture(p1)){
        perror("[mult_picture] : one of the pictures is empty");
        exit(2);
    }
    picture result = create_picture(p1.l,p1.h,p1.n_channel);
    for(int i=0; i<p1.h*p1.l*p1.n_channel; i++){
        result.pixels[i] = (p1.pixels[i]*p2.pixels[i])%256; //%256 pour avoir une valeur dans l'intervalle [0,255]
    }
    return result;
}

/**
 *  @requires p1, p2 et p3 pictures bien définies
 *  @assigns rien
 *  @ensures une nouvelle picture bien définie correspondant à la mixture définie 
 *  par l'énoncé entre p1 et p2 en utilisant p3
 */
picture mix_picture(picture p1, picture p2, picture p3){
    if(p1.h != p2.h || p2.h != p3.h || p1.l != p2.l  || p2.l != p3.l || p1.n_channel != p2.n_channel){
        perror("[mix_picture] : Incompatible dimensions or type of picture");
        exit(1);
    }
    else if(is_empty_picture(p1) || is_empty_picture(p2)){
        perror("[mix_picture] : one of the pictures is empty");
        exit(2);
    }
    int alpha;
    byte courant;
    picture result = create_picture(p1.l, p1.h, p1.n_channel);
    for(int i=0; i<p1.h; i++){
        for(int j=0; j<p1.l; j++){
            for(int k=0; k<p1.n_channel; k++){
                alpha = read_pixel(p3, i, j, k)/255;
                courant = ((1-alpha)*read_pixel(p1, i, j, k))+(alpha*read_pixel(p2, i, j, k));
                write_pixel(result, i, j, k, courant);
            }
        }
    }
    return result;
}

/**
 *  @requires image picture bien définie
 *  @assigns rien
 *  @ensures une nouvelle picture bien définie correspondant au 
 *  redimensionnement de p au nouvelles dimensions width et height en utilisant l'algorithme des plus proches voisins
 */
picture resample_picture_nearest(picture image, unsigned int width, unsigned int height){
    picture result = create_picture(width, height, image.n_channel);
    double x_scale_factor = ((double)(image.l))/((double)(width));
    double y_scale_factor = ((double)(image.h))/((double)(height));
    int x, y;
    byte current;
    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            for(int k=0; k<image.n_channel; k++){
                x = (int)(j*x_scale_factor);
                y = (int)(i*y_scale_factor);
                current = read_pixel(image,y, x, k);
                write_pixel(result, i, j, k, current);
            }
        }
    }
    return result;
}

//Retourne le min entre a et b. Sert dans resample_picture_bilinear

/**
 *  @requires image picture bien définie
 *  @assigns rien
 *  @ensures retourn min(a,b)
 */
int min(int a, int b){
    if(a <= b){
        return a;
    }
    return b;
}

/**
 *  @requires image picture bien définie
 *  @assigns rien
 *  @ensures une nouvelle picture bien définie correspondant au 
 *  redimensionnement de p au nouvelles dimensions width et height en utilisant 
 * l'algorithme d'interpolation bilinéaire
 */
picture resample_picture_bilinear(picture image, unsigned int width, unsigned int height){
    picture result = create_picture(width, height, image.n_channel);
    double x_scale_factor = ((double)(image.l))/((double)(width));
    double y_scale_factor = ((double)(image.h))/((double)(height));
    double x, y, alpha, beta;
    int x1, x2, y1, y2;
    double current;
    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            x = (double)(j)*x_scale_factor;
            y = (double)(i)*y_scale_factor;
            x1 = (int)(x);
            y1 = (int)(y);
            x2 = min(x1 + 1, image.l - 1);
            y2 = min(y1 + 1, image.h - 1);
            alpha = x - x1;
            beta = y - y1;
            for(int k=0; k<image.n_channel; k++){
                current = (1.0-alpha)*(1.0-beta)*(double)(read_pixel(image,y1,x1,k))
                    + alpha*(1.0-beta)*(double)(read_pixel(image,y1,x2,k))
                    + (1.0-alpha)*beta*(double)(read_pixel(image,y2,x1,k))
                    + alpha*beta*(double)(read_pixel(image,y2,x2,k));
                write_pixel(result, i, j, k, (byte)(current));
            }
        }
    }   
    return result;
}

/* BONUS */

/**
 *  @requires image picture bien définie
 *  @assigns rien
 *  @ensures une nouvelle picture bien définie correspondant à p éclaircie d'un facteur factor
 */
picture brighten_picture_lut(picture p, double factor){
    if(is_empty_picture(p)){
        perror("[brighten_picture_lut] : empty picture");
        exit(1);
    }
    lut brighten = create_lut(256);
    int new_value;
    for(int i=0; i<256; i++){
        new_value = (int)(i*factor);
        if(new_value > 255){
            brighten.values[i] = 255;
        }
        else{
            brighten.values[i] = new_value;
        }
    }
    picture result = apply_lut(brighten,p);
    return result;
}