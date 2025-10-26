#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "artista.h"
#include "album.h"
#include "musica.h"
#include "tipo.h"

#define TAM_STRING 50

void menuArtistas(Artista **raizArt);
void menuAlbuns(Artista *art);
void menuMusicas(Album *alb);

int main(void) {
    Artista *raizArt = NULL;

    carregarBiblioteca("/home/sthefany/Documentos/5Periodo/EDII/T2/EDII-T2/rubroNegra/biblioteca.txt", &raizArt); // Lê ao iniciar
    menuArtistas(&raizArt);
    salvarBiblioteca("/home/sthefany/Documentos/5Periodo/EDII/T2/EDII-T2/rubroNegra/biblioteca.txt", raizArt);    // Salva ao sair
    liberarArvore(raizArt);                         // Libera memória
    return 0;
}


// ---------- Menu Artistas ----------
void menuArtistas(Artista **raizArt) 
{
    int op = -1;

    do {
        printf("\n=== Biblioteca de Música ===\n");
        printf("0 - Sair\n");
        printf("1 - Listar artistas\n");
        printf("2 - Inserir artista\n");
        printf("3 - Buscar/Entrar no artista\n");
        printf("4 - Remover artista (remove todos os álbuns e músicas)\n");
        printf("5 - Buscar álbum (global)\n");
        printf("6 - Buscar música (global)\n");
        printf("7 - Experimento (30 buscas)\n");
        printf("Escolha: ");
        scanf("%d", &op);

        switch (op)
        {
        case 1:
            mostrarArtistas(*raizArt);
            break;
        case 2:
            infoArtista a = preencherArtista();
            a.numAlbuns = 0; // já faz no seu preencher; garanta a->albuns=NULL
            // a.albuns = NULL; // se existir no seu struct
            if (insercao(raizArt, alocaArtista(a)))
                printf("Artista inserido!\n");
            else
                printf("Duplicado: artista não inserido.\n");
            break;
        case 3:
            char nomeArtista[TAM_STRING];
            printf("Nome do artista: ");
            scanf(" %49[^\n]", nomeArtista);
            Artista *art = buscarArtista(*raizArt, nomeArtista);
            if (art) {
                menuAlbuns(art);
            } else {
                printf("Artista não encontrado.\n");
            }
            break;
        case 4:
            char nome[TAM_STRING];
            printf("Remover artista (ATENÇÃO: todos os álbuns e músicas serão removidos): \n");
            printf("Nome do artista: ");
            scanf(" %49[^\n]", nome);
            if (removeArtista(raizArt, nome))
                printf("Artista removido.\n");
            else
                printf("Artista não encontrado.\n");
            break;
        case 5: {
            char tituloAlb[TAM_STRING];
            printf("Titulo do album: ");
            scanf(" %49[^\n]", tituloAlb);

            printf("\n=== Resultado da busca global por album ===\n");
            percorrerArtistasEBuscarAlbum(*raizArt, tituloAlb);
            break;
        }
        case 6:
            char tit[TAM_STRING];
            printf("Título da música: ");
            scanf(" %49[^\n]", tit);
            printf("\n=== Resultado da busca global ===\n");
            percorrerArtistasAlbunsListasEBuscarMusica(*raizArt, tit);
            break;
        case 7:
            // Experimento: 30 buscas, imprime caminho e tempo
            // Ex.: ler 30 nomes, ou gerar; medir com clock()
            executarExperimentoBuscas(*raizArt); // mostre caminho percorrido e tempo total/médio
            break;
        
        default:
            printf("Opção inválida.\n");
            break;
        }
    } while (op != 0);
    printf("Saindo...\n");
}

// ---------- Menu Álbuns (de 1 artista) ----------
void menuAlbuns(Artista *art) {
    int op = -1;
    do {
        printf("\n=== Álbuns de %s ===\n", art->info.nome);
        printf("0 - Voltar\n");
        printf("1 - Listar álbuns\n");
        printf("2 - Inserir álbum\n");
        printf("3 - Buscar/Entrar no álbum\n");
        printf("4 - Remover álbum (remove músicas)\n");
        printf("Escolha: ");
        scanf("%d", &op); 
        
        switch (op)
        {
        case 1:
            // listar RB de álbuns
            mostrarAlbumPreOrdem(art->info.albuns); // ou in-order, como preferir
            break;
        case 2:
            infoAlbum ab = preencherAlbum();
            // ab.musica = NULL; // já seta no seu preencher
            if (insercaoAlbum(&art->info.albuns, criarNoAlbum(ab))) 
            {
                art->info.numAlbuns++;
                printf("Álbum inserido!\n");
            } else {
                printf("Álbum duplicado!\n");
            }
            break;
        case 3:
            char tituloAlbum[TAM_STRING];
            printf("Título do álbum: ");
            scanf(" %49[^\n]", tituloAlbum);
            Album *alb = BuscarNoPorTituloAlbum(art->info.albuns, tituloAlbum);
            if (alb) menuMusicas(alb);
            else     printf("Álbum não encontrado.\n");
            break;
        case 4:
            char titulo[TAM_STRING];
            printf("Remover álbum (ATENÇÃO: músicas serão removidas): ");
            scanf(" %49[^\n]", titulo);
            if (removeAlbum(&art->info.albuns, titulo)) {
                art->info.numAlbuns--;
                printf("Álbum removido.\n");
            } else {
                printf("Álbum não encontrado.\n");
            }
            break;
        default:
            printf("Opção inválida.\n");
            break;
        }   
    } while (op != 0);
}

// ---------- Menu Músicas (de 1 álbum) ----------
void menuMusicas(Album *alb) {
    int op = -1;
    do {
        printf("\n=== Músicas do álbum %s ===\n", alb->info.titulo);
        printf("0 - Voltar\n");
        printf("1 - Listar músicas\n");
        printf("2 - Inserir música\n");
        printf("3 - Buscar música\n");
        printf("4 - Remover música\n");
        printf("Escolha: ");
        scanf("%d", &op); 

        switch (op)
        {
        case 1:
            mostrarMusicas(alb->info.musica);
            break;
        case 2:
            Musica *musica = alocarNo();
            preencherNo(musica);
            if (inserirMusica(&alb->info.musica, musica))
                printf("Música inserida!\n");
            else {
                printf("Duplicada!\n");
                free(musica);
            }
            break;
        case 3:
            char nomeMusica[TAM_STRING];
            printf("Nome da música: ");
            scanf(" %49[^\n]", nomeMusica);
            Musica *achou = buscarMusica(alb->info.musica, nomeMusica);
            if (achou) printf("Encontrada: %s (%d min)\n", achou->info.titulo, achou->info.minutos);
            else       printf("Não encontrada.\n");
            
            break;
        case 4:
            char nomeMus[TAM_STRING];
            printf("Nome da música: ");
            scanf(" %49[^\n]", nomeMus);
            if (removerMusica(&alb->info.musica, nomeMus)) printf("Removida!\n");
            else                                        printf("Música não encontrada.\n");
            break;
        
        default:
            printf("Opção inválida.\n");
            break;
        }
    } while (op != 0);
}



//busca global de album não esta funcionando
//busca global de música não esta funcionando
//a remoção só remove se digitar duas vezes o nome do álbum ou artista