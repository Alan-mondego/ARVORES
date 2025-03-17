#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "aluno.h"
#include <locale.h>


ArvoreAlunos* criarArvore(float mensalidade) {
    ArvoreAlunos* arvore = (ArvoreAlunos*)malloc(sizeof(ArvoreAlunos));
    if (arvore != NULL) {
        arvore->raiz = NULL;
        arvore->mensalidade = mensalidade;
    }
    return arvore;
}

Aluno* criarAluno(char* nomeAluno) {
    Aluno* novo = (Aluno*)malloc(sizeof(Aluno));
    if (novo != NULL) {
        strncpy(novo->nome, nomeAluno, MAX_NOME - 1);
        novo->nome[MAX_NOME - 1] = '\0';
        novo->num_filhos = 0;
        novo->desconto = 0;
        novo->pai = NULL;
        for (int i = 0; i < MAX_FILHOS; i++) {
            novo->filhos[i] = NULL;
        }
    }
    return novo;
}

void toLowerCase(char* str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}


void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


int lerInteiro() {
    int num;
    while (scanf("%d", &num) != 1) {
        printf("Erro: Digite um numero valido!\n");
        limparBuffer();
    }
    limparBuffer();
    return num;
}

int lerString(char* buffer, int tamanho) {
    if (fgets(buffer, tamanho, stdin) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0';
        }
        return 1;
    }
    return 0;
}

Aluno* buscarAluno(Aluno* raiz, char* nomeAluno) {
    if (raiz == NULL) return NULL;

    char nomeRaiz[MAX_NOME];
    char nomeBusca[MAX_NOME];

    strncpy(nomeRaiz, raiz->nome, MAX_NOME - 1);
    strncpy(nomeBusca, nomeAluno, MAX_NOME - 1);

    toLowerCase(nomeRaiz);
    toLowerCase(nomeBusca);

    if (strcmp(nomeRaiz, nomeBusca) == 0) return raiz;

    for (int i = 0; i < raiz->num_filhos; i++) {
        Aluno* encontrado = buscarAluno(raiz->filhos[i], nomeAluno);
        if (encontrado != NULL) return encontrado;
    }

    return NULL;
}

float calcularDescontoNivel(int nivel) {
    switch(nivel) {
        case 0: return 5.0f;
        case 1: return 4.0f;
        case 3: return 2.0f;
        case 4: return 1.0f;
        default: return 0.0f;
    }
}


Aluno* encontrarRaiz(Aluno* aluno) {
    if (!aluno)
        return NULL;
    while (aluno->pai != NULL) {
        aluno = aluno->pai;
    }
    return aluno;
}

int calcularNivel(Aluno* ancestral, Aluno* descendente) {
    if (!descendente || !ancestral) return -1;

    int nivel = 0;
    Aluno* atual = descendente;

    while (atual && atual != ancestral) {
        nivel++;
        atual = atual->pai;
    }

    return atual == ancestral ? nivel : -1;
}

void verificarNoParaDesconto(Aluno* no, Aluno* atual, Aluno* raiz) {
    if (!atual) return;


    Aluno* temp = atual;
    int nivel = 0;


    while (temp && temp != no && nivel < 5) {
        temp = temp->pai;
        nivel++;
    }

    if (temp == no && nivel < 5) {
        float desconto = calcularDescontoNivel(nivel);
        if (desconto > atual->desconto) {
            atual->desconto = desconto;
        }
    }

    for (int i = 0; i < atual->num_filhos; i++) {
        verificarNoParaDesconto(no, atual->filhos[i], raiz);
    }
}

void atualizarDescontoNo(Aluno* no, Aluno* raiz) {
    if (!no || !raiz) return;

    no->desconto = 0;


    verificarNoParaDesconto(no, raiz, raiz);
}

void atualizarTodosDescontos(Aluno* no, Aluno* raiz) {
    if (!no) return;

    atualizarDescontoNo(no, raiz);

    for (int i = 0; i < no->num_filhos; i++) {
        atualizarTodosDescontos(no->filhos[i], raiz);
    }
}

void propagarDescontos(Aluno* aluno) {
    if (!aluno || !aluno->pai) return;


    Aluno* raiz = encontrarRaiz(aluno);
    if (!raiz) return;


    atualizarTodosDescontos(raiz, raiz);
}

void atualizarDescontosAcima(Aluno* aluno) {
    if (!aluno) return;

    // Primeiro, define o desconto do aluno que fez a indicação direta
    aluno->desconto = 5.0f;
    
    // Agora sobe na árvore atualizando os descontos
    Aluno* atual = aluno->pai;
    float descontos[] = {4.0f, 3.0f, 2.0f, 1.0f}; // Descontos para cada nível acima
    int nivel = 0;  // Índice para o array de descontos
    
    while (atual != NULL && nivel < 4) {
        atual->desconto += descontos[nivel];
        atual = atual->pai;
        nivel++;
    }
}


