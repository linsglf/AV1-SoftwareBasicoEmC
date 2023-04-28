#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    char nome[30];
    char matricula[10];
    char cpf[11];
    int dia;
    int mes;
    int ano;
    int idade;
    float nota1;
    float nota2;
    float media;
    char situacao[12];
} aluno;

aluno *alunos;

FILE *arquivo;

FILE *arquivo_qtd_alunos;

void AlocarMemoria(){

    alunos = malloc (sizeof(aluno) * 5);
}

int validar_cpf(char* cpf) {
    int i, soma, resto;
    char cpf_numeros[10];
    int pesos1[9] = {10, 9, 8, 7, 6, 5, 4, 3, 2};
    int pesos2[10] = {11, 10, 9, 8, 7, 6, 5, 4, 3, 2};

    // Verificar se o CPF é nulo
    if (cpf == NULL) {
        printf("CPF invalido. O CPF nao pode ser nulo.\n");
        return 0;
    }

    // Verificar se os numeros do CPF são iguais
    for (i = 0; i < 10; i++) {
        if (cpf[i] == cpf[i + 1] && cpf[i] == cpf[i + 2]){
            printf("CPF invalido. O CPF nao pode conter todos os numeros iguais.\n");
            return 0;
        }
    }

    // Verificar se o CPF tem 11 dígitos
    if (strlen(cpf) != 11) {
        printf("CPF invalido. O CPF deve ter 11 digitos.\n");
        return 0;
    }

    // Verificar se todos os caracteres do CPF são números
    for (i = 0; i < 11; i++) {
        if (cpf[i] < '0' || cpf[i] > '9') {
            printf("CPF invalido. O CPF deve conter apenas numeros.\n");
            return 0;
        }
    }

    // Copiar os nove primeiros dígitos do CPF para uma variável auxiliar
    for (i = 0; i < 9; i++) {
        cpf_numeros[i] = cpf[i];
    }
    cpf_numeros[9] = '\0';

    // Verificar o primeiro dígito verificador
    soma = 0;
    for (i = 0; i < 9; i++) {
        soma += (cpf_numeros[i] - '0') * pesos1[i];
    }
    resto = soma % 11;
    if (resto == 0 || resto == 1) {
        if (cpf[9] != '0') {
            printf("CPF invalido.\n");
            return 0;
        }
    } else {
        if (cpf[9] != '0' + (11 - resto)) {
            printf("CPF invalido.\n");
            return 0;
        }
    }

    // Copiar os dez primeiros dígitos do CPF para a variável auxiliar
    for (i = 0; i < 10; i++) {
        cpf_numeros[i] = cpf[i];
    }
    cpf_numeros[10] = '\0';

    // Verificar o segundo dígito verificador
    soma = 0;
    for (i = 0; i < 10; i++) {
        soma += (cpf_numeros[i] - '0') * pesos2[i];
    }
    resto = soma % 11;
    if (resto == 0 || resto == 1) {
        if (cpf[10] != '0') {
            printf("CPF invalido.\n");
            return 0;
        }
    } else {
        if (cpf[10] != '0' + (11 - resto)) {
            printf("CPF invalido.\n");
            return 0;
        }
    }

    // O CPF é válido
    return 1;
}

int CalcularIdade(int dia, int mes, int ano) {

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int ano_atual = tm.tm_year + 1900;
    int mes_atual = tm.tm_mon + 1;
    int dia_atual = tm.tm_mday;

    int idade = ano_atual - ano;

    if (mes_atual < mes) {
        idade--;
    } else if (mes_atual == mes && dia_atual < dia) {
        idade--;
    }

    return idade;
}


