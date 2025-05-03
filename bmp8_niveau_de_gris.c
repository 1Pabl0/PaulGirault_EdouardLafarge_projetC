#include "bmp8_niveau_de_gris.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

char image[] = "DATA/lena_gray.bmp";
// En supposant que le fichier image BMP est ouvert et
// que l'en-tête est stocké dans le tableau header

// On sait que la largeur de l'image est stockée à l'offset 18 de l'en-tête
t_bmp8 * bmp8_loadImage(const char * filename){
    FILE *pfile = fopen(filename,"rb");
    if(pfile == NULL){
        printf("Lecture impossible du fichier, vérifiez le nom de fichier.\n");
        return NULL;
    }

    // Allouer la structure
    t_bmp8 *img = (t_bmp8*)malloc(sizeof(t_bmp8));
    if (img == NULL) {
        printf("Erreur d'allocation mémoire pour l'image.\n");
        fclose(pfile);
        return NULL;
    }

    // Lire l'en-tête BMP
    if (fread(img->header, sizeof(unsigned char), 54, pfile) != 54) {
        printf("En-tête BMP invalide...\n");
        free(img);
        fclose(pfile);
        return NULL;
    }

    // Lire les dimensions et la profondeur des couleurs
    img->width = *(unsigned int*)&img->header[18];
    img->height = *(unsigned int*)&img->header[22];
    img->colorDepth = *(unsigned short*)&img->header[28];

    if (img->colorDepth != 8) {
        printf("Image avec une profondeur différente de 8 bits. Merci de choisir une autre image...\n");
        free(img);
        fclose(pfile);
        return NULL;
    }

    // Lire la table des couleurs (1024 octets pour BMP 8 bits)
    if (fread(img->colorTable, sizeof(unsigned char), 1024, pfile) != 1024) {
        printf("Erreur lors de la lecture de la table des couleurs.\n");
        free(img);
        fclose(pfile);
        return NULL;
    }

    // Allouer la mémoire pour les pixels
    img->dataSize = img->width * img->height;
    img->data = (unsigned char*)malloc(img->dataSize);

    if (img->data == NULL) {
        printf("Erreur d'allocation mémoire pour les pixels.\n");
        free(img);
        fclose(pfile);
        return NULL;
    }

    // Lire les pixels
    if (fread(img->data, sizeof(unsigned char), img->dataSize, pfile) != img->dataSize) {
        printf("Erreur lors de la lecture des données d'image.\n");
        free(img->data);
        free(img);
        fclose(pfile);
        return NULL;
    }

    fclose(pfile);
    return img;
}



void bmp8_saveImage(const char * filename, t_bmp8 * img){
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier %s\n", filename);
        return;
    }
    fwrite(img->header, sizeof(unsigned char), 54, file);
    fwrite(img->colorTable, sizeof(unsigned char), 1024, file);
    fwrite(img->data, sizeof(unsigned char), img->dataSize, file);
    fclose(file);
    printf("Image enregistrée avec succès dans %s\n", filename);
}
void bmp8_free(t_bmp8 * img){
    if(img != NULL){
        free(img->data);
        free(img);
    }
}
void bmp8_printInfo(t_bmp8 * img){
    printf("Informations de l'image : %s\n",image);
    printf("height = %d pixels\n",img->height);
    printf("width = %d pixels\n",img->width);
    printf("colorDepth = %d bits\n",img->colorDepth);
    printf("Data size = %d\n",img->dataSize);
}


void bmp8_negative(t_bmp8 *img) {
    if (img == NULL || img->data == NULL) return; // Vérifier que l'image est valide

    unsigned char *ptr = img->data; // Pointeur vers les pixels
    for (unsigned int i = 0; i < img->width * img->height; i++) {
        ptr[i] = 255 - ptr[i]; // Inversion de la couleur du pixel
    }
}

void bmp8_brightness(t_bmp8 * img, int value){
    if (img == NULL ) {
        printf("Erreur : Image non chargée ou invalide.\n");
        return;
    }
    for (unsigned int i = 0; i < img->dataSize; i++) {
        int newValue = img->data[i] + value;

        // Clamp les valeurs pour qu'elles restent entre 0 et 255
        if (newValue > 255) {
            img->data[i] = 255;
        } else if (newValue < 0) {
            img->data[i] = 0;
        } else {
            img->data[i] = (unsigned char)newValue;
        }
    }
}


void bmp8_threshold(t_bmp8 * img, int threshold){
    if (img == NULL){
        printf("Image invalide");
        return;
    }
    for (unsigned int i=0 ; i< img->dataSize ;i++){
        if(img->data[i]>=threshold){
        img->data[i]=255;
        }
        else{
            img->data[i]=0;
        }
    }
}

