#ifndef TIPO_H
#define TIPO_H

/* Tipos comuns a todo o projeto */
typedef enum { VERMELHO = 0, PRETO = 1 } Cor;

typedef struct Artista Artista;
typedef struct Album   Album;
typedef struct Musica  Musica;

/* ==== Protótipos do experimento ==== */
void executarExperimentoBuscas(Artista *raizArt);

/* ==== Protótipos de carga/salvamento ==== */
int carregarBiblioteca(const char *caminho, Artista **raizArt);
int salvarBiblioteca  (const char *caminho, Artista *raizArt);

typedef struct {
    const Artista **nos;
    int qtd;
    int cap;
} CaminhoRB;

typedef struct {
    int    encontrado;       /* 1 = achou, 0 = não achou */
    int    nosVisitados;     /* quantidade de nós visitados */
    double tempo_ms;         /* duração da busca em milissegundos */
} ResultadoBusca;

#endif
