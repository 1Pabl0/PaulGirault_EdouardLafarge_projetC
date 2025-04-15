#include "bmp8_couleur.h"

// 🔸 Allocation d'une matrice de pixels
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
            for (int j = 0; j < i; j++) {
                free(pixels[j]);
            }
            free(pixels);
            return NULL;
        }
    }

    return pixels;
}

// 🔸 Libération de la matrice de pixels
void bmp24_freeDataPixels(t_pixel **pixels, int height) {
    if (pixels == NULL) return;

    for (int i = 0; i < height; i++) {
        free(pixels[i]);
    }
    free(pixels);
}

// 🔸 Allocation de la structure t_bmp24
t_bmp24 * bmp24_allocate(int width, int height, int colorDepth) {
    t_bmp24 *img = (t_bmp24 *)malloc(sizeof(t_bmp24));
    if (img == NULL) {
        printf("Erreur d'allocation mémoire pour la structure t_bmp24.\n");
        return NULL;
    }

    img->width = width;
    img->height = height;
    img->colorDepth = colorDepth;
    img->data = NULL;

    img->data = bmp24_allocateDataPixels(width, height);
    if (img->data == NULL) {
        printf("Erreur : impossible d'allouer les données pixels.\n");
        free(img);
        return NULL;
    }

    return img;
}

// 🔸 Libération complète de l'image
void bmp24_free(t_bmp24 *img) {
    if (img == NULL) return;
    bmp24_freeDataPixels(img->data, img->height);
    free(img);
}

// 🔸 Fonction file_rawRead
void file_rawRead(uint32_t position, void * buffer, uint32_t size, size_t n, FILE * file) {
    fseek(file, position, SEEK_SET);
    fread(buffer, size, n, file);
}

// 🔸 Chargement complet de l’image 24 bits
t_bmp24 * bmp24_loadImage(const char * filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Erreur ouverture fichier");
        return NULL;
    }

    // Lecture de l'en-tête BMP
    t_bmp_header header;
    file_rawRead(BITMAP_MAGIC, &header, sizeof(t_bmp_header), 1, file);

    if (header.type != BMP_TYPE) {
        fprintf(stderr, "Erreur : ce n'est pas un fichier BMP valide.\n");
        fclose(file);
        return NULL;
    }

    // Lecture du DIB header
    t_bmp_info header_info;
    file_rawRead(BITMAP_WIDTH, &header_info, sizeof(t_bmp_info), 1, file);

    if (header_info.bits != DEFAULT_DEPTH) {
        fprintf(stderr, "Erreur : ce programme ne gère que les images 24 bits.\n");
        fclose(file);
        return NULL;
    }

    // Allocation mémoire pour l’image
    t_bmp24 *image = bmp24_allocate(header_info.width, header_info.height, header_info.bits);
    if (!image) {
        fclose(file);
        return NULL;
    }

    image->header = header;
    image->header_info = header_info;

    int width = image->width;
    int height = image->height;
    int padding = (4 - (width * 3) % 4) % 4;

    fseek(file, header.offset, SEEK_SET);

    // Lecture directe des pixels (ligne par ligne, du bas vers le haut)
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            uint8_t bgr[3];
            fread(bgr, sizeof(uint8_t), 3, file);
            image->data[height - 1 - i][j].blue  = bgr[0];
            image->data[height - 1 - i][j].green = bgr[1];
            image->data[height - 1 - i][j].red   = bgr[2];
        }
        fseek(file, padding, SEEK_CUR);
    }

    fclose(file);
    return image;
}