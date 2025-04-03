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

t_bmp8 * bmp8_loadImage(const char * filename);
void bmp8_saveImage(const char * filename, t_bmp8 * img);
void bmp8_free(t_bmp8 * img);
void bmp8_printInfo(t_bmp8 * img);
void bmp8_negative(t_bmp8 * img);
void bmp8_brightness(t_bmp8 * img, int value);
void bmp8_threshold(t_bmp8 * img, int threshold);
/*
 * — header : représente l’en-tête du fichier BMP. Cet en-tête est composé de 54 octets.
— colorTable : représente la table de couleur de l’image. Cette table est composée de 1024 octets. On
rappelle que pour une image de 8 bits de profondeur, la table de couleur obligatoirement présente.
— data : représente les données de l’image. Ces données sont stockées sous forme de tableau d’octets.
— width : représente la largeur de l’image en pixels (Située à l’offset 18 de l’en-tête).
— height : représente la hauteur de l’image en pixels (Située à l’offset 22 de l’en-tête).
 */

#endif //PAULGIRAULT_EDOUARDLAFARGE_PROJETC_BMP8_NIVEAU_DE_GRIS_H