aluno Cadastrar(int qtd_alunos){
    aluno cadastro_aluno;

    printf("Digite o nome do aluno: ");
    gets(cadastro_aluno.nome);
    fflush(stdin);

    if (strcmp(cadastro_aluno.nome, "0") == 0){
        strcpy(cadastro_aluno.matricula, "0\0");
        return cadastro_aluno;
    }

    printf("Digite a matricula do aluno: ");
    gets(cadastro_aluno.matricula);
    fflush(stdin);

    printf("Digite a data de nascimento do aluno: ");
    scanf("%d/%d/%d", &cadastro_aluno.dia, &cadastro_aluno.mes, &cadastro_aluno.ano);
    fflush(stdin);

    printf("Digite o cpf do aluno: ");
    gets(cadastro_aluno.cpf);
    fflush(stdin);
    while (validar_cpf(cadastro_aluno.cpf) == 0){
        printf("Digite o cpf do aluno: ");
        gets(cadastro_aluno.cpf);
        fflush(stdin);
    }

    printf("Digite a nota 1 do aluno: ");
    scanf("%f", &cadastro_aluno.nota1);
    fflush(stdin);
    printf("Digite a nota 2 do aluno: ");
    scanf("%f", &cadastro_aluno.nota2);
    fflush(stdin);
    
    cadastro_aluno.media = (cadastro_aluno.nota1 + cadastro_aluno.nota2) / 2;

    if (cadastro_aluno.media >= 6){
         strcpy(cadastro_aluno.situacao, "Aprovado");
    }else if (cadastro_aluno.media < 6 && cadastro_aluno.media >= 4){
         strcpy(cadastro_aluno.situacao, "Recuperacao");
    }else{
         strcpy(cadastro_aluno.situacao, "Reprovado");
    }

    cadastro_aluno.idade = CalcularIdade(cadastro_aluno.dia, cadastro_aluno.mes, cadastro_aluno.ano);

    fprintf(arquivo, "| %s | %d | %s | %s | %d/%d/%d | %.2f | %.2f | %.2f | %s |\n", cadastro_aluno.nome, cadastro_aluno.idade, cadastro_aluno.matricula, cadastro_aluno.cpf, cadastro_aluno.dia, cadastro_aluno.mes, cadastro_aluno.ano, cadastro_aluno.nota1, cadastro_aluno.nota2, cadastro_aluno.media, cadastro_aluno.situacao);
    
    fclose(arquivo);

    AdicionarQuantidadeAlunosArquivo(qtd_alunos);

    return cadastro_aluno;
}

void AdicionarQuantidadeAlunosArquivo(int qtd_alunos){
    // Abre o arquivo e ve qual a quantidade de alunos
    if (arquivo_qtd_alunos == NULL){
        printf("Erro ao abrir o arquivo qtd_alunos.txt alunos é null");
        arquivo_qtd_alunos = fopen("qtd_alunos.txt", "w");
        fprintf(arquivo_qtd_alunos, "%d", qtd_alunos);
        fclose(arquivo_qtd_alunos);
    }else{
        arquivo_qtd_alunos = fopen("qtd_alunos.txt", "r");
        fscanf(arquivo_qtd_alunos, "%d", &qtd_alunos);
        fclose(arquivo_qtd_alunos);

        qtd_alunos++;

        arquivo_qtd_alunos = fopen("qtd_alunos.txt", "w");
        fprintf(arquivo_qtd_alunos, "%d", qtd_alunos);
        fclose(arquivo_qtd_alunos);
    }
}

void DecrementarQuantidadeAlunosArquivo(){
    int qtd_alunos;

    arquivo_qtd_alunos = fopen("qtd_alunos.txt", "r");
    fscanf(arquivo_qtd_alunos, "%d", &qtd_alunos);
    fclose(arquivo_qtd_alunos);

    qtd_alunos--;

    arquivo_qtd_alunos = fopen("qtd_alunos.txt", "w");
    fprintf(arquivo_qtd_alunos, "%d", qtd_alunos);
    fclose(arquivo_qtd_alunos);
}


