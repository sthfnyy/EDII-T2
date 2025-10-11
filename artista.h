#ifndef ARTISTA_H
#define ARTISTA_H

#include <stddef.h>
#include "tipo.h" // Inclui Cor e a declaração de struct Album

// Inclua o cabeçalho completo do Album aqui, pois infoArtista precisa dele.
#include "album.h" 
/* --------- Tipos / Modelos --------- */

typedef struct {
    char nome[64];
    char estilo[32];
    char tipo[32];
    int  numAlbuns;
    Album *album;
} infoArtista;

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
