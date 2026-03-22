# Notice d'utilisation de l'exécutable main ---


Remarque : les images Lenna_gray.pgm, Lenna_color.ppm, Lenna_BW.pgm et Lenna_BW.ppm sont fournis dans le dossier /Projet/Images


- Compilez le code à l'aide de la commande "make all" tapée dans un terminal dont le répertoire courant est le répertoire /Projet/code
- Toujours dans le même terminal : 

	- La commande ./main rappelle comment utiliser l'exécutable

	- Toute commande de type "./main filename_Lenna_gray.pgm operation", où "filename" est le chemin absolu vers l'image nommée 	"Lenna_gray.pgm" aura pour effet d'appliquer l'opération "operation" sur l'image "Lenna_gray.pgm", et de créer le fichier résultat 	"Lenna_gray_operation.p[g|p]m" dans le même répertoire que là où est stocké l'image "Lenna_gray.pgm".

	- Toute commande de type "./main filename_Lenna_color.ppm operation", où "filename" est le chemin absolu vers l'image nommée 	"Lenna_color.ppm" aura pour effet d'appliquer l'opération "operation" sur l'image "Lenna_color.ppm", et de créer le fichier résultat	"Lenna_color_operation.p[g|p]m" dans le même répertoire que là où est stocké l'image "Lenna_color.ppm".

	- Toute commande de type "./main filename_Lenna_gray.pgm filename_mask.pgm operation" où "filename" est le chemin absolu vers l'image 
	nommée "Lenna_gray.pgm" et "filename_mask" est le chemin absolu vers l'image utilisée comme masque aura pour effet d'appliquer l'opération 	"operation" sur l'image "Lenna_gray.pgm" à l'aide du masque et de créer le fichier résultat "Lenna_gray_operation.pgm" dans le même 	répertoire que là où est stocké l'image "Lenna_gray.pgm".
	**Pour cette étape, on utilisera l'image Lenna_BW.pgm comme masque**

	- Toute commande de type "./main filename_Lenna_color.ppm filename_mask.ppm operation" où "filename" est le chemin absolu vers l'image 	nommée "Lenna_color.ppm" et "filename_mask" est le chemin absolu vers l'image utilisée comme masque aura pour effet d'appliquer 	l'opération "operation" sur l'image "Lenna_color.ppm" à l'aide du masque et de créer le fichier résultat "Lenna_color_operation.ppm" 	dans le même répertoire que là où est stocké l'image "Lenna_color.ppm".
	**Pour cette étape, on utilisera l'image Lenna_BW.ppm comme masque**

	- Toute commande de type "./main filename_Lenna_gray.pgm filename_mask.pgm filename_inv.pgm operation" où "filename" est le chemin absolu 	vers l'image nommée "Lenna_gray.pgm", "filename_mask" est le chemin absolu vers l'image utilisée comme masque et "filename_inv" est le 	chemin absolu vers l'image inversée de Lenna_gray.pgm (qu'on aura pris soin de calculer précédemment), aura pour effet d'appliquer 	l'opération "operation" sur l'image "Lenna_gray.pgm" à l'aide du masque et de l'image inversée et de créer le fichier résultat 	"Lenna_gray_operation.pgm" dans le même répertoire que là où est stocké l'image "Lenna_gray.pgm".
	**Pour cette étape, on utilisera l'image Lenna_BW.pgm comme masque et Lenna_inverse.pgm (à calculer à l'aide du programme) comme image 	inverse**

	- Toute commande de type "./main filename_Lenna_color.ppm filename_mask.ppm filename_inv.ppm operation" où "filename" est le chemin absolu 	vers l'image nommée "Lenna_color.ppm", "filename_mask" est le chemin absolu vers l'image utilisée comme masque et "filename_inv" est le 	chemin absolu vers l'image inversée de Lenna_color.ppm (qu'on aura pris soin de calculer précédemment), aura pour effet d'appliquer 	l'opération "operation" sur l'image "Lenna_color.ppm" à l'aide du masque et de l'image inversée et de créer le fichier résultat 	"Lenna_color_operation.ppm" dans le même répertoire que là où est stocké l'image "Lenna_color.ppm".
	**Pour cette étape, on utilisera l'image Lenna_BW.ppm comme masque et Lenna_inverse.ppm (à calculer à l'aide du programme) comme image 	inverse**

	



