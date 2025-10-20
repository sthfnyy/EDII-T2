#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tipo.h"
#include "album.h"

Album *criarNo(infoAlbum album)
{
    Album *novoNo = (Album*) malloc(sizeof(Album));
    if (novoNo != NULL)
    {
        novoNo->esq = NULL;
        novoNo->dir = NULL;
        novoNo->cor = VERMELHO;
        novoNo->info = album;
    }
    return novoNo;
}

infoAlbum preencherAlbum(void)
{
    infoAlbum dados;
    printf("Digite o titulo do album: ");
    setbuf(stdin, NULL);
    scanf("%[^\n]", dados.titulo);
    printf("Digite o ano de lancamento do album: ");
    scanf("%d", &dados.anoLancamento);
    printf("Digite a quantidade de musica do album: ");
    scanf("%d", &dados.quantMusica);
    dados.musica = NULL;
    return dados;
}

void liberarArvore(Album *raiz)
{
    if (raiz != NULL)
    {
        liberarArvore(raiz->esq);
        liberarArvore(raiz->dir);
        free(raiz);
    }
}

// Essa funcao inicia faz com que os nos folhas seja pretos
int cor(Album *raiz)
{
    int corNo = PRETO;
    if (raiz)
    {
        corNo = (raiz)->cor;
    }
    
    return corNo;
}

//tive que modificar e tirar do tipo void, pois na remoção não dava pra usar void
Album *rotacionaEsq(Album *raiz)
{
    Album *aux = (raiz)->dir;

    raiz->dir = aux->esq;
    aux->esq = raiz;
    aux->cor = raiz->cor;
    raiz->cor = VERMELHO;
    return aux;
}

Album *rotacionaDir(Album *raiz)
{
    Album *aux = raiz->esq;

    raiz->esq = aux->dir;
    aux->dir = raiz;
    aux->cor = raiz->cor;
    raiz->cor = VERMELHO;
    return aux;
}

void trocaCor(Album *raiz) 
{
    raiz->cor = !(raiz->cor); // troca a cor 
    if(raiz->esq != NULL)
        raiz->esq->cor = !(raiz->esq->cor);
    if(raiz->dir != NULL)
        raiz->dir->cor = !(raiz->dir->cor);
}

void balanceamento(Album **raiz) 
{
    if (cor((*raiz)->esq) == PRETO && cor((*raiz)->dir) == VERMELHO)
    {
        rotacionaEsq(raiz);
    } 

    if ((cor((*raiz)->esq) == VERMELHO) && (cor((*raiz)->esq->esq) == VERMELHO))
    {
        rotacionaDir(raiz);
    } 

    if (cor((*raiz)->esq) == VERMELHO && cor((*raiz)->dir) == VERMELHO)
    {
        trocaCor(*raiz); 
    } 
}

int insereNo(Album **raiz, Album *novoNo)
{
    int inseriu = 1;

    if (*raiz == NULL)
    {
        *raiz = novoNo;
    }else{
        int cmp = strcmp(novoNo->info.titulo, (*raiz)->info.titulo);

        if (cmp < 0)
        {
            inseriu = insereNo(&((*raiz)->esq), novoNo);
        }
        else if (cmp > 0)
        {
            inseriu = insereNo(&((*raiz)->dir), novoNo);
        }
        else
        {
            inseriu = 0; //nome duplicado
        }
            
        if (*raiz && inseriu)
        {
            balanceamento(raiz);
        }
    }    
    return inseriu;
}

int  insercao(Album **raiz, Album *novoNo) 
{
    int inseriu = insereNo(raiz, novoNo);

    if (inseriu && *raiz) 
        (*raiz)->cor = PRETO;

    return inseriu;
}


Album* BuscarNoPorTitulo(Album *raiz, const char* tituloBusca)
{
    // 1. Cria um ponteiro para o nó que será retornado. Inicia com NULL.
    Album *noEncontrado = NULL;

    // 2. Cria um ponteiro para percorrer a árvore, começando pela raiz.
    Album *noAtual = raiz;

    // 3. O laço continua enquanto não chegamos ao fim de um galho (noAtual != NULL)
    //    e enquanto ainda não encontramos o nó (noEncontrado == NULL).
    while (noAtual != NULL && noEncontrado == NULL)
    {
        // Compara o título buscado com o título do nó atual
        int comparacao = strcmp(tituloBusca, noAtual->info.titulo);

        if (comparacao == 0)
        {
            // Encontrou! Atribui o nó atual à variável de resultado.
            noEncontrado = noAtual;
        }
        else if (comparacao < 0)
        {
            // O título buscado vem antes, então desce para a esquerda.
            noAtual = noAtual->esq;
        }
        else // (comparacao > 0)
        {
            // O título buscado vem depois, então desce para a direita.
            noAtual = noAtual->dir;
        }
    }

    // 4. Retorna a variável de resultado.
    //    Se o nó foi encontrado, ela conterá o endereço dele.
    //    Se o laço terminou sem encontrar, ela ainda terá o valor NULL.
    return noEncontrado;
}


