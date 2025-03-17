#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "aluno.h"
#include <locale.h>

int main() {
    setlocale(LC_ALL,"Portuguese");
    printf("=== CONFIGURACAO INICIAL DA ACADEMIA ===\n");
    float mensalidade = lerValorMensalidade();
    
    ArvoreAlunos* arvore = criarArvore(mensalidade);
    if (!arvore) {
        printf("Erro ao criar a arvore!\n");
        return 1;
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
                processarVisualizacao(arvore);
                break;
            case 3:
                mostrarValorDesconto(arvore);
                break;
            case 0:
                printf("Saindo do programa...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 0);

    free(arvore);
    return 0;
}
