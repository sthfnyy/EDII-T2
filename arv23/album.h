#ifndef ALBUM_23_H
#define ALBUM_23_H

#define TAM_STRING 50

typedef struct Musica {
    char titulo[TAM_STRING];
    int minutos;
    struct Musica *proximo;
} Musica;

typedef struct infoAlbum {
    char titulo[TAM_STRING];
    int anoLancamento;
    int quantMusica;
    Musica *musica;
} infoAlbum;

typedef struct Album {
    infoAlbum infoUm;
    infoAlbum infoDois;
    int quantInfo;
    struct Album *esq;
    struct Album *cen;
    struct Album *dir;
    struct Album *pai;
} Album;

Album *criarNo(infoAlbum info, Album *fEsq, Album *fCen);
void adicionarInfo(Album **no, infoAlbum info, Album *subArvInfo);
Album *quebrarNo(Album **no, infoAlbum info, infoAlbum *sobe, Album *filhoDir);
Album* inserirNo(Album **raiz, Album *pai, infoAlbum info, infoAlbum *sobe);
int verificaInfos(Album *raiz, const char* titulo);
void imprimirArv(Album *raiz, int nivel);
void liberarArv(Album **raiz);
Album* buscarAlbum(Album *raiz, const char *titulo);


#endif
