//
// Created by mariana on 27/03/18.
//

#include "scanner.h"

void scanner(FILE *arq, Ttoken *t, TErro *erro) {
    int qtd = 0, status;
    static char c = ' ';
    while (1) {
        while (isblank(c)) { //ESPACO
            c = fgetc(arq);
            (*erro).coluna++;
        }
        if (c == '+' || c == '-' || c == '*' || c == '=') { //ARITMETICO
            verArit(arq, &c, &qtd, t, erro);
            break;
        } else if (c == '>' || c == '<' || c == '!') { //RELACIONAL
            if (verRelacional(arq, &c, &qtd, t, erro)) {
                break;
            }
            printa_erro(*t, *erro, "Erro Relacional");
        } else if (c == ')' || c == '(' || c == '{' || c == '}' || c == ',' || c == ';') { //ESPECIAL
            verEspecial(arq, &c, &qtd, t, erro);
            break;
        } else if (isdigit(c) || c == '.') { //INTEIRO E FLOAT
            if (verInt_Float(arq, &c, &qtd, t, erro)) {
                break;
            }
            printa_erro(*t, *erro, (*erro).nome);
        } else if (isalpha(c) || c == '_') { //ID E PR
            ID_PR(arq, &c, &qtd, t, erro);
            break;
        } else if (c == 39) { //CHAR
            if (verChar(arq, &c, &qtd, t, erro)) {
                (*t).classificacao = TIPO_CHAR;
            } else {
                printa_erro(*t, *erro, "Erro Char");
            }
            break;
        } else if (c == 47) { //COMENTARIO E DIVISAO
            status = comentario(arq, &c, &qtd, t, erro);
            if (status == false) {
                printa_erro(*t, *erro, "Erro Comentario");
            } else if (status == TIPO_DIV) {
                break;
            }
        } else if (feof(arq)) { //FIM DE ARQUIVO
            eof(t);
            return;
        } else if (c == 10) { // \N QUEBRA DE LINHA
            (*erro).coluna = 0;
            (*erro).linha++;
            c = fgetc(arq);
        } else { //ERRO
            (*t).lexema[qtd] = '\0';
            printa_erro(*t, *erro, "Caracter invalido");
        }
    }
    if (qtd != 0) {
        (*t).lexema[qtd] = '\0';
    }
}

void insere(Ttoken *token, int *qtd, char *c, FILE *arq, TErro *erro) {
    (*token).lexema[(*qtd)++] = *c;
    *c = fgetc(arq);
    (*erro).coluna++;
}

void eof(Ttoken *token) {
    char eof[] = "EOF";
    strcpy((*token).lexema, eof);
    (*token).classificacao = TIPO_EOF;
}

int comentario(FILE *arq, char *c, int *qtd, Ttoken *token, TErro *erro) {
    insere(token, qtd, c, arq, erro);
    if (*c == 47) {//6
        (*qtd)--;
        (*token).lexema[0] = '\0';
        while (*c != 10) {
            *c = fgetc(arq);
            (*erro).coluna++;
            if (feof(arq)) {//11
                eof(token);
                return true;
            }
        }
        *c = fgetc(arq);
        (*erro).coluna = 0;
        (*erro).linha++;
        return true;
    } else if (*c == '*') {//9
        (*qtd)--;
        *c = fgetc(arq);
        if (*c == 10) {
            (*erro).coluna = 0;
            (*erro).linha++;
        } else {
            (*erro).coluna++;
        }
        volta:
        while (*c != '*') {
            *c = fgetc(arq);
            if (*c == 10) {
                (*erro).coluna = 0;
                (*erro).linha++;
            } else {
                (*erro).coluna++;
            }
            if (feof(arq)) {//Erro
                return false;
            }
        }
        while (*c == '*') {
            *c = fgetc(arq);
            (*erro).coluna++;
            if (feof(arq)) {//Erro
                return false;
            }
        }
        if (*c == 47) {
            *c = fgetc(arq);
            return true;
        } else {
            goto volta;
        }
        return true;
    }
    (*token).classificacao = TIPO_DIV;
    return TIPO_DIV;
}

void ID_PR(FILE *arq, char *c, int *qtd, Ttoken *token, TErro *erro) {
    int flag = 0;
    while (isalpha(*c) || *c == '_' || isdigit(*c) || !feof(arq)) {
        insere(token, qtd, c, arq, erro);
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

void verArit(FILE *arq, char *c, int *qtd, Ttoken *token, TErro *erro) {
    if (*c == '+') {
        (*token).classificacao = TIPO_SOMA;
    } else if (*c == '-') {
        (*token).classificacao = TIPO_SUB;
    } else if (*c == '*') {
        (*token).classificacao = TIPO_MULT;
    } else {
        insere(token, qtd, c, arq, erro);
        if (*c == '=') {
            (*token).classificacao = TIPO_IGUAL;
        } else {
            (*token).classificacao = TIPO_RECEBE;
            return;
        }
    }
    insere(token, qtd, c, arq, erro);
}

bool verRelacional(FILE *arq, char *c, int *qtd, Ttoken *token, TErro *erro) {
    if (*c == '<') {
        insere(token, qtd, c, arq, erro);
        if (*c == '=') {
            (*token).classificacao = TIPO_MENOR_IGUAL;
        } else {
            (*token).classificacao = TIPO_MENOR;
            return true;
        }
    } else if (*c == '>') {
        insere(token, qtd, c, arq, erro);
        if (*c == '=') {
            (*token).classificacao = TIPO_MAIOR_IGUAL;
        } else {
            (*token).classificacao = TIPO_MAIOR;
            return true;
        }
    } else {
        insere(token, qtd, c, arq, erro);
        if (*c == '=') {
            (*token).classificacao = TIPO_DIFERENTE;
        } else {
            return false;
        }
    }
    insere(token, qtd, c, arq, erro);
    return true;
}

void verEspecial(FILE *arq, char *c, int *qtd, Ttoken *token, TErro *erro) {
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
    insere(token, qtd, c, arq, erro);
}

bool verInt_Float(FILE *arq, char *c, int *qtd, Ttoken *token, TErro *erro) {
    while (isdigit(*c)) {
        insere(token, qtd, c, arq, erro);
    }
    if (*c == '.') {
        insere(token, qtd, c, arq, erro);
        if (isdigit(*c)) {
            while (isdigit(*c)) {
                insere(token, qtd, c, arq, erro);
            }
        } else {
            strcpy((*erro).nome, "Erro Float");
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

bool verChar(FILE *arq, char *c, int *qtd, Ttoken *token, TErro *erro) {
    insere(token, qtd, c, arq, erro);
    if (isalpha(*c) || isdigit(*c)) {
        insere(token, qtd, c, arq, erro);
        if (*c == 39) {
            insere(token, qtd, c, arq, erro);
            return true;
        } else {
            return false;
        }
    }
    return false;
}