void bmp8_applyFilter(t_bmp8 * img, float ** kernel, int kernelSize) {
    if (img == NULL || img->data == NULL || kernel == NULL){
        printf("Image invalide");
    return;
}

    int n = kernelSize / 2;  // Décalage du noyau
    unsigned char *newData = (unsigned char *) malloc(img->width * img->height); // Image temporaire
    if (newData == NULL) {
        printf("Erreur d'allocation mémoire pour l'application du filtre.\n");
        return;
    }

    // Parcours de l'image (sans les bords)
    for (int y = n; y < img->height - n; y++) {
        for (int x = n; x < img->width - n; x++) {
            float somme = 0.0;

            // Appliquer la convolution
            for (int j = -n; j <= n; j++) {
                for (int i = -n; i <= n; i++) {
                    int pixel = img->data[(y + j) * img->width + (x + i)]; // Pixel voisin
                    somme += pixel * kernel[j + n][i + n]; // Produit par l'élément du noyau
                }
            }

            // Clamping des valeurs (0 à 255)
            if (somme < 0) somme = 0;
            if (somme > 255) somme = 255;

            newData[y * img->width + x] = (unsigned char) somme; // Stockage du résultat
        }
    }

    for (int i = 0; i < img->width * img->height; i++) {
        img->data[i] = newData[i];
    }

    free(newData);


}



void bmp8_filterEmboss(t_bmp8 * img) {
    float embossKernel[3][3] = {
        { -2, -1,  0 },
        { -1,  1,  1 },
        {  0,  1,  2 }
    };

    float **kernel = malloc(3 * sizeof(float*));
    for (int i = 0; i < 3; i++) {
        kernel[i] = malloc(3 * sizeof(float));
        for (int j = 0; j < 3; j++) {
            kernel[i][j] = embossKernel[i][j];
        }
    }

    bmp8_applyFilter(img, kernel, 3);

    for (int i = 0; i < 3; i++) free(kernel[i]);
    free(kernel);
}

void bmp8_filterSharpen(t_bmp8 * img) {
    float sharpenKernel[3][3] = {
        {  0, -1,  0 },
        { -1,  5, -1 },
        {  0, -1,  0 }
    };

    float **kernel = malloc(3 * sizeof(float*));
    for (int i = 0; i < 3; i++) {
        kernel[i] = malloc(3 * sizeof(float));
        for (int j = 0; j < 3; j++) {
            kernel[i][j] = sharpenKernel[i][j];
        }
    }

    bmp8_applyFilter(img, kernel, 3);

    for (int i = 0; i < 3; i++) free(kernel[i]);
    free(kernel);
}

void bmp8_filterOutline(t_bmp8 * img) {
    float outlineKernel[3][3] = {
            { -1, -1, -1 },
            { -1,  8, -1 },
            { -1, -1, -1 }
    };

    // Allocation du kernel dynamique
    float **kernel = malloc(3 * sizeof(float*));
    for (int i = 0; i < 3; i++) {
        kernel[i] = malloc(3 * sizeof(float));
        for (int j = 0; j < 3; j++) {
            kernel[i][j] = outlineKernel[i][j];
        }
    }

    bmp8_applyFilter(img, kernel, 3);

    for (int i = 0; i < 3; i++) free(kernel[i]);
    free(kernel);
}


unsigned int * bmp8_computeHistogram(t_bmp8 * img) {
    if (!img || !img->data) return NULL;

    unsigned int *hist = calloc(256, sizeof(unsigned int));
    if (!hist) return NULL;

    for (unsigned int i = 0; i < img->dataSize; i++) {
        unsigned char pixel = img->data[i];
        hist[pixel]++;
    }

    return hist;
}


unsigned int * bmp8_computeCDF(unsigned int * hist) {
    if (!hist) return NULL;

    unsigned int *cdf = malloc(256 * sizeof(unsigned int));
    if (!cdf) return NULL;

    cdf[0] = hist[0];
    for (int i = 1; i < 256; i++) {
        cdf[i] = cdf[i - 1] + hist[i];
    }

    return cdf;
}


void bmp8_equalize(t_bmp8 * img, unsigned int * hist_eq) {
    if (!img || !img->data || !hist_eq) return;

    unsigned int totalPixels = img->width * img->height;
    unsigned char LUT[256]; // Lookup table de correspondance

    // Créer la table de correspondance en utilisant la formule d’égalisation d’histogramme
    for (int i = 0; i < 256; i++) {
        LUT[i] = (unsigned char)(((float)hist_eq[i] - hist_eq[0]) / (totalPixels - hist_eq[0]) * 255.0 + 0.5);
    }

    // Appliquer la transformation à chaque pixel
    for (unsigned int i = 0; i < img->dataSize; i++) {
        img->data[i] = LUT[img->data[i]];
    }
}
