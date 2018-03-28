//
// Created by mariana on 27/03/18.
//

#include "parser.h"
#include "scanner.h"

void parser(FILE *arq) {
    Ttoken token;
    TErro erro;
    erro.coluna = 0;
    erro.linha = 1;
    while (!feof(arq)) {
        scanner(arq, &token, &erro);
        programa(arq, &token, &erro);
        printf("Parser ok \n");
    }
}

void programa(FILE *arq, Ttoken *token, TErro *erro) {
    int sequencia[5] = {TIPO_PR_INT, TIPO_MAIN, TIPO_ABRE_PARENTESE, TIPO_FECHA_PARENTESE, TIPO_EOF};
    char nome[5][21] = {"ERRO SEM INT", "ERRO SEM MAIN", "ERRO SEM (", "ERRO SEM )", "ERRO FECHA } SEM EOF"};
    for (int i = 0; i < 5; ++i) {
        if ((*token).classificacao == sequencia[i]) {
            if ((*token).classificacao == TIPO_FECHA_PARENTESE) {
                scanner(arq, token, erro);
                bloco(arq, token, erro);
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
        scanner(arq, token, erro);
        while (flag) {
            if (decl_var(arq, token, erro)) {
                scanner(arq, token, erro);
            } else if (!comando(arq, token, erro)) {
                flag = false;
            }
        }
//        while (decl_var(arq, token, erro) || comando(arq, token, erro) && (*token).classificacao != TIPO_FECHA_CHAVE) {
//            scanner(arq, token, erro);
//        }
        if ((*token).classificacao == TIPO_FECHA_CHAVE) {
            scanner(arq, token, erro);
            return true;
        }
        printa_erro(*token, *erro, "ERRO BLOCO SEM } OU SEM DECL_VAR OU SEM COMANDO");
    }
    return false;
}

bool decl_var(FILE *arq, Ttoken *token, TErro *erro) {
    int i;
    int sequencia[9] = {TIPO_PR_INT, TIPO__PR_FLOAT, TIPO_PR_CHAR, TIPO_ID, TIPO_RECEBE, TIPO_INT, TIPO_FLOAT,
                        TIPO_CHAR, TIPO_PV};
    char nome[4][33] = {"ERRO SEM ID", "ERRO SEM , OU ;", "ERRO SEM DIGITO OU FLOAT OU CHAR", "ERRO SEM ;"};
    for (i = 0; i < 3; ++i) {
        if ((*token).classificacao == sequencia[i]) {
            scanner(arq, token, erro);
            if ((*token).classificacao == TIPO_ID) {
                scanner(arq, token, erro);
                if ((*token).classificacao == TIPO_VIRGULA) {
                    while ((*token).classificacao == TIPO_VIRGULA) {
                        scanner(arq, token, erro);
                        if ((*token).classificacao == TIPO_ID) {
                            scanner(arq, token, erro);
                        } else {
                            goto erro_id;
                        }
                    }
                    if ((*token).classificacao == sequencia[8]) {
                        return true;
                    } else {
                        printa_erro(*token, *erro, nome[3]);
                    }
                    return true;
                } else if ((*token).classificacao == sequencia[8]) {
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
    }else if ((*token).classificacao == TIPO_IF) {
        scanner(arq, token, erro);
        if ((*token).classificacao == TIPO_ABRE_PARENTESE) {
            scanner(arq, token, erro);
            if (expr_relacional(arq, token, erro)) {
                if ((*token).classificacao == TIPO_FECHA_PARENTESE) {
                    scanner(arq, token, erro);
                    if (comando(arq, token, erro)){
                        if((*token).classificacao == TIPO_ELSE){
                            scanner(arq, token, erro);
                            if (comando(arq, token, erro)){
                                return true;
                            }else{
                                printa_erro(*token, *erro, "ERRO ELSE SEM COMANDO");
                            }
                        }
                        return true;
                    }else{
                        printa_erro(*token, *erro, "ERRO IF SEM COMANDO");
                    }
                } else{
                    printa_erro(*token, *erro, "ERRO IF SEM )");
                }
            }else{
                printa_erro(*token, *erro, "ERRO IF SEM EXP_RELACIONAL");
            }
        }else{
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
    char nome[3][26] = {"ERRO ATRIBUICAO SEM =", "ERRO ATRIBUICAO EXP_ARIT", "ERRO SEM ;"};
    if ((*token).classificacao == TIPO_ID) {
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
    if (expr_arit(arq, token, erro)) {
        if ((*token).classificacao >= TIPO_MENOR && (*token).classificacao <= TIPO_DIFERENTE) {
            scanner(arq, token, erro);
            if (expr_arit(arq, token, erro)) {
                return true;
            } else {
                printa_erro(*token, *erro, "Erro sem EXP_ARIT");
            }
        } else {
            printa_erro(*token, *erro, "Erro sem OP_RELACIONAL");
        }
    }
    return false;
}

bool expr_arit(FILE *arq, Ttoken *token, TErro *erro) {
    if (termo(arq, token, erro)) {
        if ((*token).classificacao == TIPO_SOMA) {
            scanner(arq, token, erro);
            return expr_arit(arq, token, erro);
        } else if ((*token).classificacao == TIPO_SUB) {
            scanner(arq, token, erro);
            return expr_arit(arq, token, erro);
        } else {
            return true;
        }
    }
    return false;
}

bool termo(FILE *arq, Ttoken *token, TErro *erro) {
    if ((fator(arq, token, erro))) {
        scanner(arq, token, erro);
        if ((*token).classificacao == TIPO_MULT) {
            scanner(arq, token, erro);
            return termo(arq, token, erro);
        } else if ((*token).classificacao == TIPO_DIV) {
            scanner(arq, token, erro);
            return termo(arq, token, erro);
        } else {
            return true;
        }
    }
    return false;
}

bool fator(FILE *arq, Ttoken *token, TErro *erro) {
    int sequencia[4] = {TIPO_ID, TIPO_INT, TIPO_FLOAT, TIPO_CHAR};
    if ((*token).classificacao == TIPO_ABRE_PARENTESE) {
        scanner(arq, token, erro);
        if (expr_arit(arq, token, erro)) {
            if ((*token).classificacao == TIPO_FECHA_PARENTESE) {
                return true;
            } else {
                printa_erro(*token, *erro, "Erro sem )");
            }
        } else {
            printa_erro(*token, *erro, "Erro sem EXP_ARIT");
        }
    } else {
        for (int i = 0; i < 4; ++i) {
            if ((*token).classificacao == sequencia[i]) {
                return true;
            }
        }
    }
    return false;
}
