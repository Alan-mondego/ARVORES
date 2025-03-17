#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "aluno.h"
#include <locale.h>

int main() {
    setlocale(LC_ALL,"Portuguese");
    printf("=== BEM VINDO ===\n");

    // Cria a árvore com mensalidade inicial
    ArvoreAlunos* arvore = criarArvore(0);
    if (!arvore) {
        printf("Erro ao criar a arvore!\n");
        return 1;
    }

    // Tenta carregar alunos existentes
    carregarAlunos(arvore);

    // Se não houver alunos carregados, pede a mensalidade
    if (!arvore->raiz) {
        arvore->mensalidade = lerValorMensalidade();
    }

    int opcao;
    do {
        exibirMenu();
        opcao = lerInteiro();

        switch (opcao) {
            case 1:
                processarMatricula(arvore);
                break;
            case 2:
                processarVisualizacaoCompleta(arvore);
                break;
            case 3:
                processarVisualizacao(arvore);
                break;
            case 4:
                processarPagamento(arvore);
                break;
            case 0:
                printf("Salvando dados...\n");
                salvarAlunos(arvore);
                printf("Saindo do programa...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 0);

    free(arvore);
    return 0;
}
