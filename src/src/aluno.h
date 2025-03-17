#ifndef ALUNO_H
#define ALUNO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<locale.h>
#define MAX_NOME 50

// Estrutura para representar um aluno
typedef struct Aluno {
    char nome[MAX_NOME];
    struct Aluno* filhos[100];
    int num_filhos;
    float desconto;
    struct Aluno* pai;
} Aluno;

// Estrutura para gerenciar a arvore
typedef struct {
    Aluno* raiz;
    float mensalidade;
} ArvoreAlunos;

// Funcoes de manipulacao de alunos
Aluno* criarAluno(char* nome);
Aluno* buscarAluno(Aluno* raiz, char* nome);
int matricularAluno(ArvoreAlunos* arvore, char* nome, char* indicador);
void imprimirSubArvore(Aluno* aluno, int nivel, float mensalidade);

// Funcoes do menu
void exibirMenu(void);
void processarMatricula(ArvoreAlunos* arvore);
void processarVisualizacao(ArvoreAlunos* arvore);
void mostrarValorDesconto(ArvoreAlunos* arvore);

// Funcoes auxiliares
void toLowerCase(char* str);
void limparBuffer(void);
int lerInteiro(void);
int lerString(char* buffer, int tamanho);
float lerValorMensalidade(void);

// Funcao para inicializar a arvore
ArvoreAlunos* criarArvore(float mensalidade);

#endif
