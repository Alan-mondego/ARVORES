#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "aluno.h"
#include <locale.h>

// Função para criar a árvore
ArvoreAlunos* criarArvore(float mensalidade) {
    ArvoreAlunos* arvore = (ArvoreAlunos*)malloc(sizeof(ArvoreAlunos));
    if (arvore != NULL) {
        arvore->raiz = NULL;
        arvore->mensalidade = mensalidade;
    }
    return arvore;
}

// Função para criar um novo aluno
Aluno* criarAluno(char* nome) {
    Aluno* novo = (Aluno*)malloc(sizeof(Aluno));
    if (novo != NULL) {
        strcpy(novo->nome, nome);
        novo->num_filhos = 0;
        novo->desconto = 0;
        novo->pai = NULL;
        memset(novo->filhos, 0, sizeof(novo->filhos));
    }
    return novo;
}

// Função para converter string para minúsculo
void toLowerCase(char *str) {
    for(int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

// Função para limpar o buffer do teclado
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Função para ler um número inteiro com validação
int lerInteiro() {
    char linha[32];
    if (fgets(linha, sizeof(linha), stdin)) {
        char* fim;
        long val = strtol(linha, &fim, 10);
        if (fim != linha && (*fim == '\n' || *fim == '\0')) {
            return (int)val;
        }
    }
    return -1;
}

// Função para ler uma string com validação
int lerString(char *buffer, int tamanho) {
    if (!fgets(buffer, tamanho, stdin)) {
        return 0;
    }

    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n') {
        buffer[len-1] = '\0';
        len--;
    }

    if (len == 0) {
        return 0;
    }

    // Verifica se contem apenas letras e espacos
    for (size_t i = 0; i < len; i++) {
        if (!isalpha(buffer[i]) && buffer[i] != ' ') {
            return 0;
        }
    }

    return 1;
}

// Função para buscar um aluno na árvore
Aluno* buscarAluno(Aluno* raiz, char* nome) {
    if (!raiz) return NULL;

    char nome_lower[MAX_NOME];
    char raiz_nome_lower[MAX_NOME];

    strcpy(nome_lower, nome);
    strcpy(raiz_nome_lower, raiz->nome);
    toLowerCase(nome_lower);
    toLowerCase(raiz_nome_lower);

    if (strcmp(raiz_nome_lower, nome_lower) == 0) {
        return raiz;
    }

    for (int i = 0; i < raiz->num_filhos; i++) {
        Aluno* encontrado = buscarAluno(raiz->filhos[i], nome);
        if (encontrado) return encontrado;
    }

    return NULL;
}

// Função para calcular desconto de um nível específico
float calcularDescontoNivel(int nivel) {
    switch(nivel) {
        case 0: return 0.05f;  // 5% para quem indica
        case 1: return 0.04f;  // 4% para quem foi indicado
        case 2: return 0.03f;  // 3% para o "avo"
        case 3: return 0.02f;  // 2% para o "bisavo"
        default: return 0.01f; // 1% para demais niveis
    }
}

// Função para propagar descontos na árvore
void propagarDescontos(Aluno* aluno, int nivel) {
    if (!aluno) return;

    if (nivel > 0) {
        aluno->desconto += calcularDescontoNivel(1);  // Desconto por ser indicado

        // Propaga descontos para os ancestrais
        Aluno* atual = aluno->pai;
        int nivel_ancestral = 0;

        while (atual != NULL) {
            atual->desconto += calcularDescontoNivel(nivel_ancestral);
            atual = atual->pai;
            nivel_ancestral++;
        }
    }
}

// Função para matricular um novo aluno
int matricularAluno(ArvoreAlunos* arvore, char* nome, char* indicador) {
    if (!arvore) return 0;

    // Verifica se ja existe aluno com esse nome
    if (buscarAluno(arvore->raiz, nome) != NULL) {
        printf("Erro: Aluno ja cadastrado!\n");
        return 0;
    }

    Aluno* novo = criarAluno(nome);
    if (!novo) return 0;

    if (!indicador || strlen(indicador) == 0) {
        // Primeiro aluno da arvore
        if (!arvore->raiz) {
            arvore->raiz = novo;
            return 1;
        }
        // Adiciona como filho da raiz
        if (arvore->raiz->num_filhos < 100) {
            arvore->raiz->filhos[arvore->raiz->num_filhos++] = novo;
            novo->pai = arvore->raiz;
            propagarDescontos(novo, 1);
            return 1;
        }
        return 0;
    }

    // Busca o indicador
    Aluno* pai = buscarAluno(arvore->raiz, indicador);
    if (!pai) {
        free(novo);
        return 0;
    }

    // Adiciona como filho do indicador
    if (pai->num_filhos < 100) {
        pai->filhos[pai->num_filhos++] = novo;
        novo->pai = pai;
        propagarDescontos(novo, 1);
        return 1;
    }

    free(novo);
    return 0;
}

// Função para ler o valor da mensalidade
float lerValorMensalidade() {
    float valor;
    do {
        printf("Digite o valor da mensalidade da academia: R$ ");
        if (scanf("%f", &valor) == 1 && valor > 0) {
            limparBuffer();
            return valor;
        }
        printf("Erro: Digite um valor válido maior que zero!\n");
        limparBuffer();
    } while (1);
}

void imprimirSubArvore(Aluno* aluno, int nivel, float mensalidade) {
    if (!aluno) return;

    for (int i = 0; i < nivel; i++) {
        printf("  ");
    }

    printf("%s (Desconto: %.1f%%)\n", aluno->nome, aluno->desconto * 100);

    for (int i = 0; i < aluno->num_filhos; i++) {
        imprimirSubArvore(aluno->filhos[i], nivel + 1, mensalidade);
    }
}

void exibirMenu() {
    setlocale(LC_ALL,"Portuguese");
    printf("\n=== BEM VINDO A ACADEMIA ===\n");
    printf("1. Matricular novo aluno\n");
    printf("2. Visualizar sub-arvore de um aluno\n");
    printf("3. Consultar valores dos descontos\n");
    printf("0. Sair\n");
    printf("Escolha uma opcao: ");
}

void processarMatricula(ArvoreAlunos* arvore) {
    char nome[MAX_NOME];
    char indicador[MAX_NOME];

    printf("Digite o nome do aluno: ");
    if (!lerString(nome, MAX_NOME)) {
        printf("Erro: Nome invalido!\n");
        return;
    }

    if (!arvore->raiz) {
        if (matricularAluno(arvore, nome, NULL)) {
            printf("Primeiro aluno matriculado com sucesso!\n");
        } else {
            printf("Erro ao matricular aluno!\n");
        }
        return;
    }

    printf("Digite o nome do indicador: ");
    if (!lerString(indicador, MAX_NOME)) {
        printf("Erro: Nome do indicador invalido!\n");
        return;
    }

    if (matricularAluno(arvore, nome, indicador)) {
        printf("Aluno matriculado com sucesso!\n");
    } else {
        printf("Erro ao matricular aluno!\n");
    }
}

// Função para processar a visualização da sub-árvore
void processarVisualizacao(ArvoreAlunos* arvore) {
    char nome[MAX_NOME];

    printf("Digite o nome do aluno: ");
    if (!lerString(nome, MAX_NOME)) {
        printf("Erro: Nome invalido!\n");
        return;
    }

    Aluno* aluno = buscarAluno(arvore->raiz, nome);
    if (aluno) {
        printf("\nArvore de indicacoes de %s:\n", nome);
        imprimirSubArvore(aluno, 0, arvore->mensalidade);
    } else {
        printf("Aluno nao encontrado!\n");
    }
}

void mostrarValorDesconto(ArvoreAlunos* arvore) {
    char nome[MAX_NOME];

    printf("Digite o nome do aluno: ");
    if (!lerString(nome, MAX_NOME)) {
        printf("Erro: Nome invalido!\n");
        return;
    }

    Aluno* aluno = buscarAluno(arvore->raiz, nome);
    if (aluno) {
        printf("\nCalculo do desconto para %s:\n", nome);
        printf("R$ %.2f - %.1f%% = R$ %.2f\n", 
            arvore->mensalidade, 
            aluno->desconto * 100,
            arvore->mensalidade * (1 - aluno->desconto));
    } else {
        printf("Aluno nao encontrado!\n");
    }
}