int ExcluirAluno(){

    int qtd_alunos;
    arquivo_qtd_alunos = fopen("qtd_alunos.txt", "r");
    fscanf(arquivo_qtd_alunos, "%d", &qtd_alunos);
    fclose(arquivo_qtd_alunos);

    aluno arquivoVetor[qtd_alunos];

    //copiando as matriculas do arquivo para o vetor
    arquivo = fopen("alunos.txt", "r");

    for (int i = 0; i < qtd_alunos; i++){
        fscanf(arquivo, "| %s | %d | %s | %s | %d/%d/%d | %f | %f | %f | %s | ", arquivoVetor[i].nome, &arquivoVetor[i].idade, arquivoVetor[i].matricula, arquivoVetor[i].cpf, &arquivoVetor[i].dia, &arquivoVetor[i].mes, &arquivoVetor[i].ano, &arquivoVetor[i].nota1, &arquivoVetor[i].nota2, &arquivoVetor[i].media, arquivoVetor[i].situacao);
    }

    fclose(arquivo);

    int posicao_aluno = -1;
    char matricula[10];

    printf("Digite a matricula do aluno que deseja excluir: ");
    gets(matricula);
    fflush(stdin);

    //matricula = "0" volta para o menu

    if (strcmp(matricula, "0") == 0){
        printf("Voltando para o menu\n");
        return 1;
    }


    for (int i = 0; i < qtd_alunos; i++){
        if (strncmp (arquivoVetor[i].matricula, matricula, strlen(matricula)) == 0){
            posicao_aluno = i;
            break;
        }
    }

    if (posicao_aluno == -1) {
        printf("Aluno nao encontrado\n");
        return 1;
    }

    for (int i = posicao_aluno; i < qtd_alunos - 1; i++){
        arquivoVetor[i] = arquivoVetor[i + 1];
    }

    DecrementarQuantidadeAlunosArquivo();

    // Abre o arquivo e exclui a linha do aluno
    arquivo = fopen("alunos.txt", "w");

    for (int i = 0; i < qtd_alunos - 1; i++){
        fprintf(arquivo, "| %s | %d | %s | %s | %d/%d/%d | %.2f | %.2f | %.2f | %s |\n", arquivoVetor[i].nome, arquivoVetor[i].idade, arquivoVetor[i].matricula, arquivoVetor[i].cpf, arquivoVetor[i].dia, arquivoVetor[i].mes, arquivoVetor[i].ano, arquivoVetor[i].nota1, arquivoVetor[i].nota2, arquivoVetor[i].media, arquivoVetor[i].situacao);
    }

    fclose(arquivo);

    return 0;
}

int BuscarAluno(){

    int qtd_alunos;
    arquivo_qtd_alunos = fopen("qtd_alunos.txt", "r");
    fscanf(arquivo_qtd_alunos, "%d", &qtd_alunos);
    fclose(arquivo_qtd_alunos);

    aluno arquivoVetor[qtd_alunos];

    //copiando as matriculas do arquivo para o vetor

    arquivo = fopen("alunos.txt", "r");

    for (int i = 0; i < qtd_alunos; i++){
        fscanf(arquivo, "| %s | %d | %s | %s | %d/%d/%d | %f | %f | %f | %s | ", arquivoVetor[i].nome, &arquivoVetor[i].idade, arquivoVetor[i].matricula, arquivoVetor[i].cpf, &arquivoVetor[i].dia, &arquivoVetor[i].mes, &arquivoVetor[i].ano, &arquivoVetor[i].nota1, &arquivoVetor[i].nota2, &arquivoVetor[i].media, arquivoVetor[i].situacao);
    }

    fclose(arquivo);

    char matricula[10];

    printf("Digite a matricula do aluno que deseja buscar: ");
    gets(matricula);
    fflush(stdin);

    for (int i = 0; i < qtd_alunos; i++){

        if (strncmp (arquivoVetor[i].matricula, matricula, strlen(matricula)) == 0){
            printf("| %s | %d | %s | %s | %d/%d/%d | %.2f | %.2f | %.2f | %s |\n", arquivoVetor[i].nome, arquivoVetor[i].idade, arquivoVetor[i].matricula, arquivoVetor[i].cpf, arquivoVetor[i].dia, arquivoVetor[i].mes, arquivoVetor[i].ano, arquivoVetor[i].nota1, arquivoVetor[i].nota2, arquivoVetor[i].media, arquivoVetor[i].situacao);
            printf("Aluno encontrado!\n");

            return 0;
        }
    }

    return 1;
}

