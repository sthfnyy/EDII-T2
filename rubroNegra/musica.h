#ifndef MUSICA_H
#define MUSICA_H

#include "tipo.h"

#define TAM_STRING 50

typedef struct {
    char titulo[TAM_STRING];
    int minutos;
} infoMusica;

struct Musica {
    infoMusica info;
    Musica *proximo;
};


Musica *alocarNo(void);
void    preencherNo(Musica *lista);
int     inserirMusica(Musica **lista, Musica *no);
void    mostrarMusicas(Musica *lista);
Musica *buscarMusica(Musica *lista, char *tituloBusca);
int     removerMusica(Musica **lista, char *tituloRemover);
void    liberarListaMusicas(Musica *lista);

int encontrarMusicaPeloTitulo(Musica *inicioDaLista, const char *tituloBuscado);
int procurarMusicaNosAlbunsEmOrdem(Album *raizDosAlbuns, const char *tituloBuscado, const char *nomeDoArtista);
void percorrerArtistasAlbunsListasEBuscarMusica(Artista *raizDosArtistas, const char *tituloBuscado);

#endif
