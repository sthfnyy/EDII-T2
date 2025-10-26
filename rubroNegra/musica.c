#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "album.h"
#include "musica.h"   
#include "artista.h" 

Musica *criarNo()
{
    return NULL;
}

Musica* alocarNo()
{
    Musica *novoNo = (Musica*) malloc(sizeof(Musica));
    if (novoNo == NULL)
    {
        printf("Erro ao alocar memoria\n");
    }

    novoNo->proximo = NULL;
    return novoNo;
}

void preencherNo(Musica* lista)
{
    printf("Digite o nome da musica:\n");
    setbuf(stdin, NULL);
    scanf("%[^\n]", lista->info.titulo);

    printf("Digite a duração da musica:\n");
    scanf("%d", &lista->info.minutos);
}

int inserirMusica(Musica **lista, Musica *novaMusica)
{
    int inseriu = 1;

    if (novaMusica != NULL)
    {
        if (*lista == NULL || strcmp(novaMusica->info.titulo, (*lista)->info.titulo) < 0)
        {
            // Inserir no início
            novaMusica->proximo = *lista;
            *lista = novaMusica;
        }
        else
        {
            Musica *anterior = *lista;
            Musica *atual = (*lista)->proximo;
            int duplicata = 0;

            // Percorre até achar a posição correta (ordem alfabética)
            while (atual != NULL && strcmp(novaMusica->info.titulo, atual->info.titulo) > 0)
            {
                anterior = atual;
                atual = atual->proximo;
            }

            // Verifica duplicata
            if ((atual != NULL && strcmp(novaMusica->info.titulo, atual->info.titulo) == 0) ||
                (strcmp(novaMusica->info.titulo, anterior->info.titulo) == 0))
            {
                duplicata = 1;
            }

            if (duplicata == 0)
            {
                novaMusica->proximo = atual;
                anterior->proximo = novaMusica;
            }
            else
            {
                inseriu = 0;
            }
        }
    }
    else
    {
        inseriu = 0;
    }

    return inseriu;
}


void mostrarMusicas(Musica *lista)
{
    if (lista)
    {
        Musica *aux = lista;
        printf("Musicas:\n");

        while (aux != NULL)
        {
            printf("%s\n", aux->info.titulo);
            aux = aux->proximo;
        }
    }
    else
    {
        printf("Sem musicas!");
    }  
}

Musica *buscarMusica(Musica *lista, char *tituloBusca)
{
    Musica *aux = NULL;

    if (lista)
    {
        aux = lista;
        int achou = 0;

        while (aux != NULL && achou == 0)
        {
            if (strcmp(aux->info.titulo, tituloBusca) == 0)
            {
                achou = 1;
            }
            else
            {
                aux = aux->proximo;
            }
        }

        if (achou == 0)
        {
            aux = NULL;
        }

        return aux;
    }

    return NULL;
}

int removerMusica(Musica **lista, char *tituloRemover)
{
    int removeu = 0;

    if (*lista != NULL)
    {
        Musica *aux = *lista;
        Musica *anterior = NULL;

        if (strcmp(aux->info.titulo, tituloRemover) == 0)
        {
            *lista = aux->proximo;
            free(aux);
            removeu = 1;
        }
        else
        {
            while (aux != NULL && removeu == 0)
            {
                if (strcmp(aux->info.titulo, tituloRemover) == 0)
                {
                    anterior->proximo = aux->proximo;
                    free(aux);
                    removeu = 1;
                }
                else
                {
                    anterior = aux;
                    aux = aux->proximo;
                }
            }
        }
    }

    return removeu;
}

void liberarListaMusicas(Musica *lista) 
{
    while (lista) {
        Musica *aux = lista->proximo;
        free(lista);
        lista = aux;
    }
}


