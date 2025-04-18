#include "bmp8_couleur.h"

char image_c[] = "../DATA/lena_color.bmp";
void bmp24_readPixelData(FILE *file, t_bmp24 *img) {
    int padding = (4 - (img->width * 3) % 4) % 4;
    fseek(file, img->header.offset, SEEK_SET);

    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            uint8_t bgr[3];
            fread(bgr, sizeof(uint8_t), 3, file);
            img->data[img->height - 1 - i][j].blue  = bgr[0];
            img->data[img->height - 1 - i][j].green = bgr[1];
            img->data[img->height - 1 - i][j].red   = bgr[2];
        }
        fseek(file, padding, SEEK_CUR);
    }
}
void bmp24_printInfo(t_bmp24 *img) {
    if (!img) {
        printf("Aucune image fournie.\n");
        return;
    }

    printf("\n🔍 Informations de l'image BMP 24 bits :\n");
    printf("➡️  Dimensions      : %d x %d pixels\n", img->width, img->height);
    printf("🎨 Profondeur      : %d bits\n", img->colorDepth);
    printf("📦 Taille image RAW: %u octets\n", img->header_info.imagesize);
    printf("📁 Offset des données : %u (octet %d)\n", img->header.offset, BITMAP_OFFSET);
    printf("🖼️ Compression      : %u\n", img->header_info.compression);
    printf("🖨️ Résolution       : %d x %d dpi\n", img->header_info.xresolution, img->header_info.yresolution);
}
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

t_bmp24 * bmp24_loadImage(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Erreur : impossible d'ouvrir le fichier %s.\n", filename);
        return NULL;
    }

    // Lire largeur, hauteur, profondeur depuis les bons offsets
    uint32_t width, height;
    uint16_t depth;

    file_rawRead(BITMAP_WIDTH, &width, sizeof(uint32_t), 1, file);
    file_rawRead(BITMAP_HEIGHT, &height, sizeof(uint32_t), 1, file);
    file_rawRead(BITMAP_DEPTH, &depth, sizeof(uint16_t), 1, file);

    if (depth != DEFAULT_DEPTH) {
        printf("Erreur : profondeur différente de 24 bits (%d lue).\n", depth);
        fclose(file);
        return NULL;
    }

    // Allouer l'image
    t_bmp24 *img = bmp24_allocate(width, height, depth);
    if (!img) {
        fclose(file);
        return NULL;
    }

    // Lire les en-têtes via file_rawRead
    file_rawRead(BITMAP_MAGIC, &img->header, sizeof(t_bmp_header), 1, file);
    file_rawRead(HEADER_SIZE, &img->header_info, sizeof(t_bmp_info), 1, file);

    // Lire les pixels
    bmp24_readPixelData(file, img);

    fclose(file);
    return img;
}

void bmp24_negative(t_bmp24 *img) {
    if (!img || !img->data) return;

    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            t_pixel *p = &img->data[i][j];
            p->red   = 255 - p->red;
            p->green = 255 - p->green;
            p->blue  = 255 - p->blue;
        }
    }
}
void bmp24_grayscale(t_bmp24 *img) {
    if (!img || !img->data) return;

    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            t_pixel *p = &img->data[i][j];
            uint8_t average = (p->red + p->green + p->blue) / 3;
            p->red = p->green = p->blue = average;
        }
    }
}


void bmp24_brightness (t_bmp24 * img, int value) {
    if (!img || !img->data) return;
    printf("De combien voulez vous augmenter/baisser la luminosité de l'image : ");
    scanf("%d", &value);
    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            t_pixel *p = &img->data[i][j];
            p->red   = value + p->red;
            p->green = value + p->green;
            p->blue  = value + p->blue;
                if (p->red > 255 ){
                    p->red   = 255;
                    if(p->blue > 255){
                    p->blue=255;
                    if(p->green>255){
                     p->green=255;
                        }
             }
            }
        }
    }
}


