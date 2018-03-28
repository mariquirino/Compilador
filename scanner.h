//
// Created by mariana on 27/03/18.
//
#include "header.h"

#ifndef COMPILADOR_SCANNER_H
#define COMPILADOR_SCANNER_H

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


#endif //COMPILADOR_SCANNER_H
