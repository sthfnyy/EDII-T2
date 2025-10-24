#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

int inserirMusica(Musica **lista, Musica *no)
{
    int inseriu = 1;

    if (*lista == NULL)
    {
        *lista = no;
        no->proximo = NULL;
    }
    else
    {
        int duplicata = 0;
        Musica *aux = *lista;

        while (aux->proximo != NULL && duplicata != 1)
        {
            if (strcmp(aux->info.titulo, no->info.titulo) == 0)
            {
                duplicata = 1;
            }
            aux = aux->proximo;
        }

        if (strcmp(aux->info.titulo, no->info.titulo) == 0)
        {
            duplicata = 1;
        }

        if (duplicata == 0)
        {
            aux->proximo = no;
            no->proximo = NULL;
        }
        else
        {
            inseriu = 0;
        }
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
        printf("Sem categorias!");
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

        // Caso especial: primeiro nó
        if (strcmp(aux->info.titulo, tituloRemover) == 0)
        {
            *lista = aux->proximo;
            free(aux);
            removeu = 1;
        }
        else
        {
            // Percorre até achar ou acabar
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

int main()
{
    Musica *listaMusicas = NULL;
    int opcao;

    do
    {
        printf("\n--- MENU MUSICA ---\n");
        printf("1 - Inserir musica\n");
        printf("2 - Mostrar musicas\n");
        printf("3 - Buscar musica\n");
        printf("4 - Remover musica\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        setbuf(stdin, NULL);

        if (opcao == 1)
        {
            Musica *novo = alocarNo();
            preencherNo(novo);

            if (inserirMusica(&listaMusicas, novo))
                printf("Musica inserida com sucesso!\n");
            else
                printf("Erro: musica duplicada!\n");
        }
        else if (opcao == 2)
        {
            mostrarMusicas(listaMusicas);
        }
        else if (opcao == 3)
        {
            char busca[50];
            printf("Digite o titulo da musica para buscar:\n");
            scanf("%[^\n]", busca);
            setbuf(stdin, NULL);

            Musica *achada = buscarMusica(listaMusicas, busca);
            if (achada)
                printf("Musica encontrada: %s (%d min)\n", achada->info.titulo, achada->info.minutos);
            else
                printf("Musica nao encontrada!\n");
        }
        else if (opcao == 4)
        {
            char remover[50];
            printf("Digite o titulo da musica para remover:\n");
            scanf("%[^\n]", remover);
            setbuf(stdin, NULL);

            if (removerMusica(&listaMusicas, remover))
                printf("Musica removida com sucesso!\n");
            else
                printf("Musica nao encontrada!\n");
        }

    } while(opcao != 0);

    printf("Fim do programa!\n");
    return 0;
} 