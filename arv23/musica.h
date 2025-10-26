#ifndef MUSICA_H
#define MUSICA_H

#define TAM_STRING 50

typedef struct {
    char titulo[TAM_STRING];
    int minutos;
} infoMusica;

typedef struct Musica {
    infoMusica info;        
    struct Musica *proximo; 
} Musica;


Musica *criar();
Musica* alocarNo(void);
void preencherNo(Musica* lista);
int inserirMusica(Musica **lista, Musica *novaMusica);
void mostrarMusicas(Musica *lista);
Musica *buscarMusica(Musica *lista, char *tituloBusca);
int removerMusica(Musica **lista, char *tituloRemover);
void liberarListaMusicas(Musica *lista) ;

int encontrarMusicaPeloTitulo(Musica *inicio, const char *tituloBuscado);




#endif