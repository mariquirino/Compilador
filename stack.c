//
// Created by mariana on 22/04/18.
//

#include <stdlib.h>
#include "stack.h"

void push(Stack *s, char c[], int tipo, int e) {
    NoStack *novo;
    novo = (NoStack *) malloc(sizeof(NoStack));
    strcpy(novo->lexema, c);
    novo->tipo = tipo;
    novo->prox = *s;
    novo->escopo = e;
    *s = novo;
}

Ttoken pop(Stack *s) {
    NoStack *aux;
    Ttoken del;
    aux = *s;
    *s = (*s)->prox;
    strcpy(del.lexema, aux->lexema);
    del.classificacao = aux->tipo;
    free(aux);
    return del;
}

Ttoken top(Stack s) {
    Ttoken aux;
    strcpy(aux.lexema, s->lexema);
    aux.classificacao = s->tipo;
    return aux;
}

int isEmpty(Stack s) {
    if (s == NULL) {
        return TRUE;
    } else {
        return FALSE;
    }
}

int isFull(Stack s) {
    return FALSE;
}

bool existe(Stack s, char c[], int e){
    while (s != NULL){
        if(strcmp(s->lexema, c) == 0){
            if(e == s->escopo){
                return true;
            }
            return false;
        }
        s = s->prox;
    }
    return false;
}

int procuraTipo(Stack s, char c[], int e) {
    int tipo = -1;
    while (s != NULL){
        if(strcmp(s->lexema, c) == 0){
            if(e >= s->escopo){
                tipo = s->tipo;
            }
            return tipo;
        }
        s = s->prox;
    }
    return tipo;
}

void createStack(Stack *s) {
    *s = NULL;
}

void destroyStack(Stack *s) {
    NoStack *aux;
    while (*s != NULL) {
        aux = *s;
        *s = (*s)->prox;
        free(aux);
    }
}

void destroyStackEscopo(Stack *s, int e){
    NoStack *aux;
    while (*s != NULL) {
        aux = *s;
        if(aux->escopo == e) {
            *s = (*s)->prox;
            free(aux);
        }else {
            break;
        }
    }
}