int matricularAluno(ArvoreAlunos* arvore, char* nomeAluno, char* nomeIndicador) {
    if (buscarAluno(arvore->raiz, nomeAluno) != NULL) {
        return 0;
    }

    Aluno* novo = criarAluno(nomeAluno);
    if (novo == NULL) return 0;

    if (nomeIndicador[0] == '\0') {
        if (arvore->raiz == NULL) {
            arvore->raiz = novo;
        } else {

            if (arvore->raiz->num_filhos < MAX_FILHOS) {
                arvore->raiz->filhos[arvore->raiz->num_filhos++] = novo;
                novo->pai = arvore->raiz;

                atualizarDescontosAcima(arvore->raiz);
            }
        }
    } else {
        Aluno* indicador = buscarAluno(arvore->raiz, nomeIndicador);
        if (indicador == NULL) return 0;

        if (indicador->num_filhos < MAX_FILHOS) {
            indicador->filhos[indicador->num_filhos++] = novo;
            novo->pai = indicador;


            atualizarDescontosAcima(indicador);
        }
    }
    return 1;
}

float lerValorMensalidade() {
    float valor;
    printf("Digite o valor da mensalidade da academia: R$ ");
    while (scanf("%f", &valor) != 1 || valor <= 0) {
        printf("Erro: Digite um valor valido para a mensalidade!\n");
        printf("Digite o valor da mensalidade da academia: R$ ");
        limparBuffer();
    }
    limparBuffer();
    return valor;
}

void imprimirSubArvore(Aluno* aluno, int nivel, float mensalidade) {
    if (aluno == NULL) return;

    for (int i = 0; i < nivel; i++) printf("  ");
    float valorComDesconto = mensalidade * (1 - aluno->desconto/100);
    printf("- %s (Desconto: %.1f%% - Valor da mensalidade: R$ %.2f)\n",
           aluno->nome, aluno->desconto, valorComDesconto);

    for (int i = 0; i < aluno->num_filhos; i++) {
        imprimirSubArvore(aluno->filhos[i], nivel + 1, mensalidade);
    }
}

void exibirMenu() {
    setlocale(LC_ALL,"Portuguese");
    printf("\n=== BEM VINDO A ACADEMIA ===\n");
    printf("1. Incluir novo cliente\n");
    printf("2. Visualizar todos os clientes\n");
    printf("3. Visualizar arvore de um cliente\n");
    printf("4. Processar pagamento\n");
    printf("0. Sair\n");
    printf("Escolha uma opcao: ");
}

void processarMatricula(ArvoreAlunos* arvore) {
    setlocale(LC_ALL,"Portuguese");
    char nomeAluno[MAX_NOME], nomeIndicador[MAX_NOME];

    printf("Digite o nome do novo aluno: ");
    lerString(nomeAluno, MAX_NOME);

    printf("Digite o nome do indicador (ou Enter para nenhum): ");
    lerString(nomeIndicador, MAX_NOME);

    if (matricularAluno(arvore, nomeAluno, nomeIndicador)) {
        printf("Aluno matriculado com sucesso!\n");
    } else {
        printf("Erro ao matricular aluno.\n");
    }
}


void processarVisualizacao(ArvoreAlunos* arvore) {
    char nomeAluno[MAX_NOME];

    printf("Digite o nome do aluno: ");
    if (!lerString(nomeAluno, MAX_NOME)) {
        printf("Erro: Nome invalido!\n");
        return;
    }

    Aluno* aluno = buscarAluno(arvore->raiz, nomeAluno);
    if (aluno) {
        printf("\nArvore de indicacoes de %s:\n", nomeAluno);
        imprimirSubArvore(aluno, 0, arvore->mensalidade);
    } else {
        printf("Aluno nao encontrado!\n");
    }
}

void mostrarValorDesconto(ArvoreAlunos* arvore) {
    char nomeAluno[MAX_NOME];
    printf("Digite o nome do aluno: ");
    lerString(nomeAluno, MAX_NOME);

    Aluno* aluno = buscarAluno(arvore->raiz, nomeAluno);
    if (aluno == NULL) {
        printf("Aluno nao encontrado.\n");
        return;
    }

    printf("\nInformacoes do aluno %s:\n", aluno->nome);
    float valorComDesconto = arvore->mensalidade * (1 - aluno->desconto/100);
    printf("Mensalidade original: R$ %.2f\n", arvore->mensalidade);
    printf("Desconto: %.1f%%\n", aluno->desconto);
    printf("Valor com desconto: R$ %.2f\n", valorComDesconto);
}

