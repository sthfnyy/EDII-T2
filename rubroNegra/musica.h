#ifndef MUSICA_H
#define MUSICA_H

#define TAM_STRING 50

// Estrutura para armazenar as informações da música
typedef struct {
    char titulo[TAM_STRING];
    int minutos;
} infoMusica;

// Estrutura do nó da lista de Músicas
typedef struct Musica {
    infoMusica info;        // Contém os dados da música (título, minutos)
    struct Musica *proximo; // Ponteiro para a próxima música na lista ordenada
} Musica;


Musica *criarNo();
Musica* alocarNo();
void preencherNo(Musica* lista);
int inserirMusica(Musica **lista, Musica *no);
void mostrarMusicas(Musica *lista);
Musica *buscarMusica(Musica *lista, char *tituloBusca);
int removerMusica(Musica **lista, char *tituloRemover);
void liberarListaMusicas(Musica *lista) ;

#endif