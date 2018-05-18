//
// Created by mariana on 27/03/18.
//
#include "header.h"

#ifndef COMPILADOR_PARSER_H
#define COMPILADOR_PARSER_H

void parser(FILE *arq);
void programa(FILE *arq, Ttoken *token, TErro *erro);
bool bloco(FILE *arq, Ttoken *token, TErro *erro);
bool decl_var(FILE *arq, Ttoken *token, TErro *erro);
bool comando(FILE *arq, Ttoken *token, TErro *erro);
bool comando_basico(FILE *arq, Ttoken *token, TErro *erro);
bool iteracao(FILE *arq, Ttoken *token, TErro *erro);
bool atribuicao(FILE *arq, Ttoken *token, TErro *erro);
char* expr_relacional(FILE *arq, Ttoken *token, TErro *erro);
TAnt expr_arit(FILE *arq, Ttoken *token, TErro *erro);
TAnt termo(FILE *arq, Ttoken *token, TErro *erro);
TAnt fator(FILE *arq, Ttoken *token, TErro *erro);

#endif //COMPILADOR_PARSER_H
