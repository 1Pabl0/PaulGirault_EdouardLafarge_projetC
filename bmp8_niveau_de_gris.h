#ifndef PAULGIRAULT_EDOUARDLAFARGE_PROJETC_BMP8_NIVEAU_DE_GRIS_H
#define PAULGIRAULT_EDOUARDLAFARGE_PROJETC_BMP8_NIVEAU_DE_GRIS_H

typedef struct {
    unsigned char header[54];
    unsigned char colorTable[1024];
    unsigned char * data;
    unsigned int width;
    unsigned int height;
    unsigned int colorDepth;
    unsigned int dataSize;
} t_bmp8;





#endif //PAULGIRAULT_EDOUARDLAFARGE_PROJETC_BMP8_NIVEAU_DE_GRIS_H
