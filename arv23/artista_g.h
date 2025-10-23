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
void imprimirArtistasInOrdem(Artista *raiz) ;
Artista *buscarArtista(Artista *raiz, const char *nome);
void imprimirInOrdemArtistaComRaiz(Artista *raiz) ;
void liberarArtistas(Artista *raiz) ;

//=====FUNÇÃO QUE TO USANDO PARA REMOÇÃO =====//
Artista* removerArtista(Artista* raiz, const char* nome, int* diminuiu);
infoArtista obterPredecessorArtista(Artista* no);
void fundir(Artista *pai, int caso);
void redistribuiDaDireita(Artista *pai, int caso);
void redistribuiDaEsquerda(Artista *pai, int caso);
int ehFolha(Artista *no);
int verificarValidadeArvore(Artista* no, int nivel) ;

#endif 
