#ifndef ARTISTA_H
#define ARTISTA_H

#define TAM_STRING 50

typedef struct {
    char nome[TAM_STRING];
    char estilo[TAM_STRING];
    char tipo[TAM_STRING];
    int  numAlbuns;
} infoArtista;

typedef enum { 
    VERMELHO = 0, PRETO = 1 
} Cor;

typedef struct Artista {
    infoArtista info;
    struct Artista *esq, *dir, *pai;   
    Cor cor;
} Artista;


int cor (Artista *raiz);
void rotacionaEsq(Artista **raiz);
void rotacionaDir(Artista **raiz);
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

#endif 
