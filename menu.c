#include <stdio.h>
#include <stdlib.h>
#include "menu.h"
#include "bmp8_niveau_de_gris.h"
int Menu(){
    int nombre = 0;
    t_bmp8 *img = NULL;  // Pointeur pour stocker l’image

    while (1) {
        printf("\nVeuillez choisir une option : \n");
        printf("1. Ouvrir une image\n");
        printf("2. Appliquer un filtre\n");
        printf("3. Sauvegarder une image\n");
        printf("4. Afficher les informations de l'image\n");
        printf("5. Quitter\n");
        printf("Votre choix -> ");
        scanf("%d", &nombre);

        while (nombre > 5 || nombre < 1) {
            printf("Option invalide. Veuillez choisir une option valide : \n");
            scanf("%d", &nombre);
        }

        if (nombre == 1) {
            // Chargement de l'image
            char filename[256];
            printf("Entrez le nom du fichier image BMP : ");
            printf("Mettre le chemin d'accès entier : ");
            scanf("%s", filename);

            img = bmp8_loadImage(filename);  // Chargement de l’image
            if (img != NULL) {
                printf("Image chargée avec succès !\n");
            } else {
                printf("Erreur : Impossible de charger l’image.\n");

            }
        }
        else if (nombre == 2){
            menu_filtres();
        }
        else if (nombre == 4) {
            if (img != NULL) {
                bmp8_printInfo(img);  // Affichage des infos de l’image
            } else {
                printf("Aucune image chargée. Veuillez d'abord ouvrir une image.\n");
            }
        }
        else if (nombre == 5) {
            // Libérer la mémoire et quitter
            if (img != NULL) {
                bmp8_free(img);
                printf("Mémoire libérée.\n");
            }
            printf("Au revoir !\n");
            break;
        }
        else if (nombre == 3){
            if (img != NULL) {
                char filename[256];
                printf("Mettre le chemin d'accès entier : ");
                scanf("%s", filename);
                bmp8_saveImage(filename, img);
            }
        }
    }

    return 0;
}
int menu_filtres(){
    t_bmp8 *img = NULL;
    printf("Veuillez choisir un filtre :\n");
    printf("1. Négatif\n"
           "2. Luminosité\n"
           "3. Binarisation\n"
           "4. Flou\n"
           "5. Flou gaussien\n"
           "5. Netteté\n"
           "6. Contours\n"
           "7. Relief\n"
           "8. Retourner au menu précédent");
    printf("Votre choix ->");
    int choix = 0;
    scanf("%d",&choix);
    while (choix>8 || choix<1){
        printf("Votre choix ->");
    scanf("%d",&choix);
    }
    switch (choix) {
        case 1:
            img = bmp8_loadImage("../DATA/lena_gray.bmp");
            bmp8_negative(img);
            bmp8_saveImage("../DATA/lena_gray_output.bmp",img);
            break;

        case 2:
            img= bmp8_loadImage("../DATA/lena_gray.bmp");
            int value;
            printf("de combien voulez vous augmenter la luminosité (-255;255) : ");
            scanf("%d",&value);
            bmp8_brightness(img,value);
            bmp8_saveImage("../DATA/lena_gray_output.bmp",img);
            break;

        case 3:
            img = bmp8_loadImage("../DATA/lena_gray.bmp");
            int treshold;
            printf("Veuillez rentrer un nombre référence treshold : (faible = 60, moyen = 128, fort = 200)");
            scanf("%d",&treshold);
            bmp8_threshold(img,treshold);
            bmp8_saveImage("../DATA/lena_gray_output.bmp",img);
            break;
        case 4:
            t_bmp8 *img = bmp8_loadImage("../DATA/lena_gray.bmp");  // Charger l’image
            if (img == NULL) return 1;

            int kernelSize = 3;
            float **kernel = (float **)malloc(kernelSize * sizeof(float *));
            for (int i = 0; i < kernelSize; i++) {
                kernel[i] = (float *)malloc(kernelSize * sizeof(float));
            }

    // Initialisation du noyau flou box blur (3x3)
            for (int i = 0; i < kernelSize; i++) {
                for (int j = 0; j < kernelSize; j++) {
                    kernel[i][j] = 1.0 / 9;  // Chaque élément a la même valeur
                }
            }


    // Appliquer le filtre
            bmp8_applyFilter(img, kernel, kernelSize);

    // Sauvegarde de l’image modifiée
            bmp8_saveImage("../DATA/lena_gray_output.bmp", img);

    // Libération de la mémoire
            for (int i = 0; i < kernelSize; i++) {
                free(kernel[i]);
            }
            free(kernel);
            bmp8_free(img);

            printf("Filtre appliqué avec succès !\n");
            break;
        case 7:
            printf("");
        img = bmp8_loadImage("../DATA/lena_gray.bmp");
        bmp8_filterSharpen(img);
        bmp8_saveImage("../DATA/lena_gray_output.bmp", img);
        bmp8_free(img);
        printf("Filtre appliqué avec succès !\n");
        break;


    }
    return choix;
}