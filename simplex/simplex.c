#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "manipulaMatriz.h"


int main(){
    int nrVariaveis, nrRestricoes;
    char tipoObjetivo[5];
    char sAux[30];

    scanf("%d",&nrVariaveis);
    scanf("%d",&nrRestricoes);
    scanf("%s",tipoObjetivo);

    int ***matriz = (int ***)malloc((nrRestricoes+1) * sizeof(int**));
    for(int i = 0; i < (nrRestricoes+1); i++) {
        matriz[i] = (int **)malloc(nrVariaveis * sizeof(int*));
        for (int j = 0; j < nrVariaveis; j++){
            matriz[i][j] = (int*)malloc(2 * sizeof(int));
        }
    }

    int ***sinalVetorB = (int ***)malloc((nrRestricoes) * sizeof(int**));
    for(int i = 0; i < (nrRestricoes); i++) {
        sinalVetorB[i] = (int **)malloc(2 * sizeof(int*));
        for (int j = 0; j < 2; j++){
            sinalVetorB[i][j] = (int*)malloc(2 * sizeof(int));
        }
    }

    for(int i=0; i<nrVariaveis; i++){
        matriz[nrRestricoes][i][0] = 0;
        matriz[nrRestricoes][i][1] = 1;
        scanf("%s",sAux);
        incluiMatriz(nrRestricoes,i,sAux,matriz);
    }
    
    for(int i=0; i<nrRestricoes; i++){
        for(int j=0; j<nrVariaveis; j++){
            matriz[i][j][0] = 0;
            matriz[i][j][1] = 1;
            scanf("%s",sAux);
            incluiMatriz(i,j,sAux,matriz);
        }
        scanf("%s",sAux);
        switch (sAux[0]){
            case '<':
                sinalVetorB[i][0][0] = 0;
                break;
            case '>':
                sinalVetorB[i][0][0] = 1;
                break;
            case '=':
                sinalVetorB[i][0][0] = 2;
                break;
            default:
                break;
        }
        sinalVetorB[i][1][0] = 0;
        sinalVetorB[i][1][1] = 1;
        scanf("%s",sAux);
        incluiMatriz(i,1,sAux,sinalVetorB);
    }


    //Ver se a função é de max ou min
    if(tipoObjetivo[1] == 'a' && tipoObjetivo[2] == 'x'){
        //se for de máximo transformar em min
        for(int i=0; i<nrVariaveis; i++){
            matriz[nrRestricoes][i][0] *= -1;
        }
    }

    //verificar a necessidade de fase I
    //se tiver variaveis arificiais precisa fase 1
    //se b > 0 e sinal = '<'  ->  só folga
    //se b > 0 e sinal = '>'  ->  folga + artificial
    //se b < 0 e sinal = '<'  ->  multiplicar por -1 + folga + artificial
    //se b < 0 e sinal = '>'  ->  multiplicar por -1

    // Eu preciso saber quem são as arificiais
    // Eu preciso setar o valor de todas como 0 e artificiais como 1 e 
    // no fim da fase 1 voltar o valor ao normal




    //fase I
    //fase II

    printf("\n\nMatriz:\n");
    for(int i=0; i<=nrRestricoes; i++){
        for(int j=0; j<nrVariaveis; j++){
            printf("%d  ", matriz[i][j][0]);
        }
        printf("\n");
    }





    



    return 0;
}