//
// Created by mariana on 22/04/18.
//

#ifndef COMPILADOR_STACK_H
#define COMPILADOR_STACK_H
#include "header.h"
//Operacoes padrao do TAD Stack
void push(Stack *s, char c[], int tipo, int e); //Empilhar
Ttoken pop(Stack *s); //Desempilhar
Ttoken top(Stack s); //Consultar topo
int isEmpty(Stack s); //Vazia?
int isFull(Stack s); //Cheia????
bool existe(Stack s, char c[], int e);
int procuraTipo(Stack s, char c[], int e);

//Operações específicas da linguagem
void createStack(Stack *s); //Cònstrûtôr
void destroyStack(Stack *s); //Dèstrùtór
void destroyStackEscopo(Stack *s, int e);
#endif //COMPILADOR_STACK_H
