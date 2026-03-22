#include <stdio.h>
#include <stdlib.h>
#include <string.h> //For strcmp
#include <time.h> //For srand(time(NULL))
#include "pictures.h"
#include "filename.h"

//Pour bien utiliser l'executable, lire le guide "README.txt" inclus

int main(int argc, char *argv[]){
    srand(time(NULL));
    if(argc <= 2){
        printf("Usage : ./main <filepath1> [filepath2] ... [filepath n] <operation_name>");
    }
    else if(argc == 3){
        picture p = read_picture(argv[1]);
        char* dir = dir_from_path(argv[1]);
        char* name = name_from_path(argv[1]);
        if(strcmp(name, "Lenna_gray") == 0){
            if(strcmp(argv[2], "convert_color") == 0){
                char* name_convert_color = concat_parts(dir, name, argv[2], "ppm");
                picture result_convert_color = convert_to_color_picture(p);
                write_picture(result_convert_color, name_convert_color);
                free(name_convert_color);
                clean_picture(&result_convert_color);
            }
            else if(strcmp(argv[2], "brighten") == 0){
                char* name_brighten = concat_parts(dir, name, argv[2], "pgm");
                picture result_brighten = brighten_picture(p, 1.5);
                write_picture(result_brighten, name_brighten);
                free(name_brighten);
                clean_picture(&result_brighten);
            }
            else if(strcmp(argv[2], "melted") == 0){
                char* name_melted = concat_parts(dir, name, argv[2], "pgm");
                picture result_melted = melt_picture(p, p.l*p.h*p.n_channel*5);
                write_picture(result_melted, name_melted);
                free(name_melted);
                clean_picture(&result_melted);
            }
            else if(strcmp(argv[2], "inverse") == 0){
                char* name_inverse = concat_parts(dir, name, argv[2], "pgm");
                picture result_inverse = inverse_picture(p);
                write_picture(result_inverse, name_inverse);
                free(name_inverse);
                clean_picture(&result_inverse);
            }
            else if(strcmp(argv[2], "dynamic") == 0){
                char* name_dynamic = concat_parts(dir, name, argv[2], "pgm");
                picture result_dynamic = normalize_dynamic_picture(p);
                write_picture(result_dynamic, name_dynamic);
                free(name_dynamic);
                clean_picture(&result_dynamic);
            }
            else if(strcmp(argv[2], "levels") == 0){
                char* name_levels = concat_parts(dir, name, argv[2], "pgm");
                picture result_levels = set_levels_picture(p, 8);
                write_picture(result_levels, name_levels);
                free(name_levels);
                clean_picture(&result_levels);
            }
            else if(strcmp(argv[2], "smaller_nearest") == 0){
                char* name_smaller_nearest = concat_parts(dir, name, argv[2], "pgm");
                picture result_smaller_nearest = resample_picture_nearest(p, (int)((double)(p.l*(1.0/1.36))), (int)((double)(p.h*(1.0/1.36))));
                write_picture(result_smaller_nearest, name_smaller_nearest);
                free(name_smaller_nearest);
                clean_picture(&result_smaller_nearest);
            }
            else if(strcmp(argv[2], "larger_nearest") == 0){
                char* name_larger_nearest = concat_parts(dir, name, argv[2], "pgm");
                picture result_larger_nearest = resample_picture_nearest(p, p.l*1.36, p.h*1.36);
                write_picture(result_larger_nearest, name_larger_nearest);
                free(name_larger_nearest);
                clean_picture(&result_larger_nearest);
            }
            else if(strcmp(argv[2], "smaller_bilinear") == 0){
                char* name_smaller_bilinear = concat_parts(dir, name, argv[2], "pgm");
                picture result_smaller_bilinear = resample_picture_bilinear(p, (int)((double)(p.l*(1.0/1.36))), (int)((double)(p.h*(1.0/1.36))));
                write_picture(result_smaller_bilinear, name_smaller_bilinear);
                free(name_smaller_bilinear);
                clean_picture(&result_smaller_bilinear);
            }
            else if(strcmp(argv[2], "larger_bilinear") == 0){
                char* name_larger_bilinear = concat_parts(dir, name, argv[2], "pgm");
                picture result_larger_bilinear = resample_picture_bilinear(p, p.l*1.36, p.h*1.36);
                write_picture(result_larger_bilinear, name_larger_bilinear);
                free(name_larger_bilinear);
                clean_picture(&result_larger_bilinear);
            }
            else if(strcmp(argv[2], "difference") == 0){
                char* name_difference = concat_parts(dir, name, argv[2], "pgm");
                picture result_larger_nearest = resample_picture_nearest(p, p.l*1.36, p.h*1.36);
                picture result_larger_bilinear = resample_picture_bilinear(p, p.l*1.36, p.h*1.36);
                picture result_difference = diff_picture(result_larger_nearest, result_larger_bilinear);
                picture result_difference_normalized = normalize_dynamic_picture(result_difference);
                write_picture(result_difference_normalized, name_difference);
                free(name_difference);
                clean_picture(&result_larger_nearest);
                clean_picture(&result_larger_bilinear);
                clean_picture(&result_difference);
                clean_picture(&result_difference_normalized);
            }
            else if(strcmp(argv[2], "brighten_lut") == 0){
                char* name_brighten_lut = concat_parts(dir, name, argv[2], "pgm");
                picture result_brighten_lut = brighten_picture_lut(p, 1.5);
                write_picture(result_brighten_lut, name_brighten_lut);
                free(name_brighten_lut);
                clean_picture(&result_brighten_lut);
            }
            else{
                perror("[main] : enter a valid operation !");
            }
        }
        else if(strcmp(name, "Lenna_color") == 0){
            if(strcmp(argv[2], "convert_gray") == 0){
                char* name_convert_gray = concat_parts(dir, name, argv[2], "pgm");
                picture result_convert_gray = convert_to_gray_picture(p);
                write_picture(result_convert_gray, name_convert_gray);
                free(name_convert_gray);
                clean_picture(&result_convert_gray);
            }
            else if(strcmp(argv[2], "split") == 0){
                char* name_split_red = concat_parts(dir, name, "red", "pgm");
                char* name_split_green = concat_parts(dir, name, "green", "pgm");
                char* name_split_blue = concat_parts(dir, name, "blue", "pgm");
                picture* result_split = split_picture(p);
                write_picture(result_split[0], name_split_red);
                write_picture(result_split[1], name_split_green);
                write_picture(result_split[2], name_split_blue);
                clean_picture(&result_split[0]);
                clean_picture(&result_split[1]);
                clean_picture(&result_split[2]);
                free(name_split_red);
                free(name_split_green);
                free(name_split_blue);
                free(result_split);
            }
            else if(strcmp(argv[2], "brighten") == 0){
                char* name_brighten = concat_parts(dir, name, argv[2], "ppm");
                picture result_brighten = brighten_picture(p, 1.5);
                write_picture(result_brighten, name_brighten);
                free(name_brighten);
                clean_picture(&result_brighten);
            }
            else if(strcmp(argv[2], "melted") == 0){
                char* name_melted = concat_parts(dir, name, argv[2], "ppm");
                picture result_melted = melt_picture(p, p.l*p.h*p.n_channel*5);
                write_picture(result_melted, name_melted);
                free(name_melted);
                clean_picture(&result_melted);
            }
            else if(strcmp(argv[2], "inverse") == 0){
                char* name_inverse = concat_parts(dir, name, argv[2], "ppm");
                picture result_inverse = inverse_picture(p);
                write_picture(result_inverse, name_inverse);
                free(name_inverse);
                clean_picture(&result_inverse);
            }
            else if(strcmp(argv[2], "dynamic") == 0){
                char* name_dynamic = concat_parts(dir, name, argv[2], "ppm");
                picture* result_split = split_picture(p);
                picture maximized_red = normalize_dynamic_picture(result_split[0]);
                picture maximized_green = normalize_dynamic_picture(result_split[1]);
                picture maximized_blue = normalize_dynamic_picture(result_split[2]);
                picture result_dynamic = merge_picture(maximized_red, maximized_green, maximized_blue);
                write_picture(result_dynamic, name_dynamic);
                clean_picture(&result_dynamic);
                clean_picture(&maximized_red);
                clean_picture(&maximized_green);
                clean_picture(&maximized_blue);
                clean_picture(&result_split[0]);
                clean_picture(&result_split[1]);
                clean_picture(&result_split[2]);
                free(result_split);
                free(name_dynamic);
            }
            else if(strcmp(argv[2], "levels") == 0){
                char* name_levels = concat_parts(dir, name, argv[2], "ppm");
                picture result_levels = set_levels_picture(p, 8);
                write_picture(result_levels, name_levels);
                free(name_levels);
                clean_picture(&result_levels);
            }
            else if(strcmp(argv[2], "smaller_nearest") == 0){
                char* name_smaller_nearest = concat_parts(dir, name, argv[2], "ppm");
                picture result_smaller_nearest = resample_picture_nearest(p, (int)((double)(p.l*(1.0/1.36))), (int)((double)(p.h*(1.0/1.36))));
                write_picture(result_smaller_nearest, name_smaller_nearest);
                free(name_smaller_nearest);
                clean_picture(&result_smaller_nearest);
            }
            else if(strcmp(argv[2], "larger_nearest") == 0){
                char* name_larger_nearest = concat_parts(dir, name, argv[2], "ppm");
                picture result_larger_nearest = resample_picture_nearest(p, p.l*1.36, p.h*1.36);
                write_picture(result_larger_nearest, name_larger_nearest);
                free(name_larger_nearest);
                clean_picture(&result_larger_nearest);
            }
            else if(strcmp(argv[2], "smaller_bilinear") == 0){
                char* name_smaller_bilinear = concat_parts(dir, name, argv[2], "ppm");
                picture result_smaller_bilinear = resample_picture_bilinear(p, (int)((double)(p.l*(1.0/1.36))), (int)((double)(p.h*(1.0/1.36))));
                write_picture(result_smaller_bilinear, name_smaller_bilinear);
                free(name_smaller_bilinear);
                clean_picture(&result_smaller_bilinear);
            }
            else if(strcmp(argv[2], "larger_bilinear") == 0){
                char* name_larger_bilinear = concat_parts(dir, name, argv[2], "ppm");
                picture result_larger_bilinear = resample_picture_bilinear(p, p.l*1.36, p.h*1.36);
                write_picture(result_larger_bilinear, name_larger_bilinear);
                free(name_larger_bilinear);
                clean_picture(&result_larger_bilinear);
            
            }
            else if(strcmp(argv[2], "difference") == 0){
                char* name_difference = concat_parts(dir, name, argv[2], "ppm");
                picture result_larger_nearest = resample_picture_nearest(p, p.l*1.36, p.h*1.36);
                picture result_larger_bilinear = resample_picture_bilinear(p, p.l*1.36, p.h*1.36);
                picture result_difference = diff_picture(result_larger_nearest, result_larger_bilinear);
                picture result_difference_normalized = normalize_dynamic_picture(result_difference);
                write_picture(result_difference_normalized, name_difference);
                free(name_difference);
                clean_picture(&result_larger_nearest);
                clean_picture(&result_larger_bilinear);
                clean_picture(&result_difference);
                clean_picture(&result_difference_normalized);
            }
            else if(strcmp(argv[2], "brighten_lut") == 0){
                char* name_brighten_lut = concat_parts(dir, name, argv[2], "ppm");
                picture result_brighten_lut = brighten_picture_lut(p, 1.5);
                write_picture(result_brighten_lut, name_brighten_lut);
                free(name_brighten_lut);
                clean_picture(&result_brighten_lut);
            }
            else{
                perror("[main] : enter a valid operation !");
            }
        }
        free(dir);
        free(name);
        clean_picture(&p); 
    }
    else if(argc == 4){
        picture p = read_picture(argv[1]);
        picture mask = read_picture(argv[2]);
        char* dir = dir_from_path(argv[1]);
        char* name = name_from_path(argv[1]);
        if(strcmp(name, "Lenna_gray") == 0){
            if(strcmp(argv[3], "product") == 0){
                if(mask.h != p.h || mask.h != p.l){
                    mask = resample_picture_bilinear(mask, p.l, p.h);
                }
                char* name_product = concat_parts(dir, name, argv[3], "pgm");
                picture result_product = mult_picture(p, mask);
                write_picture(result_product, name_product);
                clean_picture(&result_product);
                free(name_product);
            }
            else{
                perror("[main] : enter a valid operation !");
            }
        }
        if(strcmp(name, "Lenna_color") == 0){
            if(strcmp(argv[3], "product") == 0){
                if(mask.h != p.h || mask.h != p.l){
                    mask = resample_picture_bilinear(mask, p.l, p.h);
                }
                char* name_product = concat_parts(dir, name, argv[3], "ppm");
                picture result_product = mult_picture(p, mask);
                write_picture(result_product, name_product);
                clean_picture(&result_product);
                free(name_product);
            }
            else{
                perror("[main] : enter a valid operation !");
            }
        }
        free(dir);
        free(name);
        clean_picture(&p);
        clean_picture(&mask);
    }
    else if(argc == 5){
        picture p = read_picture(argv[1]);
        picture inv = read_picture(argv[2]);
        picture mask = read_picture(argv[3]);
        char* dir = dir_from_path(argv[1]);
        char* name = name_from_path(argv[1]);
        if(strcmp(name, "Lenna_gray") == 0){
            if(strcmp(argv[4], "mixture") == 0){
                char* name_mixture = concat_parts(dir, name, argv[4], "pgm");
                picture result_mixture = mix_picture(p, inv, mask);
                write_picture(result_mixture, name_mixture);
                clean_picture(&result_mixture);
                free(name_mixture);
            }
            else{
                perror("[main] : enter a valid operation !");
            }
        }
        else if(strcmp(name, "Lenna_color") == 0){
            if(strcmp(argv[4], "mixture") == 0){
                char* name_mixture = concat_parts(dir, name, argv[4], "ppm");
                picture result_mixture = mix_picture(p, inv, mask);
                write_picture(result_mixture, name_mixture);
                clean_picture(&result_mixture);
                free(name_mixture);
            }
            else{
                perror("[main] : enter a valid operation !");
            }
        }
        free(dir);
        free(name);
        clean_picture(&p);
        clean_picture(&inv);
        clean_picture(&mask);
    }
    return 0;
}