// percorre lista ligada de músicas e compara por título
int encontrarMusicaPeloTitulo(Musica *inicioDaLista, const char *tituloBuscado) {
    Musica *musicaAtual = inicioDaLista;
    int encontrada = 0;

    while (musicaAtual != NULL) {
        if (strcmp(musicaAtual->info.titulo, tituloBuscado) == 0) {
            printf("    Música encontrada: %s (%d min)\n",
                   musicaAtual->info.titulo,
                   musicaAtual->info.minutos);
            encontrada = 1;
        }
        musicaAtual = musicaAtual->proximo;
    }
    return encontrada;
}

// percorre a RB de álbuns de UM artista (in-order) e procura a MÚSICA dentro de cada álbum
int procurarMusicaNosAlbunsEmOrdem(Album *raizDosAlbuns, const char *tituloBuscado, const char *nomeDoArtista)
{
    int musicaEncontrada = 0;

    if (raizDosAlbuns != NULL) {
        if (procurarMusicaNosAlbunsEmOrdem(raizDosAlbuns->esq, tituloBuscado, nomeDoArtista)) {
            musicaEncontrada = 1;
        }

        if (encontrarMusicaPeloTitulo(raizDosAlbuns->info.musica, tituloBuscado)) {
            printf("  >> Artista: %s | Álbum: %s (%d)\n",
                   nomeDoArtista,
                   raizDosAlbuns->info.titulo,
                   raizDosAlbuns->info.anoLancamento);
            musicaEncontrada = 1;
        }

        if (procurarMusicaNosAlbunsEmOrdem(raizDosAlbuns->dir, tituloBuscado, nomeDoArtista)) {
            musicaEncontrada = 1;
        }
    }

    return musicaEncontrada;
}

void percorrerArtistasAlbunsListasEBuscarMusica(Artista *raizDosArtistas,
                                                const char *tituloBuscado) {
    if (raizDosArtistas != NULL) {
        percorrerArtistasAlbunsListasEBuscarMusica(raizDosArtistas->esq, tituloBuscado);

        int encontradaNesteArtista =
            procurarMusicaNosAlbunsEmOrdem(raizDosArtistas->info.albuns,
                                           tituloBuscado,
                                           raizDosArtistas->info.nome);

        if (encontradaNesteArtista) {
            printf("--------------------------------------------\n");
        }

        percorrerArtistasAlbunsListasEBuscarMusica(raizDosArtistas->dir, tituloBuscado);
    }
}




// int main()
// {
//     Musica *listaMusicas = NULL;
//     int opcao;

//     do
//     {
//         printf("\n--- MENU MUSICA ---\n");
//         printf("1 - Inserir musica\n");
//         printf("2 - Mostrar musicas\n");
//         printf("3 - Buscar musica\n");
//         printf("4 - Remover musica\n");
//         printf("0 - Sair\n");
//         printf("Escolha: ");
//         scanf("%d", &opcao);
//         setbuf(stdin, NULL);

//         if (opcao == 1)
//         {
//             Musica *novo = alocarNo();
//             preencherNo(novo);

//             if (inserirMusica(&listaMusicas, novo))
//                 printf("Musica inserida com sucesso!\n");
//             else
//                 printf("Erro: musica duplicada!\n");
//         }
//         else if (opcao == 2)
//         {
//             mostrarMusicas(listaMusicas);
//         }
//         else if (opcao == 3)
//         {
//             char busca[50];
//             printf("Digite o titulo da musica para buscar:\n");
//             scanf("%[^\n]", busca);
//             setbuf(stdin, NULL);

//             Musica *achada = buscarMusica(listaMusicas, busca);
//             if (achada)
//                 printf("Musica encontrada: %s (%d min)\n", achada->info.titulo, achada->info.minutos);
//             else
//                 printf("Musica nao encontrada!\n");
//         }
//         else if (opcao == 4)
//         {
//             char remover[50];
//             printf("Digite o titulo da musica para remover:\n");
//             scanf("%[^\n]", remover);
//             setbuf(stdin, NULL);

//             if (removerMusica(&listaMusicas, remover))
//                 printf("Musica removida com sucesso!\n");
//             else
//                 printf("Musica nao encontrada!\n");
//         }

//     } while(opcao != 0);

//     printf("Fim do programa!\n");
//     return 0;
// } 
