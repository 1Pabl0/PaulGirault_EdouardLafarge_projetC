#include "bmp8_niveau_de_gris.h"
#include <stdio.h>
#include <stdlib.h>

char image[] = "DATA/lena_gray.bmp";

// En supposant que le fichier image BMP est ouvert et
// que l'en-tête est stocké dans le tableau header

// On sait que la largeur de l'image est stockée à l'offset 18 de l'en-tête
t_bmp8 * bmp8_loadImage(const char * filename){
    unsigned char header[54];
    FILE *pfile = fopen(filename,"rb");
    if(pfile== NULL){
        printf("Lecture impossible du fichier, Verifiez le nom de fichier.");
        exit(1);
    }
    //prototupe de fread -> size_t fread(void *ptr, size_t size, size_t count, FILE *stream);
    fread(header,sizeof(unsigned char ),54,pfile);

    unsigned int width = *(unsigned int *)&header[18];
}
void bmp8_saveImage(const char * filename, t_bmp8 * img){
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier%s\n",filename);
        return;
    }
    fwrite(img->header, sizeof(unsigned char), 54, file);
    fwrite(img->data, sizeof(unsigned char), img->width * img->height, file);
    fclose(file);
    printf("Image enregistrée avec succès dans %s\n", filename);
}


