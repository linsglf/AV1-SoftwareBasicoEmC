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


        int quantidade_alunos;
        arquivo_qtd_alunos = fopen("qtd_alunos.txt", "r");
        fscanf(arquivo_qtd_alunos, "%d", &quantidade_alunos);
        fclose(arquivo_qtd_alunos);


        if (quantidade_alunos >= 5){
            alunos = realloc(alunos, sizeof(aluno) * (quantidade_alunos + 1));
        }

        if (opcao == 1){

            fopen("alunos.txt", "a");

            alunos[qtd_alunos] = Cadastrar(qtd_alunos);

            if (strcmp(alunos[qtd_alunos].matricula, "0") == 0){
                printf("Cadastro cancelado!\n");
                system("pause");
                system("cls");
                continue;
            }

            while(VerificacaoNotas(qtd_alunos) == 0){
                alunos[qtd_alunos]= Cadastrar(qtd_alunos);
            }

            system("pause");
            system("cls");
        }

         if (opcao == 2){

            int erro = ExcluirAluno();

            if (erro == 0){
                printf("Aluno excluido com sucesso!\n");
                printf("\n");
            }

            system("pause");
            system("cls");
        }
        
        if (opcao == 3){

            int erro = BuscarAluno();

            if (erro == 1){
                printf("Aluno nao encontrado!\n");
            }
            
            system("pause");
            system("cls");
        }

        if (opcao == 4){
            //abrir arquivo em modo leitura e imprimir na tela
            int erro = ImprimirRelatorioArquivo();

            if (erro != 0){
                printf("Nao ha alunos cadastrados!\n");
            }

            system("pause");
            system("cls");
        }

        if (opcao == 5){

            int erro = AlterarAluno();

            if (erro != 0){
                printf("Aluno nao encontrado!\n");
            }

            

            system("pause");
            system("cls");   

        }

        if (opcao == 6){
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
    printf("1 - Incluir aluno\n2 - Excluir Aluno\n3 - Buscar Aluno\n4 - Relatorio de Notas\n5 - Alterar aluno\n6 - Sair\n");
    printf("\nDigite a opcao desejada: ");

    return;
}

//split 
//strtok
//remover linha do vetor 