int VerificacaoNotas(int qtd_alunos){

    if(alunos[qtd_alunos].nota1 >= 0 && alunos[qtd_alunos].nota1 <= 10 && alunos[qtd_alunos].nota2 >= 0 && alunos[qtd_alunos].nota2 <= 10){
        printf("Aluno cadastrado com sucesso!\n");
        printf("| %s | %s | %.2f | %.2f |\n", alunos[qtd_alunos].nome, alunos[qtd_alunos].matricula ,alunos[qtd_alunos].nota1, alunos[qtd_alunos].nota2);

        return 1;
    }else if (alunos[qtd_alunos].nota1 > 10 || alunos[qtd_alunos].nota1 < 0){
        printf("Nota 1 invalida!\n");
        printf("Tente Novamente!\n");

        return 0;
    }else if (alunos[qtd_alunos].nota2 > 10 || alunos[qtd_alunos].nota2 < 0){
        printf("Nota 2 invalida!\n");
        printf("Tente Novamente!\n");

        return 0;
    }
}

int ImprimirRelatorioArquivo(){

    int qtd_alunos_arquivo = 0;
    FILE* arquivo_qtd_alunos = fopen("qtd_alunos.txt", "r");
    fscanf(arquivo_qtd_alunos, "%d", &qtd_alunos_arquivo); // Lê a quantidade de alunos do arquivo
    fclose(arquivo_qtd_alunos);

    if (qtd_alunos_arquivo == 0){
        return 1;
    }

    // Abre o arquivo
    FILE* arquivo = fopen("alunos.txt", "r");

    // Imprime o cabeçalho da tabela
    printf("|      NOME      | MATRICULA | IDADE | NOTA 1 | NOTA 2 | MEDIA |  SITUACAO  |\n");
    for (int i = 0; i < qtd_alunos_arquivo; i++) {
       // Lê os dados do arquivo
        fscanf(arquivo, "| %s | %d | %s | %s | %d/%d/%d | %f | %f | %f | %s |\n", alunos[i].nome, &alunos[i].idade, alunos[i].matricula, alunos[i].cpf, &alunos[i].dia, &alunos[i].mes, &alunos[i].ano, &alunos[i].nota1, &alunos[i].nota2, &alunos[i].media, alunos[i].situacao);

        // Imprime os dados na tela
        printf("| %s | %s | %d | %s | %02d/%02d/%04d | %.2f | %.2f | %.2f | %s |\n", alunos[i].nome, alunos[i].matricula, alunos[i].idade, alunos[i].cpf, alunos[i].dia, alunos[i].mes, alunos[i].ano, alunos[i].nota1, alunos[i].nota2, alunos[i].media, alunos[i].situacao);
    }

    // Fecha o arquivo
    fclose(arquivo);

    return 0;
}

