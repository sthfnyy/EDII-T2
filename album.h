// Arquivo: album.h
#ifndef ALBUM_H
#define ALBUM_H

#include "tipo.h"

#define TAM_STRING 50

typedef struct infoAlbum{
    char titulo[TAM_STRING];
    int anoLancamento;
    int quantMusica;
    struct Musica *musica; 
} infoAlbum;

typedef struct Album {
    infoAlbum info;
    struct Album *esq;
    struct Album *dir;
    struct Album *pai;
    Cor cor;
} Album;


Album *criarNo(infoAlbum album);
infoAlbum preencherAlbum(void);
int cor(Album *raiz);
void rotacionaEsq(Album **raiz);
void rotacionaDir(Album **raiz);    
void trocaCor(Album *raiz) ;
void balanceamento(Album **raiz);
int  insercao(Album **raiz, Album *novoNo) ;
int insereNo(Album **raiz, Album *novoNo);
void mostrarAlbumPreOrdem(Album *raiz);

Album *BuscarNoPorTitulo(Album *raiz, const char* tituloBusca);
Album *move2EsqRed(Album *raiz);
Album *move2DirRed(Album *raiz);
Album *removeMenor(Album *raiz);
Album *procuraMenor(Album *raiz);
Album* removeNo(Album *raiz, char valor);
void liberarArvore(Album *raiz);




#endif