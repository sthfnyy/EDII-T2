#ifndef ALBUM_H
#define ALBUM_H

#include "tipo.h"

#define TAM_STRING 50

typedef struct infoAlbum{
    char titulo[TAM_STRING];
    int anoLancamento;
    int quantMusica;
    Musica *musica;     // usa forward de tipo.h
} infoAlbum;

struct Album {
    infoAlbum info;
    Album *esq, *dir, *pai;
    Cor cor;
};

/* API da RB de álbuns */
Album  *criarNoAlbum(infoAlbum album);
infoAlbum preencherAlbum(void);

int     corAlbum(Album *raiz);
Album  *rotacionaEsqAlbum(Album *raiz);
Album  *rotacionaDirAlbum(Album *raiz);
void    trocaCorAlbum(Album *raiz);
void    balanceamentoAlbum(Album **raiz);

int     insereNoAlbum(Album **raiz, Album *novoNo);
int     insercaoAlbum(Album **raiz, Album *novoNo);

Album  *BuscarNoPorTituloAlbum(Album *raiz, const char *tituloBusca);
Album  *move2EsqRedAlbum(Album *raiz);
Album  *move2DirRedAlbum(Album *raiz);
Album  *removeMenorAlbum(Album *raiz);
Album  *procuraMenorAlbum(Album *raiz);

int     removeAlbum(Album **raiz, const char *titulo);
Album  *removeNoAlbum(Album *raiz, const char *titulo);

void    mostrarAlbumPreOrdem(Album *raiz);
void    liberarArvoreAlbum(Album *raiz);

/* Busca global (varre artistas e seus álbuns) */
int procurarAlbumPorTitulo(Album *raizDosAlbuns,const char *tituloBuscado, const char *nomeDoArtista);
void percorrerArtistasEBuscarAlbum(Artista *raizDosArtistas, const char *tituloBuscado);

#endif