int AlterarAluno(){

    int qtd_alunos;
    arquivo_qtd_alunos = fopen("qtd_alunos.txt", "r");
    fscanf(arquivo_qtd_alunos, "%d", &qtd_alunos);
    fclose(arquivo_qtd_alunos);

    aluno arquivoVetor[qtd_alunos];

    //copiando as matriculas do arquivo para o vetor

    arquivo = fopen("alunos.txt", "r");

    for (int i = 0; i < qtd_alunos; i++){
        fscanf(arquivo, "| %s | %d | %s | %s | %d/%d/%d | %f | %f | %f | %s | ", arquivoVetor[i].nome, &arquivoVetor[i].idade, arquivoVetor[i].matricula, arquivoVetor[i].cpf, &arquivoVetor[i].dia, &arquivoVetor[i].mes, &arquivoVetor[i].ano, &arquivoVetor[i].nota1, &arquivoVetor[i].nota2, &arquivoVetor[i].media, arquivoVetor[i].situacao);
    }

    fclose(arquivo);

    char matricula[10];

    printf("Digite a matricula do aluno que deseja alterar: ");
    gets(matricula);
    fflush(stdin);

    for (int i = 0; i < qtd_alunos; i++){

        if (strncmp (arquivoVetor[i].matricula, matricula, strlen(matricula)) == 0){
            printf("| %s | %d | %s | %s | %d/%d/%d | %.2f | %.2f | %.2f | %s |\n", arquivoVetor[i].nome, arquivoVetor[i].idade, arquivoVetor[i].matricula, arquivoVetor[i].cpf, arquivoVetor[i].dia, arquivoVetor[i].mes, arquivoVetor[i].ano, arquivoVetor[i].nota1, arquivoVetor[i].nota2, arquivoVetor[i].media, arquivoVetor[i].situacao);
            printf("Aluno encontrado!\n");

            printf("Digite o novo nome do aluno: ");
            gets(arquivoVetor[i].nome);
            fflush(stdin);
            printf("Digite a nova matricula do aluno: ");   
            gets(arquivoVetor[i].matricula);
            fflush(stdin);
            printf("Digite a nova data de nascimento do aluno: ");
            scanf("%d/%d/%d", &arquivoVetor[i].dia, &arquivoVetor[i].mes, &arquivoVetor[i].ano);
            fflush(stdin);

            printf("Digite o cpf do aluno: ");
            gets(arquivoVetor[i].cpf);
            fflush(stdin);
            while (validar_cpf(arquivoVetor[i].cpf) == 0){
                printf("Digite o cpf do aluno: ");
                gets(arquivoVetor[i].cpf);
                fflush(stdin);
            }

            printf("Digite a nova nota 1 do aluno: ");
            scanf("%f", &arquivoVetor[i].nota1);
            fflush(stdin);
            printf("Digite a nova nota 2 do aluno: ");
            scanf("%f", &arquivoVetor[i].nota2);
            fflush(stdin);
            
            while (arquivoVetor[i].nota1 < 0 || arquivoVetor[i].nota1 > 10 || arquivoVetor[i].nota2 < 0 || arquivoVetor[i].nota2 > 10)
            {
                printf("Nota invalida!\nTente novamente!\n");
                printf("Digite a nova nota 1 do aluno: ");
                scanf("%f", &arquivoVetor[i].nota1);
                fflush(stdin);
                printf("Digite a nova nota 2 do aluno: ");
                scanf("%f", &arquivoVetor[i].nota2);
                fflush(stdin);
            }

            arquivoVetor[i].media = (arquivoVetor[i].nota1 + arquivoVetor[i].nota2 ) / 2;

            if (arquivoVetor[i].media >= 6){
                strcpy(arquivoVetor[i].situacao, "Aprovado");
            }else if (arquivoVetor[i].media < 6 && arquivoVetor[i].media >= 4){
                strcpy(arquivoVetor[i].situacao, "Recuperacao");
            }else{
                strcpy(arquivoVetor[i].situacao, "Reprovado");
            };

            arquivoVetor[i].idade = CalcularIdade(arquivoVetor[i].dia, arquivoVetor[i].mes, arquivoVetor[i].ano);
            
            break;
        }else if (i == qtd_alunos - 1){
            return 1;
        }
    }

    arquivo = fopen("alunos.txt", "w");

    for (int i = 0; i < qtd_alunos; i++){
        fprintf(arquivo, "| %s | %d | %s | %s | %d/%d/%d | %.2f | %.2f | %.2f | %s |\n", arquivoVetor[i].nome, arquivoVetor[i].idade, arquivoVetor[i].matricula, arquivoVetor[i].cpf, arquivoVetor[i].dia, arquivoVetor[i].mes, arquivoVetor[i].ano, arquivoVetor[i].nota1, arquivoVetor[i].nota2, arquivoVetor[i].media, arquivoVetor[i].situacao);
    }

    fclose(arquivo);

    return 0;
}

void liberarMemoria(void){
    free(alunos);
}   