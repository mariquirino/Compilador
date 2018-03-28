#include "header.h"
#include "parser.h"

int main(int argc, char *argv[]) {
    if (argc < 2) { //Argc é o tamanho do vetor de argv
        printf("Falta nome do arquivo. \n");
        exit(-1);
    }
    FILE *arq = fopen(argv[1], "rb"); //Pos em que o nome do arq ta no argv
//    FILE *arq = fopen("teste.txt", "rb");
    if (arq == NULL) {
        printf("Nome: %s invalido. \n", argv[1]);
        exit(-1);
    }
    parser(arq);
    fclose(arq);
    return 0;
}

void printa_erro(Ttoken token, TErro erro, char nome[]) {
    printf("%s na linha: %d, coluna: %d, ultimo token lido: %s \n", nome, erro.linha, erro.coluna, token.lexema);
    exit(-1);
}

/*bool decl_var(FILE *arq, Ttoken *token, TErro *erro) {
    int i;
    int sequencia[9] = {TIPO_PR_INT, TIPO__PR_FLOAT, TIPO_PR_CHAR, TIPO_ID, TIPO_RECEBE, TIPO_INT, TIPO_FLOAT,
                        TIPO_CHAR, TIPO_PV};
    char nome[9][33] = {"ERRO SEM INT", "ERRO SEM FLOAT", "ERRO SEM CHAR", "ERRO SEM ID", "ERRO SEM =",
                        "ERRO SEM DIGITO OU FLOAT OU CHAR", "ERRO SEM ;"};
    for (i = 0; i < 3; ++i) {
        if ((*token).classificacao == sequencia[i]) {
            scanner(arq, token, erro);
            for (i = 3; i < 5; ++i) {
                if ((*token).classificacao == sequencia[i]) {
                    scanner(arq, token, erro);
                } else {
                    strcpy((*erro).nome, nome[i]); //Sem ID ou =
                    printf("%s na linha: %d, coluna: %d, ultimo token lido: %s \n", (*erro).nome, (*erro).linha,
                           (*erro).coluna,
                           (*token).lexema);
                    exit(-1);
                    return false;
                }
            }
            for (i = 5; i < 8; ++i) {
                if ((*token).classificacao == sequencia[i]) {
                    scanner(arq, token, erro);
                    if ((*token).classificacao == sequencia[8]) {
                        return true;
                    } else {
                        strcpy((*erro).nome, nome[6]);
                        printf("%s na linha: %d, coluna: %d, ultimo token lido: %s \n", (*erro).nome, (*erro).linha,
                               (*erro).coluna,
                               (*token).lexema);
                        exit(-1);
                        return false;
                    }
                }
            }
            strcpy((*erro).nome, nome[5]);
            printf("%s na linha: %d, coluna: %d, ultimo token lido: %s \n", (*erro).nome, (*erro).linha, (*erro).coluna,
                   (*token).lexema);
            exit(-1);
            return false;
        }
    }
    return false;
}
*/