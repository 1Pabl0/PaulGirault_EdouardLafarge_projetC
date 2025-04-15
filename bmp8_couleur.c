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
    img->data = NULL; // par sécurité

    img->data = bmp24_allocateDataPixels(width, height);
    if (img->data == NULL) {
        printf("Erreur : impossible d'allouer les données pixels.\n");
        free(img);
        return NULL;
    }

    return img;
}
void bmp24_free(t_bmp24 *img) {
    if (img == NULL) return;

    bmp24_freeDataPixels(img->data, img->height);
    free(img);
}




t_bmp24 * bmp24_loadImage(const char * filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Erreur ouverture fichier");
        return NULL;
    }

    // Lecture de l'en-tête BMP
    t_bmp_header header;
    fseek(file, BITMAP_MAGIC, SEEK_SET);
    fread(&header, sizeof(t_bmp_header), 1, file);

    // Vérification du type
    if (header.type != BMP_TYPE) {
        fprintf(stderr, "Erreur : ce n'est pas un fichier BMP valide.\n");
        fclose(file);
        return NULL;
    }

    // Lecture du header info (DIB)
    t_bmp_info header_info;
    fread(&header_info, sizeof(t_bmp_info), 1, file);

    // Vérification que c’est une image 24 bits
    if (header_info.bits != DEFAULT_DEPTH) {
        fprintf(stderr, "Erreur : ce programme ne gère que les images 24 bits.\n");
        fclose(file);
        return NULL;
    }

    // Allocation de la structure t_bmp24
    t_bmp24 *image = bmp24_allocate(header_info.width, header_info.height, header_info.bits);
    if (!image) {
        fclose(file);
        return NULL;
    }

    // Stockage des headers dans l'objet image
    image->header = header;
    image->header_info = header_info;

    int width = image->width;
    int height = image->height;
    int padding = (4 - (width * 3) % 4) % 4;

    // Aller à la position des données
    fseek(file, header.offset, SEEK_SET);

    // Lecture des données pixel par pixel
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            uint8_t bgr[3];
            fread(bgr, sizeof(uint8_t), 3, file);
            image->data[height - 1 - i][j].blue = bgr[0];
            image->data[height - 1 - i][j].green = bgr[1];
            image->data[height - 1 - i][j].red = bgr[2];
        }
        fseek(file, padding, SEEK_CUR); // Sauter le padding
    }

    fclose(file);
    return image;
}

void bmp24_saveImage(t_bmp24 * img, const char * filename) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Erreur ouverture fichier écriture");
        return;
    }

    // Écriture de l'en-tête BMP
    fwrite(&img->header, sizeof(t_bmp_header), 1, file);
    fwrite(&img->header_info, sizeof(t_bmp_info), 1, file);

    int width = img->width;
    int height = img->height;
    int padding = (4 - (width * 3) % 4) % 4;
    uint8_t pad[3] = {0, 0, 0};

    // Aller à la position de l'image
    fseek(file, img->header.offset, SEEK_SET);

    // Écriture ligne par ligne (du bas vers le haut)
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            uint8_t bgr[3] = {
                    img->data[height - 1 - i][j].blue,
                    img->data[height - 1 - i][j].green,
                    img->data[height - 1 - i][j].red
            };
            fwrite(bgr, sizeof(uint8_t), 3, file);
        }
        fwrite(pad, sizeof(uint8_t), padding, file); // padding
    }

    fclose(file);
}
