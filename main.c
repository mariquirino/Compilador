#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "header.h"

int main(int argc, char *argv[]) {
    Ttoken token;
    token.linha = 1;
    token.coluna = -1;
    if(argc < 2){ //Argc é o tamanho do vetor de argv
        printf("Falta nome do arquivo. \n");
        exit(-1);
    }
    FILE *arq = fopen(argv[1], "r+b"); //Pos em que o nome do arq ta no argv
    fseek(arq, 0, 0);
    while (!feof(arq)) {
        if (scanner(arq, &token)) {
            printf("Token: %s, classificacao: %d\n", token.lexema, token.classificacao, token.coluna);
        } else {
            printf("ERRO na linha: %d, coluna: %d, ultimo token lido: %s \n", token.linha, token.coluna + 1,
                   token.lexema);
            break;
        }
    }
    fclose(arq);
    return 0;
}

bool scanner(FILE *arq, Ttoken *t) {
    int qtd = 0, status;
    static char c = ' ';
    while (1) {
        while (isblank(c)) { //ESPACO
            c = fgetc(arq);
            (*t).coluna++;
        }
        if (c == '+' || c == '-' || c == '*' || c == '=') { //ARITMETICO
            verArit(arq, &c, &qtd, t);
            break;
        } else if (c == '>' || c == '<' || c == '!') { //RELACIONAL
            if (verRelacional(arq, &c, &qtd, t)) {
                break;
            }
            goto fim;
        } else if (c == ')' || c == '(' || c == '{' || c == '}' || c == ',' || c == ';') { //ESPECIAL
            verEspecial(arq, &c, &qtd, t);
            break;
        } else if (isdigit(c) || c == '.') { //INTEIRO E FLOAT
            if (verInt_Float(arq, &c, &qtd, t)) {
                break;
            }
            goto fim;
        } else if (isalpha(c) || c == '_') { //ID E PR
            ID_PR(arq, &c, &qtd, t);
            break;
        } else if (c == 39) { //CHAR
            if (verChar(arq, &c, &qtd, t)) {
                (*t).classificacao = TIPO_CHAR;
            } else {
                goto fim;
            }
            break;
        } else if (c == 47) { //COMENTARIO E DIVISAO
            status = comentario(arq, &c, &qtd, t);
            if (status == FALSE) {
                goto fim;
            } else if (status == TIPO_DIV) {
                break;
            }
        } else if (feof(arq)) { //FIM DE ARQUIVO
            eof(t);
            return true;
        } else if (c == 10) { // \N QUEBRA DE LINHA
            (*t).coluna = 0;
            (*t).linha++;
            c = fgetc(arq);
        } else { //ERRO
            fim:
            (*t).lexema[qtd] = '\0';
            return false;
        }
    }
    if (qtd != 0) {
        (*t).lexema[qtd] = '\0';
    }
    return true;
}

void insere(Ttoken *token, int *qtd, char *c, FILE *arq) {
    (*token).lexema[(*qtd)++] = *c;
    *c = fgetc(arq);
    (*token).coluna++;
}

void eof(Ttoken *token) {
    char eof[] = "EOF";
    strcpy((*token).lexema, eof);
    (*token).classificacao = tipo_EOF;
}

int comentario(FILE *arq, char *c, int *qtd, Ttoken *token) {
    insere(token, qtd, c, arq);
    if (*c == 47) {//6
        (*qtd)--;
        (*token).lexema[0] = '\0';
        while (*c != 10) {
            *c = fgetc(arq);
            (*token).coluna++;
            if (feof(arq)) {//11
                eof(token);
                return TRUE;
            }
        }
        *c = fgetc(arq);
        (*token).coluna = 0;
        (*token).linha++;
        return TRUE;
    } else if (*c == '*') {//9
        (*qtd)--;
        volta:
        while (*c != '*') {
            *c = fgetc(arq);
            (*token).coluna++;
            if (feof(arq)) {//Erro
                return FALSE;
            }
        }
        while (*c == '*') {
            *c = fgetc(arq);
            (*token).coluna++;
            if (feof(arq)) {//Erro
                return FALSE;
            }
        }
        if (*c == 47) {
            *c = fgetc(arq);
            return TRUE;
        } else {
            goto volta;
        }
        return TRUE;
    }
    (*token).classificacao = TIPO_DIV;
    return TIPO_DIV;
}