t_pixel bmp24_convolution(t_bmp24 *img, int x, int y, float **kernel, int kernelSize) {
    int half = kernelSize / 2;
    float red = 0, green = 0, blue = 0;

    for (int i = -half; i <= half; i++) {
        for (int j = -half; j <= half; j++) {
            int px = x + j;
            int py = y + i;

            // Gestion des bords : on ignore les pixels hors image
            if (px >= 0 && px < img->width && py >= 0 && py < img->height) {
                t_pixel p = img->data[py][px];
                float coeff = kernel[i + half][j + half];
                red   += p.red * coeff;
                green += p.green * coeff;
                blue  += p.blue * coeff;
            }
        }
    }

    // Clamp les valeurs entre 0 et 255
    t_pixel result;
    result.red = (uint8_t) fmin(fmax(red, 0), 255);
    result.green = (uint8_t) fmin(fmax(green, 0), 255);
    result.blue = (uint8_t) fmin(fmax(blue, 0), 255);
    return result;
}
t_bmp24 *bmp24_applyFilter(t_bmp24 *img, float **kernel, int kernelSize) {
    t_bmp24 *output = bmp24_allocate(img->width, img->height, img->colorDepth);
    if (!output) return NULL;

    output->header = img->header;
    output->header_info = img->header_info;

    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            output->data[y][x] = bmp24_convolution(img, x, y, kernel, kernelSize);
        }
    }

    return output;
}


float **boxBlurKernel() {
    float **k = malloc(3 * sizeof(float*));
    for (int i = 0; i < 3; i++) {
        k[i] = malloc(3 * sizeof(float));
        for (int j = 0; j < 3; j++) {
            k[i][j] = 1.0f / 9.0f;
        }
    }
    return k;
}


float **gaussianBlurKernel() {
    float values[3][3] = {
            {1, 2, 1},
            {2, 4, 2},
            {1, 2, 1}
    };
    float **k = malloc(3 * sizeof(float*));
    for (int i = 0; i < 3; i++) {
        k[i] = malloc(3 * sizeof(float));
        for (int j = 0; j < 3; j++) {
            k[i][j] = values[i][j] / 16.0f;
        }
    }
    return k;
}


float **outlineKernel() {
    float values[3][3] = {
            {-1, -1, -1},
            {-1,  8, -1},
            {-1, -1, -1}
    };
    float **k = malloc(3 * sizeof(float*));
    for (int i = 0; i < 3; i++) {
        k[i] = malloc(3 * sizeof(float));
        for (int j = 0; j < 3; j++) {
            k[i][j] = values[i][j];
        }
    }
    return k;
}


float **embossKernel() {
    float values[3][3] = {
            {-2, -1,  0},
            {-1,  1,  1},
            { 0,  1,  2}
    };
    float **k = malloc(3 * sizeof(float*));
    for (int i = 0; i < 3; i++) {
        k[i] = malloc(3 * sizeof(float));
        for (int j = 0; j < 3; j++) {
            k[i][j] = values[i][j];
        }
    }
    return k;
}



float **sharpenKernel() {
    float values[3][3] = {
            { 0, -1,  0},
            {-1,  5, -1},
            { 0, -1,  0}
    };
    float **k = malloc(3 * sizeof(float*));
    for (int i = 0; i < 3; i++) {
        k[i] = malloc(3 * sizeof(float));
        for (int j = 0; j < 3; j++) {
            k[i][j] = values[i][j];
        }
    }
    return k;
}
/*
 * Pour appeler la fonction et la libérer
 * t_bmp24 *blurred = bmp24_gaussianBlur(img);

// 3. Sauvegarder l'image modifiée
bmp24_saveImage(blurred, "output_gaussian.bmp");

// 4. Libérer la mémoire
bmp24_free(img);
bmp24_free(blurred);
}
*/
void bmp24_saveImage(t_bmp24 *img, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Erreur ouverture fichier écriture");
        return;
    }

    fwrite(&img->header, sizeof(t_bmp_header), 1, file);
    fwrite(&img->header_info, sizeof(t_bmp_info), 1, file);

    int width = img->width;
    int height = img->height;
    int padding = (4 - (width * 3) % 4) % 4;
    uint8_t pad[3] = {0, 0, 0};

    fseek(file, img->header.offset, SEEK_SET);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            uint8_t bgr[3] = {
                img->data[height - 1 - i][j].blue,
                img->data[height - 1 - i][j].green,
                img->data[height - 1 - i][j].red
            };
            fwrite(bgr, sizeof(uint8_t), 3, file);
        }
        fwrite(pad, sizeof(uint8_t), padding, file);
    }

    fclose(file);
}