void imprimirArvoreCompleta(Aluno* raiz, int nivel) {
    if (raiz == NULL) return;

    for (int i = 0; i < nivel; i++) printf("  ");
    printf("- %s (Desconto: %.1f%%)\n", raiz->nome, raiz->desconto);

    for (int i = 0; i < raiz->num_filhos; i++) {
        imprimirArvoreCompleta(raiz->filhos[i], nivel + 1);
    }
}

void processarVisualizacaoCompleta(ArvoreAlunos* arvore) {
    if (arvore->raiz == NULL) {
        printf("Nenhum aluno cadastrado.\n");
        return;
    }
    printf("\nArvore de alunos:\n");
    imprimirArvoreCompleta(arvore->raiz, 0);
}

void salvarAlunoRecursivo(FILE* arquivo, Aluno* aluno, int nivel) {
    if (aluno == NULL) return;

    fprintf(arquivo, "%d;%s;%.2f\n", nivel, aluno->nome, aluno->desconto);

    for (int i = 0; i < aluno->num_filhos; i++) {
        salvarAlunoRecursivo(arquivo, aluno->filhos[i], nivel + 1);
    }
}

void salvarAlunos(ArvoreAlunos* arvore) {
    FILE* arquivo = fopen(ARQUIVO_ALUNOS, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo para salvar.\n");
        return;
    }

    fprintf(arquivo, "%.2f\n", arvore->mensalidade);
    salvarAlunoRecursivo(arquivo, arvore->raiz, 0);

    fclose(arquivo);
}

Aluno* carregarAlunoRecursivo(FILE* arquivo, int nivel, Aluno* pai) {
    char linha[256];
    if (!fgets(linha, sizeof(linha), arquivo)) {
        return NULL;
    }

    linha[strcspn(linha, "\n")] = 0;


    char* nivel_str = strtok(linha, ";");
    char* nome = strtok(NULL, ";");
    char* desconto_str = strtok(NULL, ";");

    if (!nivel_str || !nome || !desconto_str) {
        return NULL;
    }


    Aluno* aluno = criarAluno(nome);
    if (!aluno) {
        return NULL;
    }


    aluno->desconto = atof(desconto_str);
    aluno->pai = pai;


    int nivel_atual = atoi(nivel_str);
    while (1) {
        int proximo_nivel;
        if (fscanf(arquivo, "%d;", &proximo_nivel) != 1) {
            break;
        }
        fseek(arquivo, -2, SEEK_CUR);

        if (proximo_nivel <= nivel_atual) {
            break;
        }

        Aluno* filho = carregarAlunoRecursivo(arquivo, proximo_nivel, aluno);
        if (filho) {
            aluno->filhos[aluno->num_filhos++] = filho;
        }
    }

    return aluno;
}

void carregarAlunos(ArvoreAlunos* arvore) {
    FILE* arquivo = fopen(ARQUIVO_ALUNOS, "r");
    if (arquivo == NULL) return;

    fscanf(arquivo, "%f\n", &arvore->mensalidade);

    char linha[256];
    char nome[MAX_NOME];
    int nivel;
    float desconto;

    while (fgets(linha, sizeof(linha), arquivo)) {
        sscanf(linha, "%d;%[^;];%f\n", &nivel, nome, &desconto);

        if (nivel == 0) {
            arvore->raiz = criarAluno(nome);
            arvore->raiz->desconto = desconto;
        } else {

            if (arvore->raiz != NULL && arvore->raiz->num_filhos < MAX_FILHOS) {
                Aluno* novo = criarAluno(nome);
                novo->desconto = desconto;
                novo->pai = arvore->raiz;
                arvore->raiz->filhos[arvore->raiz->num_filhos++] = novo;
            }
        }
    }

    fclose(arquivo);
}

void processarPagamento(ArvoreAlunos* arvore) {
    char nomeAluno[MAX_NOME];
    printf("Digite o nome do cliente: ");
    lerString(nomeAluno, MAX_NOME);
    
    Aluno* aluno = buscarAluno(arvore->raiz, nomeAluno);
    if (aluno == NULL) {
        printf("Cliente nao encontrado!\n");
        return;
    }
    
    float valorOriginal = arvore->mensalidade;
    float desconto = aluno->desconto;
    float valorComDesconto = valorOriginal * (1 - desconto/100);
    
    printf("\n=== Processamento de Pagamento ===\n");
    printf("Cliente: %s\n", aluno->nome);
    printf("Valor da mensalidade: R$ %.2f\n", valorOriginal);
    printf("Desconto acumulado: %.1f%%\n", desconto);
    printf("Valor a pagar: R$ %.2f\n", valorComDesconto);
    printf("Valor economizado: R$ %.2f\n", valorOriginal - valorComDesconto);
    
    printf("\nPressione ENTER para continuar...");
    getchar();
}
