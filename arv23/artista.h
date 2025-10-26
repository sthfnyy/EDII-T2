#ifndef ARTISTA_H
#define ARTISTA_H

#define TAM_STRING 50

typedef struct Musica Musica;
typedef struct Album Album;

typedef struct {
    char nome[TAM_STRING];
    char estilo[TAM_STRING];
    char tipo[TAM_STRING];
    int  numAlbuns;
    Album *albuns;

} infoArtista;

typedef struct Artista {
    infoArtista infoUm;
    infoArtista infoDois;
    int  Ninfos;
    struct Artista *esq, *cen,*dir;   
   
} Artista;


//funções
Artista *alocaArtista (infoArtista dados, Artista *fEsq, Artista *fCen);
infoArtista preencherArtista(void);
void imprimirArv(Artista *raiz, int nivel) ;
void liberarArv(Artista **raiz);
void inserir23(Artista **novoNo, infoArtista dado, Artista *subArvInfo) ;
Artista *quebrarNoArtista(Artista **no, infoArtista dado, infoArtista *sobe, Artista *filhoDir) ;
Artista *inserirArtista(Artista **raiz, Artista *pai, infoArtista dado, infoArtista *sobe);
int verificaDados(Artista *raiz, const char *nome) ;
Artista *buscarArtista(Artista *raiz, const char *nome);
infoArtista *buscarInfoArtista(Artista *raiz, const char *nome);

#endif 