#include "bmp8_couleur.h"
#include <stdio.h>
#include <stdlib.h>


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
