#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "album.h"
#include "musica.h"   
#include "artista.h" 

Musica *criar()
{
    return NULL;
}

Musica *alocarNo()
{
    Musica *novo = (Musica*) malloc(sizeof(Musica));
    if (novo == NULL)
    {
        printf("Erro ao alocar memoria.\n");
    }
    else
    {
        novo->proximo = NULL;
    }
    return novo;
}

void preencherNo(Musica *mus)
{
    printf("Digite o título da música: ");
    scanf(" %49[^\n]", mus->info.titulo);

    printf("Digite a duração (min): ");
    scanf("%d", &mus->info.minutos);
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

int encontrarMusicaPeloTitulo(Musica *inicio, const char *tituloBuscado)
{
    Musica *atual = inicio;
    int encontrada = 0;

    while (atual != NULL)
    {
        if (strcmp(atual->info.titulo, tituloBuscado) == 0)
        {
            printf("    Música encontrada: %s (%d min)\n",
                   atual->info.titulo,
                   atual->info.minutos);
            encontrada = 1;
        }
        atual = atual->proximo;
    }

    return encontrada;
}