void ID_PR(FILE *arq, char *c, int *qtd, Ttoken *token) {
    int flag = 0;
    while (isalpha(*c) || *c == '_' || isdigit(*c) || !feof(arq)) {
        insere(token, qtd, c, arq);
        if (*c == '_' || isdigit(*c)) {
            flag = 1;
        } else if (*c == ' ' || *c == '\n' || !isalpha(*c)) {
            break;
        }
    }
    (*token).lexema[*qtd] = '\0';
    if (flag == 0) {
        (*token).classificacao = verPR((*token).lexema);
    } else {
        (*token).classificacao = TIPO_ID;
    }
}

void verArit(FILE *arq, char *c, int *qtd, Ttoken *token) {
    if (*c == '+') {
        (*token).classificacao = TIPO_SOMA;
    } else if (*c == '-') {
        (*token).classificacao = TIPO_SUB;
    } else if (*c == '*') {
        (*token).classificacao = TIPO_MULT;
    } else {
        insere(token, qtd, c, arq);
        if (*c == '=') {
            (*token).classificacao = TIPO_IGUAL;
        } else {
            (*token).classificacao = TIPO_RECEBE;
            return;
        }
    }
    insere(token, qtd, c, arq);
}

bool verRelacional(FILE *arq, char *c, int *qtd, Ttoken *token) {
    if (*c == '<') {
        insere(token, qtd, c, arq);
        if (*c == '=') {
            (*token).classificacao = TIPO_MENOR_IGUAL;
        } else {
            (*token).classificacao = TIPO_MENOR;
            return true;
        }
    } else if (*c == '>') {
        insere(token, qtd, c, arq);
        if (*c == '=') {
            (*token).classificacao = TIPO_MAIOR_IGUAL;
        } else {
            (*token).classificacao = TIPO_MAIOR;
            return true;
        }
    } else {
        insere(token, qtd, c, arq);
        if (*c == '=') {
            (*token).classificacao = TIPO_DIFERENTE;
        } else {
            return false;
        }
    }
    insere(token, qtd, c, arq);
    return true;
}

void verEspecial(FILE *arq, char *c, int *qtd, Ttoken *token) {
    if (*c == ';') {
        (*token).classificacao = TIPO_PV;
    } else if (*c == '(') {
        (*token).classificacao = TIPO_ABRE_PARENTESE;
    } else if (*c == ')') {
        (*token).classificacao = TIPO_FECHA_PARENTESE;
    } else if (*c == '{') {
        (*token).classificacao = TIPO_ABRE_CHAVE;
    } else if (*c == '}') {
        (*token).classificacao = TIPO_FECHA_CHAVE;
    } else {
        (*token).classificacao = TIPO_VIRGULA;
    }
    insere(token, qtd, c, arq);
}

bool verInt_Float(FILE *arq, char *c, int *qtd, Ttoken *token) {
    while (isdigit(*c)) {
        insere(token, qtd, c, arq);
    }
    if (*c == '.') {
        insere(token, qtd, c, arq);
        if (isdigit(*c)) {
            while (isdigit(*c)) {
                insere(token, qtd, c, arq);
            }
        } else {
            return false;
        }
        (*token).classificacao = TIPO_FLOAT;
        return true;
    }
    (*token).classificacao = TIPO_INT;
    return true;
}

int verPR(char pr[]) {
    char c[9][6] = {"main", "if", "else", "while", "do", "for", "int", "float", "char"};
    if (!strcmp(pr, c[0])) {
        return TIPO_MAIN;
    } else if (!strcmp(pr, c[1])) {
        return TIPO_IF;
    } else if (!strcmp(pr, c[2])) {
        return TIPO_ELSE;
    } else if (!strcmp(pr, c[3])) {
        return TIPO_WHILE;
    } else if (!strcmp(pr, c[4])) {
        return TIPO_DO;
    } else if (!strcmp(pr, c[5])) {
        return TIPO_FOR;
    } else if (!strcmp(pr, c[6])) {
        return TIPO_PR_INT;
    } else if (!strcmp(pr, c[7])) {
        return TIPO__PR_FLOAT;
    } else if (!strcmp(pr, c[8])) {
        return TIPO_PR_CHAR;
    } else {
        return TIPO_ID;
    }
}

bool verChar(FILE *arq, char *c, int *qtd, Ttoken *token) {
    insere(token, qtd, c, arq);
    if (isalpha(*c) || isdigit(*c)) {
        insere(token, qtd, c, arq);
        if (*c == 39) {
            insere(token, qtd, c, arq);
            return true;
        } else {
            return false;
        }
    }
    return false;
}