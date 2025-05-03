#include <stdio.h>
#include <stdlib.h>
#include "menu.h"
#include "bmp8_niveau_de_gris.h"
#include "bmp24.h"

void menu_filtres_couleur_complet() {
    int choix;
    t_bmp24 *img = NULL;

    const char *input = "../DATA/lena_color.bmp";
    const char *output = "../DATA/lena_color_output.bmp";

    while (1) {
        printf("\n🎨 Menu des filtres pour image couleur :\n");
        printf("1. Négatif\n");
        printf("2. Niveaux de gris\n");
        printf("3. Luminosité\n");
        printf("4. Flou (Box blur)\n");
        printf("5. Flou gaussien\n");
        printf("6. Netteté\n");
        printf("7. Contours\n");
        printf("8. Relief (Emboss)\n");
        printf("9. Histogramme en 24 bits : Image égalisée\n");
        printf("0. Retour\n");
        printf("Votre choix -> ");
        scanf("%d", &choix);

        if (choix == 0) return;

        img = bmp24_loadImage(input);
        if (!img) {
            printf("❌ Erreur de chargement de l’image.\n");
            continue;
        }

        switch (choix) {
            case 1:
                bmp24_negative(img);
                break;
            case 2:
                bmp24_grayscale(img);
                break;
            case 3: {
                int value;
                printf("De combien voulez-vous modifier la luminosité (-255 à 255) : ");
                scanf("%d", &value);
                bmp24_brightness(img, value);

                break;
            }
            case 4: {
                float **k = boxBlurKernel();
                t_bmp24 *blurred = bmp24_applyFilter(img, k, 3);
                bmp24_free(img);
                img = blurred;
                for (int i = 0; i < 3; i++) free(k[i]);
                free(k);
                break;
            }
            case 5: {
                float **k = gaussianBlurKernel();
                t_bmp24 *blurred = bmp24_applyFilter(img, k, 3);
                bmp24_free(img);
                img = blurred;
                for (int i = 0; i < 3; i++) free(k[i]);
                free(k);
                break;
            }
            case 6: {
                float **k = sharpenKernel();
                t_bmp24 *sharpened = bmp24_applyFilter(img, k, 3);
                bmp24_free(img);
                img = sharpened;
                for (int i = 0; i < 3; i++) free(k[i]);
                free(k);
                break;
            }
            case 7: {
                float **k = outlineKernel();
                t_bmp24 *outlined = bmp24_applyFilter(img, k, 3);
                bmp24_free(img);
                img = outlined;
                for (int i = 0; i < 3; i++) free(k[i]);
                free(k);
                break;
            }
            case 8: {
                float **k = embossKernel();
                t_bmp24 *embossed = bmp24_applyFilter(img, k, 3);
                bmp24_free(img);
                img = embossed;
                for (int i = 0; i < 3; i++) free(k[i]);
                free(k);
                break;

            }
            case 9:
                bmp24_equalize(img);
                break;
            default:
                printf("Choix invalide.\n");
                bmp24_free(img);
                continue;
        }

        bmp24_saveImage(img, output);
        bmp24_free(img);
        printf("✅ Filtre appliqué et image enregistrée dans %s\n", output);

    }
}
void menu_nb() {
    int choix;
    t_bmp8 *img = NULL;

    while (1) {
        printf("\n===== Menu : Image Niveaux de Gris =====\n");
        printf("1. Ouvrir une image\n");
        printf("2. Appliquer un filtre\n");
        printf("3. Sauvegarder l’image\n");
        printf("4. Afficher les informations\n");
        printf("5. Retour au menu principal\n");
        printf("Votre choix -> ");
        scanf("%d", &choix);

        switch (choix) {
            case 1: {
                char filename[256];
                printf("Nom du fichier image BMP : ");
                scanf("%s", filename);
                img = bmp8_loadImage(filename);
                if (img != NULL)
                    printf("✅ Image chargée avec succès !\n");
                else
                    printf("❌ Erreur de chargement.\n");
                break;
            }
            case 2:
                if (img != NULL) {
                    menu_filtres();
                } else {
                    printf("❗ Veuillez d'abord charger une image.\n");
                }
            break;
            case 3:
                if (img != NULL) {
                    char filename[256];
                    printf("Chemin pour enregistrer : ");
                    scanf("%s", filename);
                    bmp8_saveImage(filename, img);
                    printf("✅ Image sauvegardée.\n");
                } else {
                    printf("❗ Aucune image à sauvegarder.\n");
                }
            break;
            case 4:
                if (img != NULL)
                    bmp8_printInfo(img);
                else
                    printf("❗ Aucune image chargée.\n");
            break;
            case 5:
                if (img != NULL) {
                    bmp8_free(img);
                    printf("Mémoire libérée.\n");
                }
            return;
            default:
                printf("Choix invalide.\n");
        }
    }
}
void menu_couleur() {
    const char *input = "../DATA/lena_color.bmp";
    const char *output = "../DATA/lena_color_output.bmp";

    int choix;
    t_bmp24 *img = NULL;

    while (1) {
        printf("\n===== Menu : Image Couleur =====\n");
        printf("1. Appliquer un filtre\n");
        printf("2. Informations de l'image\n");
        printf("3. Retour au menu principal\n");
        printf("Votre choix -> ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                menu_filtres_couleur_complet();
            break;
            case 2:
                img = bmp24_loadImage(input);
            if (img != NULL) {
                bmp24_printInfo(img);
                bmp24_free(img);
            } else {
                printf("❌ Impossible de charger l’image.\n");
            }
            break;
            case 3:
                return;
            default:
                printf("Choix invalide.\n");
        }
    }
}
void afficher_instructions() {
    printf("\n📘 Instructions d'utilisation :\n");
    printf("- Ce programme permet de charger, modifier et sauvegarder des images BMP.\n");
    printf("- Vous pouvez appliquer des filtres sur des images en niveaux de gris ou en couleur.\n");
    printf("- Utilisez des chemins corrects pour les fichiers BMP.\n");
    printf("- Formats supportés : BMP 8 bits (niveaux de gris) et 24 bits (couleur).\n");
    printf("- Fichiers de test : lena_gray.bmp, lena_color.bmp dans le dossier /DATA/\n");
    printf("- Commencez par charger une image, puis appliquez un filtre et sauvegardez si besoin.\n");
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
           "8. Égalisation d’histogramme en 8 bits\n"
           "9. Retourner au menu précédent");
    printf("Votre choix ->");
    int choix = 0;
    scanf("%d",&choix);
    while (choix>9 || choix<1){
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

        case 7:
            printf("");
        img = bmp8_loadImage("../DATA/lena_gray.bmp");
        bmp8_filterSharpen(img);
        bmp8_saveImage("../DATA/lena_gray_output.bmp", img);
        bmp8_free(img);
        printf("Filtre appliqué avec succès !\n");
        break;
        case 8:
            img = bmp8_loadImage("../DATA/lena_gray.bmp");
            if (img == NULL) {
                printf("Erreur : impossible de charger l’image.\n");
                return 1;
            }

            // Calcul de l'histogramme
            unsigned int *hist = bmp8_computeHistogram(img);
            if (hist == NULL) {
                printf("Erreur lors du calcul de l'histogramme.\n");
                bmp8_free(img);
                return 1;
            }

            // Calcul de la fonction de distribution cumulée (CDF)
            unsigned int *cdf = bmp8_computeCDF(hist);
            if (cdf == NULL) {
                printf("Erreur lors du calcul de la CDF.\n");
                free(hist);
                bmp8_free(img);
                return 1;
            }

            // Application de l'égalisation d'histogramme
            bmp8_equalize(img, cdf);

            // Sauvegarde de l'image modifiée
            bmp8_saveImage("../DATA/lena_gray_output.bmp", img);
            printf("Égalisation d’histogramme appliquée avec succès !\n");

            // Libération de la mémoire
            free(hist);
            free(cdf);
            bmp8_free(img);
            break;



    }
    return choix;
}
int menu_principal() {
    int choix;

    while (1) {
        printf("\n==============================\n");
        printf("🎉 Bienvenue dans le programme \n de traitement d'image 🎉\n");
        printf("==============================\n");
        printf("1. 📷 Image en noir et blanc\n");
        printf("2. 🌈 Image en couleur\n");
        printf("3. 📘 Instructions d'utilisation\n");
        printf("4. ❌ Quitter\n");
        printf("==============================\n");
        printf("Votre choix -> ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                menu_nb();
            break;
            case 2:
                menu_couleur();
            break;
            case 3:
                afficher_instructions();
            break;
            case 4:
                printf("Merci d’avoir utilisé notre programme ! À bientôt 👋\n");
            return 0;
            default:
                printf("Choix invalide. Veuillez entrer un nombre entre 1 et 4.\n");
        }
    }
}

