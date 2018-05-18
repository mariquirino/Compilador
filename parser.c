//
// Created by mariana on 27/03/18.
//

#include "parser.h"
#include "scanner.h"
#include "stack.h"

int escopo = 0, contador = 0, label = 1;
Stack s;

void parser(FILE *arq) {
    Ttoken token;
    TErro erro;
    erro.coluna = 0;
    erro.linha = 1;
    while (!feof(arq)) {
        createStack(&s);
        scanner(arq, &token, &erro);
        if (token.classificacao != TIPO_EOF) {
            programa(arq, &token, &erro);
        }
//        printf("Parser ok \n");
        destroyStack(&s);
    }
}

void programa(FILE *arq, Ttoken *token, TErro *erro) {
    int sequencia[5] = {TIPO_PR_INT, TIPO_MAIN, TIPO_ABRE_PARENTESE, TIPO_FECHA_PARENTESE, TIPO_EOF};
    char nome[5][21] = {"ERRO SEM INT", "ERRO SEM MAIN", "ERRO SEM (", "ERRO SEM )", "ERRO FECHA } SEM EOF"};
    for (int i = 0; i < 5; ++i) {
        if ((*token).classificacao == sequencia[i]) {
            if ((*token).classificacao == TIPO_FECHA_PARENTESE) {
                scanner(arq, token, erro);
                if (!bloco(arq, token, erro)) {
                    printa_erro(*token, *erro, "ERRO BLOCO SEM {");
                }
            } else {
                scanner(arq, token, erro);
            }
        } else {
            printa_erro(*token, *erro, nome[i]);
        }
    }
}

bool bloco(FILE *arq, Ttoken *token, TErro *erro) {
    bool flag = true;
    if ((*token).classificacao == TIPO_ABRE_CHAVE) {
        escopo++;
        scanner(arq, token, erro);
        while (decl_var(arq, token, erro)) {
            scanner(arq, token, erro);
        }
        while (flag) {
            flag = comando(arq, token, erro);
        }
        if ((*token).classificacao == TIPO_FECHA_CHAVE) {
            destroyStackEscopo(&s, escopo);
            escopo--;
            scanner(arq, token, erro);
            return true;
        }
        printa_erro(*token, *erro, "ERRO BLOCO SEM }");
    }
    return false;
}

bool decl_var(FILE *arq, Ttoken *token, TErro *erro) {
    int i;
    int sequencia[9] = {TIPO_PR_INT, TIPO__PR_FLOAT, TIPO_PR_CHAR, TIPO_ID, TIPO_PV};
    char nome[2][33] = {"ERRO SEM ID", "ERRO SEM , OU ;"};
    for (i = 0; i < 3; ++i) {
        if ((*token).classificacao == sequencia[i]) {
            scanner(arq, token, erro);
            loop:
            if ((*token).classificacao == TIPO_ID) {
                if (existe(s, (*token).lexema, escopo) == false) {
                    push(&s, (*token).lexema, sequencia[i] - 36, escopo);
                } else {
                    printa_erro(*token, *erro, "Variavel ja declarada no mesmo bloco");
                }
                scanner(arq, token, erro);
                if ((*token).classificacao == TIPO_VIRGULA) {
                    scanner(arq, token, erro);
                    if ((*token).classificacao == TIPO_ID) {
                        goto loop;
                    } else {
                        goto erro_id;
                    }
                } else if ((*token).classificacao == sequencia[4]) {
                    return true;
                } else {
                    printa_erro(*token, *erro, nome[1]);
                }
            } else {
                erro_id:
                printa_erro(*token, *erro, nome[0]);
            }
        }
    }
    return false;
}

bool comando(FILE *arq, Ttoken *token, TErro *erro) {
    if (comando_basico(arq, token, erro)) {
        return true;
    } else if (iteracao(arq, token, erro)) {
        return true;
    } else if ((*token).classificacao == TIPO_IF) {
        scanner(arq, token, erro);
        if ((*token).classificacao == TIPO_ABRE_PARENTESE) {
            scanner(arq, token, erro);
            printf("%s goto L%d\n", expr_relacional(arq, token, erro), label++);
                if ((*token).classificacao == TIPO_FECHA_PARENTESE) {
                    scanner(arq, token, erro);
                    if (comando(arq, token, erro)) {
                        if ((*token).classificacao == TIPO_ELSE) {
                            scanner(arq, token, erro);
                            if (comando(arq, token, erro)) {
                                return true;
                            } else {
                                printa_erro(*token, *erro, "ERRO ELSE SEM COMANDO");
                            }
                        }
                        return true;
                    } else {
                        printa_erro(*token, *erro, "ERRO IF SEM COMANDO");
                    }
                } else {
                    printa_erro(*token, *erro, "ERRO IF SEM )");
                }
        } else {
            printa_erro(*token, *erro, "ERRO IF SEM (");
        }
    }
    return false;
}

