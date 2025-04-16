#include <stdio.h>
#include <stdlib.h>
#include "menu.h"
#include "bmp8_niveau_de_gris.h"
#include "bmp8_couleur.h"



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
        printf("Votre choix -> ");
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
            img = bmp8_loadImage("../DATA/lena_gray.bmp");  // Charger l’image
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
            bmp8_applyFilter(img,kernel,kernelSize);
            bmp8_saveImage("../DATA/lena_gray_output.bmp",img);
            break;

        case 5:
            printf("");
            img = bmp8_loadImage("../DATA/lena_gray.bmp");
            bmp8_filterEmboss(img);
            bmp8_saveImage("../DATA/lena_gray_output.bmp", img);
            bmp8_free(img);
            printf("Filtre appliqué avec succès !\n");
            break;

        case 6:
            img = bmp8_loadImage("../DATA/lena_gray.bmp");
            bmp8_filterOutline(img);
            bmp8_saveImage("../DATA/lena_gray_output.bmp", img);
            bmp8_free(img);
            printf("Filtre appliqué avec succès !\n");
            break;




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






int Menu(){
    int nombre = 0;
    t_bmp8 *img = NULL;  // Pointeur pour image N&B

    while (1) {
        printf("\nVeuillez choisir une option : \n");
        printf("1. Ouvrir une image (Niveau de gris)\n");
        printf("2. Appliquer un filtre (Niveau de gris)\n");
        printf("3. Sauvegarder une image\n");
        printf("4. Afficher les informations de l'image\n");
        printf("5. Quitter\n");
        printf("6. Filtres image couleur\n");
        printf("Votre choix -> ");
        scanf("%d", &nombre);

        while (nombre > 6 || nombre < 1) {
            printf("Option invalide. Veuillez choisir une option valide : \n");
            scanf("%d", &nombre);
        }

        if (nombre == 1) {
            char filename[256];
            printf("Entrez le nom du fichier image BMP : ");
            scanf("%s", filename);
            img = bmp8_loadImage(filename);
            if (img != NULL) {
                printf("Image chargée avec succès !\n");
            } else {
                printf("Erreur : Impossible de charger l’image.\n");
            }
        }
        else if (nombre == 2){
            menu_filtres();
        }
        else if (nombre == 3){
            if (img != NULL) {
                char filename[256];
                printf("Mettre le chemin d'accès entier : ");
                scanf("%s", filename);
                bmp8_saveImage(filename, img);
            }
        }
        else if (nombre == 4) {
            if (img != NULL) {
                bmp8_printInfo(img);
            } else {
                printf("Aucune image chargée. Veuillez d'abord ouvrir une image.\n");
            }
        }
        else if (nombre == 5) {
            if (img != NULL) {
                bmp8_free(img);
                printf("Mémoire libérée.\n");
            }
            printf("Au revoir !\n");
            break;
        }
        else if (nombre == 6) {
            menu_filtres_couleur();
        }
    }
    return 0;
}

int menu_filtres_couleur() {
    t_bmp24 *img = NULL;
    printf("\nFiltres pour image couleur :\n");
    printf("8. Couleur négative\n");
    printf("9. Conversion en niveaux de gris\n");
    printf("10. Luminosité\n");
    printf("11. Flou (box blur)\n");
    printf("12. Flou gaussien\n");
    printf("13. Netteté\n");
    printf("14. Contours\n");
    printf("15. Relief (Emboss)\n");
    printf("16. Retour au menu principal\n");
    printf("Votre choix -> ");

    int choix;
    scanf("%d", &choix);

    while (choix < 8 || choix > 17) {
        printf("Choix invalide. Réessayez : ");
        scanf("%d", &choix);
    }

    const char *input = "../DATA/lena_color.bmp";
    const char *output = "../DATA/lena_color_output.bmp";

    switch (choix) {
        case 8:
            img = bmp24_loadImage(input);
            bmp24_negative(img);
            bmp24_saveImage(img, output);
            bmp24_free(img);
            printf("Filtre négatif appliqué.\n");
            break;

        case 9:
            img = bmp24_loadImage(input);
            bmp24_grayscale(img);
            bmp24_saveImage(img, output);
            bmp24_free(img);
            printf("Conversion en niveaux de gris effectuée.\n");
            break;

        case 10:
            img = bmp24_loadImage(input);
            int val;
            printf("Valeur de luminosité (-255 à +255) : ");
            scanf("%d", &val);
            bmp24_brightness(img, val);
            bmp24_saveImage(img, output);
            bmp24_free(img);
            printf("Luminosité modifiée.\n");
            break;

        case 11:
            img = bmp24_loadImage(input);
            float **box = boxBlurKernel();
            t_bmp24 *blurred = bmp24_applyFilter(img, box, 3);
            bmp24_saveImage(blurred, output);
            bmp24_free(img);
            bmp24_free(blurred);
            for (int i = 0; i < 3; i++) free(box[i]);
            free(box);
            printf("Box blur appliqué.\n");
            break;

        case 12:
            img = bmp24_loadImage(input);
            float **gauss = gaussianBlurKernel();
            t_bmp24 *gaussed = bmp24_applyFilter(img, gauss, 3);
            bmp24_saveImage(gaussed, output);
            bmp24_free(img);
            bmp24_free(gaussed);
            for (int i = 0; i < 3; i++) free(gauss[i]);
            free(gauss);
            printf("Flou gaussien appliqué.\n");
            break;

        case 13:
            img = bmp24_loadImage(input);
            float **sharp = sharpenKernel();
            t_bmp24 *sharpened = bmp24_applyFilter(img, sharp, 3);
            bmp24_saveImage(sharpened, output);
            bmp24_free(img);
            bmp24_free(sharpened);
            for (int i = 0; i < 3; i++) free(sharp[i]);
            free(sharp);
            printf("Filtre de netteté appliqué.\n");
            break;

        case 14:
            img = bmp24_loadImage(input);
            float **outline = outlineKernel();
            t_bmp24 *outlined = bmp24_applyFilter(img, outline, 3);
            bmp24_saveImage(outlined, output);
            bmp24_free(img);
            bmp24_free(outlined);
            for (int i = 0; i < 3; i++) free(outline[i]);
            free(outline);
            printf("Contours extraits.\n");
            break;

        case 15:
            img = bmp24_loadImage(input);
            float **emboss = embossKernel();
            t_bmp24 *embossed = bmp24_applyFilter(img, emboss, 3);
            bmp24_saveImage(embossed, output);
            bmp24_free(img);
            bmp24_free(embossed);
            for (int i = 0; i < 3; i++) free(emboss[i]);
            free(emboss);
            printf("Effet de relief appliqué.\n");
            break;

        case 16:
            printf("Retour au menu principal...\n");
            break;
        case 17:
            img = bmp24_loadImage(input);
        if (img != NULL) {
            bmp24_printInfo(img);
            bmp24_free(img);
        } else {
            printf("Erreur : Impossible de charger l’image pour affichage des infos.\n");
        }
        break;

    }
    return choix;
}
