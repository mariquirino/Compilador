//
// Created by mariana on 10/03/18.
//

#ifndef COMPILADOR_ENUM_H
#define COMPILADOR_ENUM_H

enum tipo {
    TIPO_INT = 10,
    TIPO_FLOAT = 11,
    TIPO_CHAR = 12,
    TIPO_ID = 13,
    TIPO_SOMA = 20,
    TIPO_SUB = 21,
    TIPO_MULT = 22,
    TIPO_DIV = 23,
    TIPO_RECEBE = 24,
    TIPO_IGUAL = 30,
    TIPO_MENOR = 31,
    TIPO_MAIOR = 32,
    TIPO_MENOR_IGUAL = 33,
    TIPO_MAIOR_IGUAL = 34,
    TIPO_DIFERENTE = 35,
    TIPO_EOF = 17,
    TIPO_MAIN = 40,
    TIPO_IF = 41,
    TIPO_ELSE = 42,
    TIPO_WHILE = 43,
    TIPO_DO = 44,
    TIPO_FOR = 45,
    TIPO_PR_INT = 46,
    TIPO__PR_FLOAT = 47,
    TIPO_PR_CHAR = 48,
    TIPO_PV = 50,
    TIPO_ABRE_PARENTESE = 51,
    TIPO_FECHA_PARENTESE = 52,
    TIPO_ABRE_CHAVE = 53,
    TIPO_FECHA_CHAVE = 54,
    TIPO_VIRGULA = 55
};

char* getEnum(int i);

#endif //COMPILADOR_ENUM_H
