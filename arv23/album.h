#ifndef INTERFACE_AVP_H
#define INTERFACE_AVP_H

#define TAM_STRING 50


typedef struct cidade Cidade;
typedef struct cep CEP;

// Estrutura infoEstado
typedef struct infoEstado 
{
    char nome_estado[TAM_STRING];
    char nome_capital[TAM_STRING];
    int num_cidade;
    int populacao;
    Cidade *arv_cidade;  // ponteiro para árvore de cidades
} infoEstado;

// Estrutura Estado (lista duplamente encadeada)
typedef struct estado 
{
    infoEstado infoEstado; 
    struct estado *prox;
    struct estado *ant;
} Estado;

typedef struct infoCidade 
{
    char nome_cidade[TAM_STRING];
    int populacao_c;
    CEP *avr_CEP;
} infoCidade;

typedef struct cidade Cidade;
struct cidade 
{
    infoCidade info_um;
    infoCidade info_dois;
    int  qntd_info;
    Cidade *esq, *cen, *dir;
};

typedef struct infoCEP 
{
    int cep;
} infoCEP;

typedef struct cep CEP;

struct cep 
{
    infoCEP info_um;
    infoCEP info_dois;
    int  qntd_info;
    CEP *esq, *cen, *dir;
};

typedef struct infoPessoa 
{
    int cepCidadeNatal;
    int cepCidadeAtual;
    char nome[TAM_STRING];
    char CPF[TAM_STRING];
    char nascimento[TAM_STRING];
} infoPessoa;

typedef struct pessoa Pessoa;

struct pessoa 
{
    infoPessoa info_um;
    infoPessoa info_dois;
    int qntd_info;
    Pessoa *esq, *cen, *dir;
};

Estado* alocar_estado();
void preencher_estado(infoEstado *estado, char *nome_estado, char *nome_capital, int n_cidade, int populacao);
int inserir_estado_ordenado(Estado **inicio, Estado *novo);
void imprimir_estados(Estado *inicio);
Estado* buscar_estado(Estado *inicio, const char *nome_busca, int *flag);
Estado* buscar_estado_Por_CEP(Estado *inicio, int intCEP);
void liberar_lista_estados(Estado *inicio);
Estado *retorna_estados_com_maior_populacao(Estado *atual);
Cidade *buscar_Cidade_nos_Estados(Estado *inicio, char *nome_cidade);


void preencherCidade(infoCidade *cidade, char *nome, int pop);
Cidade *inserirCidade(Cidade **raiz, Cidade *pai, infoCidade dado, infoCidade *sobe);
Cidade* buscarCidade(Cidade *raiz, char *valor);
void imprimirInOrdemCidadeComRaiz(Cidade *raiz);
void liberar_arvore_cidades(Cidade **raiz);
Cidade *buscarCidadePorCEP(Cidade *raiz, int valorCEP);
infoCidade cidade_mais_populosa_de_um_estado_sem_ser_a_capital(Cidade *raiz, char *nome_capital);

void preencherCEP(infoCEP *dados, int numCEP);
CEP *inserirCEP(CEP **raiz, CEP *pai, infoCEP dado, infoCEP *sobe);
CEP* buscarCEP(CEP *raiz, int valor);
void imporimirInOrdemCEP(CEP *raiz);
void imprimirInOrdemCEPComRaiz(CEP *raiz);
void liberar_arvore_CEPs(CEP **raiz);
int removerCEP(CEP **raiz, int valor);

void preencherPessoa(infoPessoa *info_pessoa, int cepCidadeNatal,int cepCidadeAtual, char *nome, char *CPF, char *nascimento);
Pessoa *inserirPessoa(Pessoa **raiz, Pessoa *pai, infoPessoa dado, infoPessoa *sobe);
void imprimirInOrdemPessoaComRaiz(Pessoa *raiz);
Pessoa* buscarPessoa(Pessoa *raiz, char *valor);
int removerPessoa(Pessoa **raiz, char *valor);
Pessoa *consultaPessoaCEPNacimento(Pessoa *raiz, int CEP, int *encontrado);
Pessoa *consultaPessoaCEPAtual(Pessoa *raiz, int CEP, int *encontrado);
int Quantas_pessoas_não_moram_na_cidade_natal(Pessoa *raiz, Estado *estados);
int numero_de_pessoas_nascidas_em_uma_determinada_cidade_nao_mora_na_cidade_natal(Pessoa *raiz, infoCidade *cidade);
int numero_de_pessoas_que_moram_em_uma_determinada_cidade_nao_nasceram_nela(Pessoa *raiz, infoCidade *cidade);


#endif