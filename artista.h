#ifndef ARTISTA_H
#define ARTISTA_H

#include <stddef.h>

/* --------- Tipos / Modelos --------- */

typedef struct {
    char nome[64];
    char estilo[32];
    char tipo[32];
    int  numAlbuns;
} infoArtista;

typedef enum { VERMELHO = 0, PRETO = 1 } Cor;

typedef struct Artista {
    infoArtista info;
    struct Artista *esq;   // filho à esquerda
    struct Artista *dir;   // filho à direita
    struct Artista *pai;   // ponteiro para o pai
    Cor cor;               // cor do nó na RB
} Artista;

/* --------- Funções públicas --------- */

// Aloca e inicializa um novo nó de artista (entra VERMELHO)
Artista *alocaArtista(infoArtista dados);

// Lê dados do artista via stdin
infoArtista preencherDadosArtista(void);

// Insere mantendo propriedades de RB; 1 se inseriu, 0 se duplicado
int inserirArtista(Artista **raiz, Artista *no);

// Busca por nome (comparação lexicográfica)
Artista *buscarArtista(Artista *raiz, const char *nome);

// Impressão in-order (mostra [P]/[V] na frente)
void mostrarArtistas(Artista *raiz);

// Libera memória de toda a árvore
void destruirArvore(Artista *raiz);

#endif /* ARTISTA_H */
