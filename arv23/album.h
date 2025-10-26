#ifndef ALBUM_23_H
#define ALBUM_23_H

#define TAM_STRING 50
typedef struct Musica Musica;

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

Album *criarNoAlbum(infoAlbum info, Album *fEsq, Album *fCen);
void adicionarInfoAlbum(Album **no, infoAlbum info, Album *subArvInfo);
Album *quebrarNoAlbum(Album **no, infoAlbum info, infoAlbum *sobe, Album *filhoDir);
Album* inserirNoAlbum(Album **raiz, Album *pai, infoAlbum info, infoAlbum *sobe);
int verificaInfosAlbum(Album *raiz, const char* titulo);
void imprimirArvAlbum(Album *raiz, int nivel);
void liberarArvAlbum(Album **raiz);
Album* buscarAlbum(Album *raiz, const char *titulo);
Album *buscarAlbum(Album *raiz, const char *titulo);
infoAlbum *buscarInfoAlbum(Album *raiz, const char *titulo);

int procurarAlbumPorTitulo23(Album *raizAlb, const char *tituloBuscado, const char *nomeDoArtista);
int procurarMusicaNosAlbunsEmOrdem23(Album *raizAlb, const char *tituloBuscado, const char *nomeDoArtista);


#endif
