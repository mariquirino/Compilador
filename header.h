//
// Created by Mariana on 03/03/2018.
//

#ifndef COMPILADOR_HEADER_H
#define COMPILADOR_HEADER_H

typedef struct token {
    char lexema[30];
    int classificacao;
    int linha, coluna;
} Ttoken;


#define  TIPO_INT 10
#define  TIPO_FLOAT 11
#define  TIPO_CHAR 12

#define  TIPO_ID 13

#define  TIPO_SOMA 20
#define  TIPO_SUB 21
#define  TIPO_MULT 22
#define  TIPO_DIV 23
#define  TIPO_RECEBE 24

#define  TIPO_IGUAL 30
#define  TIPO_MENOR 31
#define  TIPO_MAIOR 32
#define  TIPO_MENOR_IGUAL 33
#define  TIPO_MAIOR_IGUAL 34
#define  TIPO_DIFERENTE 35

#define  tipo_EOF 17

#define  TIPO_MAIN 40
#define  TIPO_IF 41
#define  TIPO_ELSE 42
#define  TIPO_WHILE 43
#define  TIPO_DO 44
#define  TIPO_FOR 45
#define  TIPO_PR_INT 46
#define  TIPO__PR_FLOAT 47
#define  TIPO_PR_CHAR 48

#define  TIPO_PV 50
#define  TIPO_ABRE_PARENTESE 51
#define  TIPO_FECHA_PARENTESE 52
#define  TIPO_ABRE_CHAVE 53
#define  TIPO_FECHA_CHAVE 54
#define  TIPO_VIRGULA 55

#define TRUE 1
#define FALSE 0


FILE *criarArquivo(char nome[]);

bool scanner(FILE *arq, Ttoken *t);

void insere(Ttoken *token, int *qtd, char *c, FILE *arq);

void eof(Ttoken *token);

int comentario(FILE *arq, char *c, int *qtd, Ttoken *token);

void ID_PR(FILE *arq, char *c, int *qtd, Ttoken *token);

void verArit(FILE *arq, char *c, int *qtd, Ttoken *token);

bool verRelacional(FILE *arq, char *c, int *qtd, Ttoken *token);

void verEspecial(FILE *arq, char *c, int *qtd, Ttoken *token);

bool verInt_Float(FILE *arq, char *c, int *qtd, Ttoken *token);

int verPR(char pr[]);

bool verChar(FILE *arq, char *c, int *qtd, Ttoken *token);

#endif //COMPILADOR_HEADER_H