bool comando_basico(FILE *arq, Ttoken *token, TErro *erro) {
    if (atribuicao(arq, token, erro)) {
        return true;
    } else if (bloco(arq, token, erro)) {
        return true;
    }
    return false;
}

bool iteracao(FILE *arq, Ttoken *token, TErro *erro) {
    if ((*token).classificacao == TIPO_WHILE) {
        scanner(arq, token, erro);
        if ((*token).classificacao == TIPO_ABRE_PARENTESE) {
            scanner(arq, token, erro);
            printf("%s goto L%d\n", expr_relacional(arq, token, erro), label++);
            if ((*token).classificacao == TIPO_FECHA_PARENTESE) {
                scanner(arq, token, erro);
                if (comando(arq, token, erro)) {
                    return true;
                } else {
                    printa_erro(*token, *erro, "Erro sem Comando");
                }
            } else {
                printa_erro(*token, *erro, "Erro sem )");
            }
        } else {
            printa_erro(*token, *erro, "Erro sem (");
        }
    } else if ((*token).classificacao == TIPO_DO) {
        scanner(arq, token, erro);
        if (comando(arq, token, erro)) {
            if ((*token).classificacao == TIPO_WHILE) {
                scanner(arq, token, erro);
                if ((*token).classificacao == TIPO_ABRE_PARENTESE) {
                    scanner(arq, token, erro);
                    printf("%s goto L%d\n", expr_relacional(arq, token, erro), label++);
                        if ((*token).classificacao == TIPO_FECHA_PARENTESE) {
                            scanner(arq, token, erro);
                            if ((*token).classificacao == TIPO_PV) {
                                scanner(arq, token, erro);
                                return true;
                            } else {
                                printa_erro(*token, *erro, "Erro sem ;");
                            }
                        } else {
                            printa_erro(*token, *erro, "Erro sem )");
                        }
                } else {
                    printa_erro(*token, *erro, "Erro sem (");
                }
            } else {
                printa_erro(*token, *erro, "Erro DO sem WHILE");
            }
        } else {
            printa_erro(*token, *erro, "Erro sem Comando");
        }
    }
    return false;
}

bool atribuicao(FILE *arq, Ttoken *token, TErro *erro) {
    char c[30];
    int i = 0, aux;
    TAnt ant;
    char nome[2][33] = {"ERRO ATRIBUICAO SEM =", "ERRO ESPERAVA ;"};
    if ((*token).classificacao == TIPO_ID) {
        strcpy(c, (*token).lexema);
        aux = procuraTipo(s, (*token).lexema, escopo);
        if (aux == -1) {
            printa_erro(*token, *erro, "ERRO ATRIBUICAO COM VARIAVEL NAO DECLARADA");
        }
        scanner(arq, token, erro);
        if ((*token).classificacao == TIPO_RECEBE) {
            i++;
            scanner(arq, token, erro);
            ant = expr_arit(arq, token, erro);
            printf("%s = %s\n", c, ant.lexema);
            if (aux != ant.tipo && (!(aux == TIPO_FLOAT && (ant.tipo == TIPO_INT || ant.tipo == TIPO_FLOAT)))) {
                printa_erro(*token, *erro, "ERRO COMPATIBILIDADE");
            }
            if ((*token).classificacao == TIPO_PV) {
                scanner(arq, token, erro);
                return true;
            }
        }
    } else {
        return false;
    }
    printa_erro(*token, *erro, nome[i]);
}

char* expr_relacional(FILE *arq, Ttoken *token, TErro *erro) {
    TAnt aux, aux2;
    aux = expr_arit(arq, token, erro);
    if ((*token).classificacao >= TIPO_IGUAL && (*token).classificacao <= TIPO_DIFERENTE) {
        strcat(aux.lexema, (*token).lexema);
        scanner(arq, token, erro);
        aux2 = expr_arit(arq, token, erro);
        strcat(aux.lexema, aux2.lexema);
        if (aux.tipo != aux2.tipo && (!((aux.tipo == TIPO_INT || aux.tipo == TIPO_FLOAT) &&
                                        (aux2.tipo == TIPO_INT || aux2.tipo == TIPO_FLOAT)))) {
            printa_erro(*token, *erro, "ERRO COMPATIBILIDADE");
        }
        char *c;
        c = aux.lexema;
        return c;
    }
    printa_erro(*token, *erro, "ERRO SEM EXPRESSAO RELACIONAL");
}

