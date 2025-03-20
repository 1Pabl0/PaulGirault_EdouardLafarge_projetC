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



