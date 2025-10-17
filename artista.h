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


// Aloca e inicializa um novo nó de artista (entra VERMELHO)
Artista *alocaArtista(infoArtista dados);

// Lê dados do artista via stdin
infoArtista preencherArtista(void);

// Insere mantendo propriedades de RB; 1 se inseriu, 0 se duplicado
int inserirArtista(Artista **raiz, Artista *no);

// Busca por nome (comparação lexicográfica)
Artista *buscarArtista(Artista *raiz, const char *nome);

// Impressão in-order (mostra [P]/[V] na frente)
void mostrarArtistas(Artista *raiz);

// Libera memória de toda a árvore
void destruirArvore(Artista *raiz);

#endif 
