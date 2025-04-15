#include "bmp8_couleur.h"


t_pixel ** bmp24_allocateDataPixels(int width, int height) {
    t_pixel **pixels = (t_pixel **)malloc(height * sizeof(t_pixel *));
    if (pixels == NULL) {
        printf("Erreur d'allocation mémoire pour les lignes de pixels.\n");
        return NULL;
    }

    for (int i = 0; i < height; i++) {
        pixels[i] = (t_pixel *)malloc(width * sizeof(t_pixel));
        if (pixels[i] == NULL) {
            printf("Erreur d'allocation mémoire pour la ligne %d.\n", i);
            // Libérer les lignes déjà allouées
            for (int j = 0; j < i; j++) {
                free(pixels[j]);
            }
            free(pixels);
            return NULL;
        }
    }

    return pixels;
}
void bmp24_freeDataPixels(t_pixel **pixels, int height) {
    if (pixels == NULL) return;

    for (int i = 0; i < height; i++) {
        free(pixels[i]);
    }
    free(pixels);
}
t_bmp24 * bmp24_allocate(int width, int height, int colorDepth) {
    t_bmp24 *img = (t_bmp24 *)malloc(sizeof(t_bmp24));
    if (img == NULL) {
        printf("Erreur d'allocation mémoire pour la structure t_bmp24.\n");
        return NULL;
    }

    img->width = width;
    img->height = height;
    img->colorDepth = colorDepth;

    img->data = bmp24_allocateDataPixels(width, height);
    if (img->data == NULL) {
        free(img);
        printf("Erreur : impossible d'allouer les données pixels.\n");
        return NULL;
    }

    return img;
}
void bmp24_free(t_bmp24 *img) {
    if (img == NULL) return;

    bmp24_freeDataPixels(img->data, img->height);
    free(img);
}
t_bmp24 *img = bmp24_allocate(800, 600, 24);
// ... traitement de l'image ...
bmp24_free(img);