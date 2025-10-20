#ifndef ARTISTA_H
#define ARTISTA_H

#define TAM_STRING 50

typedef struct {
    char nome[TAM_STRING];
    char estilo[TAM_STRING];
    char tipo[TAM_STRING];
    int  numAlbuns;
} infoArtista;

typedef struct Artista {
    infoArtista infoUm;
    infoArtista infoDois;
    int  Ninfos;
    struct Artista *esq, *cen,*dir;   
   
} Artista;


//funções
Artista *alocaArtista(infoArtista dados);
infoArtista preencherArtista(void);
void inserir23(Artista **novoNo, infoArtista dado, Artista *subArvInfo) ;
Artista *quebrarNoArtista(Artista **no, infoArtista dado, infoArtista *sobe, Artista *filhoDir) ;
Artista *inserirArtista(Artista **raiz, Artista *pai, infoArtista dado, infoArtista *sobe);
void imprimirArtistas(Artista *raiz);
Artista *buscarArtista(Artista *raiz, const char *nome);
void liberarArtistas(Artista *raiz) ;

#endif 