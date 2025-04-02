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
            printf("Mettre le chemin d'accès entier ");
            scanf("%s", filename);

            img = bmp8_loadImage(filename);  // Chargement de l’image
            if (img != NULL) {
                printf("Image chargée avec succès !\n");
            } else {
                printf("Erreur : Impossible de charger l’image.\n");

            }
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
    }

    return 0;
}
int menu_filtres(){
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
    return choix;
}