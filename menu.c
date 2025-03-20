#include <stdio.h>
#include "menu.h"

int Menu(){
    int nombre=0;
    printf("Veuillez choisir une option : \n");
    printf("1. Ouvrir une image\n");
    printf("2; Appliquer un filtre\n");
    printf("4. Afficher les informations de l'image\n");
    printf("5. Quitter\n");
    scanf("%d",&nombre);
    printf("Votre choix : %d",nombre);

    return nombre;
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



