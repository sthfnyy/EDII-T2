#include "artista.h"


typedef struct {
    int    encontrado;    /* 1 = achou, 0 = não achou */
    int    nosVisitados;  /* nós visitados (conta por nó, não por chave) */
    double tempo_ms;      /* duração da busca em milissegundos */
} ResultadoBusca;


typedef struct {
    const Artista **nosVisitados;  /* vetor de ponteiros para nós visitados */
    int quantidade;                /* quantos nós foram registrados */
    int capacidade;                /* tamanho atual do vetor alocado */
} CaminhoBusca;


int carregarBiblioteca23(const char *caminho, Artista **raizArt);
int salvarBiblioteca23  (const char *caminho, Artista *raizArt);

void executarExperimentoBuscas23(Artista *raizArt);
