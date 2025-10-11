// Arquivo: album.h
#ifndef ALBUM_H
#define ALBUM_H

#include "tipo.h"

#define TAM_STRING 50

typedef struct {
    char titulo[TAM_STRING];
    int anoLancamento;
    int quantMusica;
    struct Musica *musica; // Ponteiro para a lista de músicas
} infoAlbum;

typedef struct Album {
    infoAlbum info;
    struct Album *esq;
    struct Album *dir;
    struct Album *pai;
    Cor cor;
} Album;

#endif