//
// Created by mariana on 27/03/18.
//

#include "parser.h"
#include "scanner.h"
#include "stack.h"

int escopo = 0, tipo;
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
        printf("Parser ok \n");
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
            if (expr_relacional(arq, token, erro)) {
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
                printa_erro(*token, *erro, "ERRO IF SEM EXP_RELACIONAL");
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
            if (expr_relacional(arq, token, erro)) {
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
                printa_erro(*token, *erro, "Erro sem EXP_ARIT");
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
                    if (expr_relacional(arq, token, erro)) {
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
                        printa_erro(*token, *erro, "Erro sem EXP_ARIT");
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
    int i = 0;
    char nome[3][33] = {"ERRO ATRIBUICAO SEM =", "ERRO ATRIBUICAO EXP_ARIT OU TERMO", "ERRO SEM ;"};
    if ((*token).classificacao == TIPO_ID) {
        tipo = procuraTipo(s, (*token).lexema, escopo);
        scanner(arq, token, erro);
        if ((*token).classificacao == TIPO_RECEBE) {
            i++;
            scanner(arq, token, erro);
            if (expr_arit(arq, token, erro)) {//AQUI EH EXP ARIT
                i++;
                if ((*token).classificacao == TIPO_PV) {
                    scanner(arq, token, erro);
                    return true;
                }
            }
        }
    } else {
        return false;
    }
    printa_erro(*token, *erro, nome[i]);
}

bool expr_relacional(FILE *arq, Ttoken *token, TErro *erro) {
    tipo = 0;
    if (expr_arit(arq, token, erro)) {
        if ((*token).classificacao >= TIPO_IGUAL && (*token).classificacao <= TIPO_DIFERENTE) {
            scanner(arq, token, erro);
            if (expr_arit(arq, token, erro)) {
                return true;
            }
            printa_erro(*token, *erro, "Erro sem EXP_ARIT ou TERMO");
        } else {
            printa_erro(*token, *erro, "Erro sem OP_RELACIONAL");
        }
    }
    return false;
}

bool expr_arit(FILE *arq, Ttoken *token, TErro *erro) {
    continua:
    if (termo(arq, token, erro)) {
        if ((*token).classificacao == TIPO_SOMA || (*token).classificacao == TIPO_SUB) {
            scanner(arq, token, erro);
            goto continua;
        }
        return true;
    }
    return false;
}

bool termo(FILE *arq, Ttoken *token, TErro *erro) {
    continua:
    if ((fator(arq, token, erro))) {
        scanner(arq, token, erro);
        if ((*token).classificacao == TIPO_MULT || (*token).classificacao == TIPO_DIV) {
            scanner(arq, token, erro);
            goto continua;
        }
        return true;
    }
    return false;
}

bool fator(FILE *arq, Ttoken *token, TErro *erro) {
    int aux;
    int sequencia[4] = {TIPO_ID, TIPO_INT, TIPO_FLOAT, TIPO_CHAR};
    if ((*token).classificacao == TIPO_ABRE_PARENTESE) {
        scanner(arq, token, erro);
        if (expr_arit(arq, token, erro)) {
            if ((*token).classificacao == TIPO_FECHA_PARENTESE) {
                return true;
            }
            printa_erro(*token, *erro, "Erro sem )");
        }
        printa_erro(*token, *erro, "Erro sem EXP_ARIT");
    } else {
        for (int i = 0; i < 4; ++i) {
            if ((*token).classificacao == sequencia[i]) {
                checa:
                if (tipo == -1) {
                    printa_erro(*token, *erro, "VARIAVEL NAO DECLARADA");
                } else if (tipo == 0) {
                    return true;
                } else if (tipo == sequencia[i]) {
                    return true;
                } else if (tipo == TIPO_FLOAT && sequencia[i] == TIPO_INT) {
                    return true;
                } else if (sequencia[i] == TIPO_ID) {
                    aux = procuraTipo(s, (*token).lexema, escopo);
                    if (aux == -1) {
                        printa_erro(*token, *erro, "VARIAVEL NAO DECLARADA");
                    }
                    sequencia[i] = aux;
                    goto checa;
                }
                printf("Tipo: %s com tipo: %s \n", getEnum(tipo), getEnum(sequencia[i]));
                printa_erro(*token, *erro, "ERRO COMPATIBILIDADE DE TIPOS");
            }
        }
    }
    return false;
}