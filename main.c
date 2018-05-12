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
