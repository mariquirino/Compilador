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

//Scanner
void scanner(FILE *arq, Ttoken *t, TErro *erro);

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
void parser(FILE *arq);
void programa(FILE *arq, Ttoken *token, TErro *erro);
bool bloco(FILE *arq, Ttoken *token, TErro *erro);
bool decl_var(FILE *arq, Ttoken *token, TErro *erro);
bool comando(FILE *arq, Ttoken *token, TErro *erro);
bool comando_basico(FILE *arq, Ttoken *token, TErro *erro);
bool iteracao(FILE *arq, Ttoken *token, TErro *erro);
bool atribuicao(FILE *arq, Ttoken *token, TErro *erro);
bool expr_relacional(FILE *arq, Ttoken *token, TErro *erro);
bool expr_arit(FILE *arq, Ttoken *token, TErro *erro);
bool termo(FILE *arq, Ttoken *token, TErro *erro);
bool fator(FILE *arq, Ttoken *token, TErro *erro);
//

//auxiliar
void printa_erro(Ttoken token, TErro erro, char nome[]);
//

#endif //COMPILADOR_HEADER_H