TAnt expr_arit(FILE *arq, Ttoken *token, TErro *erro) {
    int i = 0;
    TAnt aux, ant;
    ant = termo(arq, token, erro);
    continua:
    if ((*token).classificacao == TIPO_SOMA || (*token).classificacao == TIPO_SUB) {
        strcat(ant.lexema, (*token).lexema);
        scanner(arq, token, erro);
        aux = termo(arq, token, erro);
        strcat(ant.lexema, aux.lexema);
        if (aux.tipo == TIPO_FLOAT && ant.tipo == TIPO_INT) {
            ant.tipo = aux.tipo;
        } else if ((ant.tipo == TIPO_CHAR && aux.tipo != TIPO_CHAR) ||
                   (ant.tipo != TIPO_CHAR && aux.tipo == TIPO_CHAR)) {
            printa_erro(*token, *erro, "ERRO COMPATIBILIDADE");
        }
        if (i == 0 && contador == 0) {
            char c[3];
            printf("T%d = %s \n", contador, ant.lexema);
            strcpy(ant.lexema, "T");
            sprintf(c, "%d", contador);
            strcat(ant.lexema, c);
        } else {
            printf("T%d = %s \n", contador, ant.lexema);
            char c[3];
            strcpy(ant.lexema, "T");
            sprintf(c, "%d", contador);
            strcat(ant.lexema, c);

        }
        contador++;
        i++;
        goto continua;
    }
    return ant;
}

TAnt termo(FILE *arq, Ttoken *token, TErro *erro) {
    int i = 0;
    TAnt aux, ant;
    ant = fator(arq, token, erro);
    continua:
    scanner(arq, token, erro);
    if ((*token).classificacao == TIPO_MULT || (*token).classificacao == TIPO_DIV) {
        if ((*token).classificacao == TIPO_DIV) {
            if (ant.tipo == TIPO_INT) {
                ant.tipo = TIPO_FLOAT;
            }
        }
        strcat(ant.lexema, (*token).lexema);
        scanner(arq, token, erro);
        aux = fator(arq, token, erro);
        if (aux.tipo == TIPO_FLOAT && ant.tipo == TIPO_INT) {
            ant.tipo = aux.tipo;
        } else if ((ant.tipo == TIPO_CHAR && aux.tipo != TIPO_CHAR) ||
                   (ant.tipo != TIPO_CHAR && aux.tipo == TIPO_CHAR)) {
            printa_erro(*token, *erro, "ERRO COMPATIBILIDADE");
        }
        if (i == 0) {
            char c[3];
            strcat(ant.lexema, (*token).lexema);
            printf("T%d = %s \n", contador, ant.lexema);
            strcpy(ant.lexema, "T");
            sprintf(c, "%d", contador);
            strcat(ant.lexema, c);
        } else {
            printf("T%d = %s \n", contador, ant.lexema);
            char c[3];
            strcpy(ant.lexema, "T");
            sprintf(c, "%d", contador);
            strcat(ant.lexema, c);
        }
        contador++;
        i++;
        goto continua;
    }
    return ant;
}

TAnt fator(FILE *arq, Ttoken *token, TErro *erro) {
    TAnt aux;
    int sequencia[4] = {TIPO_ID, TIPO_INT, TIPO_FLOAT, TIPO_CHAR};
    if ((*token).classificacao == TIPO_ABRE_PARENTESE) {
        scanner(arq, token, erro);
        aux = expr_arit(arq, token, erro);
        if ((*token).classificacao == TIPO_FECHA_PARENTESE) {
            return aux;
        }
        printa_erro(*token, *erro, "Erro sem )");
    } else {
        for (int i = 0; i < 4; ++i) {
            if ((*token).classificacao == sequencia[i]) {
                if (sequencia[i] == TIPO_ID) {
                    sequencia[i] = procuraTipo(s, (*token).lexema, escopo);
                    if (sequencia[i] == -1) {
                        printa_erro(*token, *erro, "VARIAVEL NAO DECLARADA");
                    }
                }
                strcpy(aux.lexema, (*token).lexema);
                aux.tipo = sequencia[i];
                return aux;
            }
        }
    }
    printa_erro(*token, *erro, "ERRO SEM FATOR");
}