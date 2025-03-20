cmake_minimum_required(VERSION 3.27)
project(PaulGirault_EdouardLafarge_projetC C)

set(CMAKE_C_STANDARD 99)

add_executable(PaulGirault_EdouardLafarge_projetC main.c
        bmp8_niveau_de_gris.c
        bmp8_niveau_de_gris.h
        menu.c
        menu.h
        bmp8_couleur.c
        bmp8_couleur.h)
