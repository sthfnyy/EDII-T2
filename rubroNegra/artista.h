#ifndef ARTISTA_H
#define ARTISTA_H

#include "album.h"
#include "tipo.h"

#define TAM_STRING 50

typedef struct {
    char nome[TAM_STRING];
    char estilo[TAM_STRING];
    char tipo[TAM_STRING];
    int  numAlbuns;
    Album *albuns;  // árvore rubro-negra de álbuns
} infoArtista;


typedef struct Artista {
    infoArtista info;
    struct Artista *esq, *dir, *pai;   
    Cor cor;

} Artista;


int cor (Artista *raiz);
Artista *rotacionaEsq(Artista *raiz);
Artista *rotacionaDir(Artista *raiz);
void trocaCor(Artista *raiz);
void balanceamento(Artista **raiz);

Artista *alocaArtista(infoArtista dados);
infoArtista preencherArtista(void);
int inserirArtista(Artista **raiz, Artista *novoNo);
int  insercao(Artista **raiz, Artista *novoNo) ;
Artista *buscarArtista(Artista *raiz, const char *nome);
void mostrarArtistas(Artista *raiz);
void mostrarArtistasPreOrdem(Artista *raiz);
void liberarArvore(Artista *raiz);

Artista* move2EsqRed(Artista *raiz);
Artista* move2DirRed(Artista *raiz);
Artista *removeMenor(Artista *raiz);
Artista *procuraMenor(Artista *raiz);
int removeArtista(Artista **raiz, const char *nomeArtista) ;
Artista* removeNo(Artista *raiz, const char *nomeArtista) ;

#endif 
