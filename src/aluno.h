#ifndef ALUNO_H
#define ALUNO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#define MAX_NOME 50
#define MAX_FILHOS 100
#define ARQUIVO_ALUNOS "alunos.txt"


typedef struct Aluno {
    char nome[MAX_NOME];
    struct Aluno* filhos[MAX_FILHOS];
    int num_filhos;
    float desconto;
    struct Aluno* pai;
} Aluno;


typedef struct {
    Aluno* raiz;
    float mensalidade;
} ArvoreAlunos;


ArvoreAlunos* criarArvore(float mensalidade);
Aluno* criarAluno(char* nomeAluno);
Aluno* buscarAluno(Aluno* raiz, char* nomeAluno);
int matricularAluno(ArvoreAlunos* arvore, char* nomeAluno, char* nomeIndicador);


void imprimirArvoreCompleta(Aluno* raiz, int nivel);
void imprimirSubArvore(Aluno* aluno, int nivel, float mensalidade);
void processarVisualizacaoCompleta(ArvoreAlunos* arvore);
void mostrarValorDesconto(ArvoreAlunos* arvore);

void exibirMenu();
void processarMatricula(ArvoreAlunos* arvore);
void processarVisualizacao(ArvoreAlunos* arvore);
void processarPagamento(ArvoreAlunos* arvore);


void toLowerCase(char* str);
void limparBuffer();
int lerInteiro();
int lerString(char* buffer, int tamanho);
float lerValorMensalidade();


void salvarAlunos(ArvoreAlunos* arvore);
void carregarAlunos(ArvoreAlunos* arvore);

#endif
