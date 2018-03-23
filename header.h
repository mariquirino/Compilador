//
// Created by Mariana on 03/03/2018.
//

#ifndef COMPILADOR_HEADER_H
#define COMPILADOR_HEADER_H

typedef struct token {
    char lexema[30];
    int classificacao;

} Ttoken;

typedef struct erro {
    int linha, coluna;
    char nome[100];
} TErro;


#define TRUE 1
#define FALSE 0

//Scanner
bool scanner(FILE *arq, Ttoken *t, TErro *erro);

void insere(Ttoken *token, int *qtd, char *c, FILE *arq, TErro *erro);

void eof(Ttoken *token);

int comentario(FILE *arq, char *c, int *qtd, Ttoken *token, TErro *erro);

void ID_PR(FILE *arq, char *c, int *qtd, Ttoken *token, TErro *erro);

void verArit(FILE *arq, char *c, int *qtd, Ttoken *token, TErro *erro);

bool verRelacional(FILE *arq, char *c, int *qtd, Ttoken *token, TErro *erro);

void verEspecial(FILE *arq, char *c, int *qtd, Ttoken *token, TErro *erro);

bool verInt_Float(FILE *arq, char *c, int *qtd, Ttoken *token, TErro *erro);

int verPR(char pr[]);

bool verChar(FILE *arq, char *c, int *qtd, Ttoken *token, TErro *erro);
//

//Parser
//

#endif //COMPILADOR_HEADER_H
