#include "biblioteca.h"
#include "quebrador-senha.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define TAM 36

/* vetor amostra */
int possibilidades[TAM];

/* Declarando todas as funcoes que serao usadas */
int existe_repeticao (char vetor[], char elem, int tam);
int estrutura_valida (char vetor[], char elem, int tam, int n_letras, int n_numeros);

void cria_vetor_possibilidade (int *vetor);
void cria_vetor_nulo (int *vetor, int tam);
void resolvedor_exaustivo (char senha[], int i, int vetor[], int *achei);
void poda_resolvedor (char senha_atual[], int vetor[], int usado[], int k,  int *achei ,int *letras, int *numeros);

/* Retorna 1 caso tenha encontrado a senha e 0 caso contrario */
int quebradorSenhaExaustivo(){
    char senha[6];
    /* variavel de controle */
    int *achei = malloc(sizeof(int));
    *achei = 0;
    cria_vetor_possibilidade(possibilidades);
    

    resolvedor_exaustivo(senha, 0, possibilidades, achei);
    if (*achei == 1){
        free(achei);
        return 1;
    } 
    free(achei);
    return 0;
}

/* Retorna 1 caso tenha encontrado a senha e 0 caso contrario */
int quebradorSenhaPoda(){
    /* 
    * Esse vetor_nulo sera usado para marcar as posicoes do
    * vetor "possibilidades" que ja foram usadas
    * dessa forma nao sera necessario a checagem "existe repeticao"
    */
    int *vetor_nulo = (int*)malloc(sizeof(int) * TAM);
    /* 
    * Utilizando o numero de letras e de numeros como ponteiros
    * podemos evitar de iterar pelo vetor contando a quantidade
    * de letras e numeros 
    */
    int *letras = (int*)malloc(sizeof(int));
    int *numeros = (int*)malloc(sizeof(int));
    *letras = 0;
    *numeros = 0;
    
    cria_vetor_possibilidade(possibilidades);
    cria_vetor_nulo(vetor_nulo, TAM);

    /* variavale de controle */
    int *achei = malloc(sizeof(int));
    *achei = 0;

    char senha[6];
    
    poda_resolvedor(senha, possibilidades, vetor_nulo, 0, achei, letras, numeros);
    free(letras);
    free(numeros);
    free(vetor_nulo);

    if (*achei == 1){
        free(achei);
        return 1;
    } 
    free(achei);
    return 0;
}

void cria_vetor_possibilidade (int *vetor){
    /* vetor que contem todos os caracteres possiveis */
    int i_vetor = 0;
    int i;
    
    /* caracteres[TAM] = "a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,0,1,2,3,4,5,6,7,8,9" */
    for (i = 97; i <= 122; i++){
        vetor[i_vetor] = i;
        i_vetor++;
    }

    for (i = 48; i <= 57; i++){
        vetor[i_vetor] = i;
        i_vetor++;
    }
    
}

void cria_vetor_nulo (int *vetor, int tam){
    int i;
    for (i = 0; i < tam; i ++){
        vetor[i] = 0;
    }
}

/* Retorna 0 caso a estrutura seja valida, 1 caso contrario */
int estrutura_valida (char vetor[], char elem, int tam, int n_letras, int n_numeros){

    if (tam == 5){
        /* significa que nao ha espaco livre para 2 numeros ou letras */
        if (n_letras == 5 || n_numeros == 5) return 1;
    }

    if (tam == 6){
        /* significa que a senha nao contem ao menos 2 letras ou numeros */
        if (n_letras < 2 || n_numeros < 2) return 1;
    }

    /* Se conseguimos passar por todos os testes */
    
    return 0;
}

/* Realiza um backtracking bruto, testando todas as combinacoes possiveis */
void resolvedor_exaustivo (char senha[], int i, int vetor[], int *achei){
	int v, j;

	if (i == 6) {
        /* tentando login */
        if (login(senha) == 1){
            *achei = 1;
        } 
		return;
	}
    
	for (v = 0; v < TAM; v++) {
        /* se ja achamos a senha */
        if (*achei == 1) return;

		senha[i] = vetor[v];
	    resolvedor_exaustivo(senha, i+1, vetor, achei);
	}
    return;
    
}

/* Realiza o backtracking otimizado */
void poda_resolvedor (char senha_atual[], int vetor[], int usado[], int k,  int *achei ,int *letras, int *numeros){
    int i;
    char c;

    /* Caso base */
    if (k == 6){
        if (login(senha_atual) == 1){
            *achei = 1;
        }   
        return;
    }

    for (i = 0; i < TAM; i++){
        /* Verifica se ja achamos a senha */
        if (*achei == 1) return;
        /* converte para caracter */
        c = vetor[i];
        senha_atual[k] = c;
        /* incrementa respectivamente:
        * o tamanho da senha, quantidade de letras ou de numeros
        */
        k = k + 1;
        if (vetor[i] >= 97) *letras = *letras + 1;
        else *numeros = *numeros + 1;

        /* Continua o caminho se a estrutura permanecer valida */
        if (estrutura_valida(senha_atual, c, k, *letras, *numeros) == 0 && usado[i] == 0){
            /* marca que a posicao ja esta sendo usada */
            usado[i] = 1;
            poda_resolvedor(senha_atual, vetor,usado, k, achei, letras, numeros);
            /* devolve a posicao */
            usado[i] = 0;
        }
        /* backtracking */
        /* Decrementa respectivamente:
         * o tamanho da senha, quantidade de letras ou numeros 
        */
        k = k - 1;
        if (vetor[i] >= 97) *letras = *letras - 1;
        else *numeros = *numeros - 1;
    }
    return;
    

}
