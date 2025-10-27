#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "artista.h"   
#include "album.h"     
#include "musica.h"    
#include "tipo.h"

void menuMusicas(infoAlbum *albInfo);
void menuAlbuns(infoArtista *art);
void menuArtistas(Artista **raizArt);


int main(void)
{
    Artista *raizArt = NULL;

    carregarBiblioteca23("biblioteca_30.txt", &raizArt);
    menuArtistas(&raizArt); 
    salvarBiblioteca23("biblioteca_30.txt", raizArt);

    liberarArv(&raizArt);   
    return 0;
}


void menuArtistas(Artista **raizArt)
{
    int op = -1;
    infoArtista a = {0}, sobe;
    char nome[TAM_STRING];
    infoArtista *alvo = NULL;

    do {
        printf("\n=== Biblioteca (Árvore 2–3) ===\n");
        printf("0 - Sair\n");
        printf("1 - Listar artistas\n");
        printf("2 - Inserir artista\n");
        printf("3 - Entrar no artista\n");
        //printf("4 - Remover artista\n");        
        printf("5 - Buscar álbum (global)\n");     
        printf("6 - Buscar música (global)\n");    
        printf("7 - Experimento (30 buscas)\n");  

        printf("Escolha: "); scanf("%d", &op);

        switch (op){
        case 1:
            if (*raizArt == NULL){
                printf("(sem artistas)\n");
            }
            else{
                printf("=== Árvore 2–3 de Artistas ===\n");
                imprimirArv(*raizArt, 0);
            }
            break;
        case 2:
            printf("Nome: "); scanf(" %49[^\n]", a.nome);
            printf("Estilo: "); scanf(" %49[^\n]", a.estilo);
            printf("Tipo: "); scanf(" %49[^\n]", a.tipo);

            a.numAlbuns = 0;
            a.albuns = NULL;

            inserirArtista(raizArt, NULL, a, &sobe);
            printf("Artista inserido!\n");
            break;
        case 3:
            if (*raizArt != NULL)
            {
                printf("Nome do artista: ");
                scanf(" %49[^\n]", nome);

                alvo = buscarInfoArtista(*raizArt, nome);

                if (alvo != NULL)
                {
                    menuAlbuns(alvo);
                }
                else
                {
                    printf("Artista não encontrado.\n");
                }
            }
            else
            {
                printf("Sem artistas.\n");
            }
            break;
        //case 4:
            // TODO: remover artista (quando implementar a remoção 2–3)
            // break;
        case 5:
            char tituloBusca[TAM_STRING];
            printf("Título do álbum (busca global): ");
            scanf(" %49[^\n]", tituloBusca);

            printf("\n=== Resultado da busca global por \"%s\" ===\n", tituloBusca);
            percorrerArtistasEBuscarAlbum23(*raizArt, tituloBusca);
            break;
        case 6:
            char titulo[TAM_STRING];
            printf("Título da música (busca global): ");
            scanf(" %49[^\n]", titulo);

            printf("\n=== Resultado da busca global por música: \"%s\" ===\n", titulo);
            percorrerArtistasAlbunsListasEBuscarMusica23(*raizArt, titulo);
            break;
        case 7:
            executarExperimentoBuscas23(*raizArt);
            break;
        case 0:
            break;

        default:
            printf("Opção inválida.");
            break;
        }

    } while (op != 0);
}

void menuAlbuns(infoArtista *art)
{
    int op = -1;
    char titulo[TAM_STRING];
    infoAlbum ab = {0};
    infoAlbum sobe;
    infoAlbum *alvo = NULL;

    do {
        printf("\n=== Álbuns de %s ===\n", art->nome);
        printf("0 - Voltar\n");
        printf("1 - Listar álbuns\n");
        printf("2 - Inserir álbum\n");
        printf("3 - Entrar no álbum\n");
        /* printf("4 - Remover álbum\n"); // TODO: implementar remoção na 2–3 de álbuns */
        printf("Escolha: "); scanf("%d", &op);

        switch (op)
        {
        case 1:
            if (art->albuns == NULL){
                printf("(sem álbuns)\n");
            }else{
                printf("=== Árvore 2–3 de Álbuns ===\n");
                imprimirArvAlbum(art->albuns, 0);
            }
            break;
        case 2:
            printf("Título: "); scanf(" %49[^\n]", ab.titulo);
            printf("Ano de lançamento: "); scanf("%d", &ab.anoLancamento);
            printf("Qtd de músicas: "); scanf("%d", &ab.quantMusica);

            ab.musica = NULL;
            inserirNoAlbum(&art->albuns, NULL, ab, &sobe);
            art->numAlbuns++;

            printf("Álbum inserido!\n");
            break;
        case 3:
            if (art->albuns != NULL){
                printf("Título do álbum: "); scanf(" %49[^\n]", titulo);

                alvo = buscarInfoAlbum(art->albuns, titulo);

                if (alvo != NULL){
                    menuMusicas(alvo);
                }
                else{
                    printf("Álbum não encontrado.\n");
                }
            }else{
                printf("Sem álbuns.\n");
            }
            break;
        /* case 4:
            // TODO: remover álbum (quando implementar a remoção 2–3)
            // break;
        */
        case 0:
            break;
        default:
            printf("Opção inválida.");
            break;
        }
    } while (op != 0);
}

void menuMusicas(infoAlbum *albInfo)
{
    int op = -1;
    char nome[TAM_STRING];
    Musica *m = NULL;
    Musica *achada = NULL;

    do {
        printf("\n=== Músicas do álbum \"%s\" ===\n", albInfo->titulo);
        printf("0 - Voltar\n");
        printf("1 - Listar músicas\n");
        printf("2 - Inserir música\n");
        printf("3 - Buscar música\n");
        printf("4 - Remover música\n");
        printf("Escolha: "); scanf("%d", &op);

        switch (op)
        {
        case 1:
            mostrarMusicas(albInfo->musica);
            break;
        case 2:
            m = alocarNo();
            if (m != NULL)
            {
                preencherNo(m);

                if (inserirMusica(&albInfo->musica, m))
                {
                    printf("Música inserida!\n");
                }
                else
                {
                    printf("Música duplicada!\n");
                    free(m);
                }
            }
            else
            {
                printf("Erro ao criar música.\n");
            }
            break;
        case 3:
            printf("Título da música: "); scanf(" %49[^\n]", nome);

            achada = buscarMusica(albInfo->musica, nome);

            if (achada != NULL){
                printf("Encontrada: %s (%d min)\n", achada->info.titulo, achada->info.minutos);
            }else{
                printf("Música não encontrada.\n");
            }
            break;
        case 4:
            printf("Título da música: "); scanf(" %49[^\n]", nome);

            if (removerMusica(&albInfo->musica, nome)){
                printf("Música removida!\n");
            }
            else{
                printf("Música não encontrada.\n");
            }
            break;
        case 0:
            break;

        default:
            printf("Opção inválida.");
            break;
        }
    } while (op != 0);
}
