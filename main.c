#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "functions.h"

FILE *arquivo, *arquivo_qtd_alunos;


void menu(void);

int main(){

    

    arquivo = fopen("alunos.txt", "ab");
    arquivo_qtd_alunos = fopen("qtd_alunos.txt", "ab");

    if (arquivo == NULL){
        printf("Erro ao abrir o arquivo!\n");
        exit(1);
    }

    int opcao;
    int qtd_alunos = 0;
    
    AlocarMemoria();

    for (;;){
        
        printf("\n");
        menu();

        scanf("%d", &opcao);
        fflush(stdin);

        if (qtd_alunos >= 5){
            alunos = realloc(alunos, sizeof(aluno) * (qtd_alunos + 1));
        }

        if (opcao == 1){

            fopen("alunos.txt", "a");

            alunos[qtd_alunos] = Cadastrar();

            while(VerificacaoNotas(qtd_alunos) == 0){
                alunos[qtd_alunos]= Cadastrar();
            }

            fclose(arquivo);

            AdicionarQuantidadeAlunosArquivo(qtd_alunos);
        }

         if (opcao == 2){

            int erro = ExcluirAluno(qtd_alunos);

            if (erro == 0){
                printf("Aluno excluido com sucesso!\n");
                printf("\n");
            }

            fclose(arquivo);
        }
        
        if (opcao == 3){

            int erro = BuscarAluno(qtd_alunos);

            if (erro == 1){
                printf("Aluno nao encontrado!\n");
            }
        }

        if (opcao == 4){
            //abrir arquivo em modo leitura e imprimir na tela
            int erro = ImprimirRelatorioArquivo();

            if (erro != 0){
                printf("Nao ha alunos cadastrados!\n");
            }
        }

        if (opcao == 5){
            liberarMemoria();
            printf("Saindo...\n");
            break;
        }

        if (opcao < 1 || opcao > 5){
            printf("Opcao invalida!\n");
        }
    }
    
    return 0;
}

void menu(){
    printf("1 - Incluir aluno\n2 - Excluir Aluno\n3 - Buscar Aluno\n4 - Relatorio de Notas\n5 - Sair\n");
    printf("\nDigite a opcao desejada: ");

    return;
}

//split 
//strtok
//remover linha do vetor 