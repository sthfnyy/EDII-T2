#ifndef TIPO_H
#define TIPO_H

/* Tipos e forward declarations comuns a todo o projeto */
typedef enum { VERMELHO = 0, PRETO = 1 } Cor;

typedef struct Artista Artista;
typedef struct Album   Album;
typedef struct Musica  Musica;

/* ==== Protótipos do experimento ==== */
void executarExperimentoBuscas(Artista *raizArt);

/* ==== Protótipos de carga/salvamento ==== */
int carregarBiblioteca(const char *caminho, Artista **raizArt);
int salvarBiblioteca  (const char *caminho, Artista *raizArt);

#endif