Album *move2EsqRed(Album *raiz)
{
    trocaCor(raiz);
    if (cor((*raiz).dir->esq) == VERMELHO)
    {
        rotacionaDir(&((*raiz).dir));
        rotacionaEsq(&raiz);
        trocaCor(raiz);
    }
    return raiz;
}

Album *move2DirRed(Album *raiz)
{
    trocaCor(raiz);
    if (cor((*raiz).esq->esq) == VERMELHO)
    {
        rotacionaDir(&raiz);
        trocaCor(raiz);
    }
    return raiz;
}

Album *removeMenor(Album *raiz)
{
    if ((*raiz).esq == NULL)
    {
        free(raiz);
        return NULL;
    }

    if (cor(raiz->esq) == PRETO && cor(raiz->esq->esq) == PRETO)
        raiz = move2EsqRed(raiz);

    raiz->esq = removeMenor(raiz->esq);
    return (raiz);
}

Album *procuraMenor(Album *raiz)
{
    Album *aux = raiz;

    while (aux->esq != NULL) 
    {
        aux = aux->esq;
    }
    return aux;
}

int removeAlbum(Album **raiz, char titulo) 
{
    int removeu = 0;
    if (BuscarNoPorTitulo (*raiz, titulo)) 
    {
        Album *no = *raiz;
        *raiz = removeNo(*raiz, titulo);
        if (*raiz != NULL) 
            (*raiz)->cor = PRETO;
        removeu = 1;
    }
    return removeu;
}

Album* removeNo(Album *raiz, const char *titulo) 
{
    Album *resultado = raiz;

    if (raiz != NULL)
    {    
        int cmp = strcmp(titulo, raiz->info.titulo);

        if (cmp < 0) 
        {
            if (cor((raiz)->esq) == PRETO && cor((raiz)->esq->esq) == PRETO) 
            
                raiz = move2EsqRed(raiz);
            
            raiz->esq = removeNo(raiz->esq, titulo);
            resultado = raiz; // ainda temos nó no topo desta subárvore
        }
        else 
        {
            if (cor((raiz)->esq) == VERMELHO) 
            {
                raiz = rotacionaDir(raiz);
            }

            if (cmp == 0 && (raiz->dir) == NULL) 
            {
                free(raiz);
                resultado =  NULL; //tirar esse return daqui
            }

            if (cor(raiz->dir) == PRETO && cor(raiz->dir->esq) == PRETO)
            {
                raiz = move2DirRed(raiz);
            } 

            if (cmp == 0) 
            {
                Album *menor = procuraMenor(raiz->dir);
                raiz->info = menor->info;
                raiz->dir = removeMenor(raiz->dir);
            } 
            else{ 
                raiz->dir = removeNo(raiz->dir, titulo);
            }
            resultado = raiz;  // seguimos com a subárvore atualizada
        }
        if (resultado != NULL) {
            balanceamento(&raiz);   
        }          
    }
    return resultado;  
}



// Impressão pre-order
void mostrarAlbumPreOrdem(Album *raiz)
{
    
    if (raiz != NULL)
    {
        // imprime primeiro o nó atual (raiz local)
        char corChar;
        if (raiz->cor == PRETO)
            corChar = 'P';
        else
            corChar = 'V';
        
        printf("[%c] Titulo: %s, Ano Lancamento: %d, Quantidade Musica: %d\n",
               corChar,
               raiz->info.titulo,
               raiz->info.anoLancamento,
               raiz->info.quantMusica);

        // depois percorre os filhos
        mostrarAlbumPreOrdem(raiz->esq);
        mostrarAlbumPreOrdem(raiz->dir);
    }
}


/* ======== Teste rápido ======== */

int main(void)
{
    Album *raiz = NULL;

    infoAlbum a = {"Epitafio", 2020, 5};
    infoAlbum b = {"Chuva",   2018,  2};
    infoAlbum c = {"Cavaquinho",  2025, 7};
    infoAlbum d = {"Guitarra",  2022, 1};

    insercao(&raiz, criarNo(a));
    insercao(&raiz, criarNo(b));
    insercao(&raiz, criarNo(c));
    insercao(&raiz, criarNo(d));

    printf("Lista em pre-ordem:\n");
    mostrarAlbumPreOrdem(raiz);

    Album *encontrado = BuscarNoPorTitulo(raiz, "Chuva");
    if (encontrado) printf("Achei %s\n", encontrado->info.titulo);

    liberarArvore(raiz);
    return 0;
}
