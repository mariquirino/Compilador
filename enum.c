//
// Created by mariana on 10/03/18.
//
#include <stdio.h>
#include "enum.h"


char* getEnum(int i) {
    switch (i) {
        case TIPO_INT:
            return "Inteiro";
        case TIPO_FLOAT :
            return "Float";
        case TIPO_CHAR :
            return "Char";
        case TIPO_ID :
            return "Identificador";
        case TIPO_SOMA :
            return "Soma";
        case TIPO_SUB :
            return "Subtracao";
        case TIPO_MULT :
            return "Multiplicacao";
        case TIPO_DIV :
            return "Divisao";
        case TIPO_RECEBE :
            return "Recebe";
        case TIPO_IGUAL :
            return "Igual";
        case TIPO_MENOR :
            return "Menor";
        case TIPO_MAIOR :
            return "Maior";
        case TIPO_MENOR_IGUAL :
            return "Menor Igual";
        case TIPO_MAIOR_IGUAL :
            return "Maior Igual";
        case TIPO_DIFERENTE :
            return "Diferente";
        case tipo_EOF :
            return "Fim De Arquivo";
        case TIPO_MAIN :
            return "Palavra Reservada: main";
        case TIPO_IF :
            return "Palavra Reservada: if";
        case TIPO_ELSE :
            return "Palavra Reservada: else";
        case TIPO_WHILE :
            return "Palavra Reservada: while";
        case TIPO_DO :
            return "Palavra Reservada: do";
        case TIPO_FOR :
            return "Palavra Reservada: for";
        case TIPO_PR_INT :
            return "Palavra Reservada: int";
        case TIPO__PR_FLOAT :
            return "Palavra Reservada: float";
        case TIPO_PR_CHAR :
            return "Palavra Reservada: char";
        case TIPO_PV :
            return "Ponto e Virgula";
        case TIPO_ABRE_PARENTESE :
            return "Abre Patentese";
        case TIPO_FECHA_PARENTESE :
            return "Fecha Parentese";
        case TIPO_ABRE_CHAVE :
            return "Abre Chave";
        case TIPO_FECHA_CHAVE :
            return "Fecha Chave";
        case TIPO_VIRGULA :
            return "Virgula";
    }
}