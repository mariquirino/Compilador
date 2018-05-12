//
// Created by Mariana on 03/03/2018.
//
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "enum.h"

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

//auxiliar
void printa_erro(Ttoken token, TErro erro, char nome[]);
//

#endif //COMPILADOR_HEADER_H


#ifndef ESTRUTURAS
#define ESTRUTURAS

typedef struct noStack{
    char lexema[30];
    int tipo;
    int escopo;
    struct noStack *prox;
}NoStack;

typedef NoStack * Stack;
#endif

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
