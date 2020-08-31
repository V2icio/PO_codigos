#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "manipulaMatriz.h"


void addArtificial(){

}

void addFolga(){
    
}


int main(){
    int nrVariaveis, nrRestricoes;
    char tipoObjetivo[5];
    char sAux[30];

    scanf("%d",&nrVariaveis);
    scanf("%d",&nrRestricoes);
    scanf("%s",tipoObjetivo);

    //Matriz q guarda os valores das restrições e da função.
    int ***matriz = (int ***)malloc((nrRestricoes+1) * sizeof(int**));
    for(int i = 0; i < (nrRestricoes+1); i++) {
        matriz[i] = (int **)malloc(nrVariaveis * sizeof(int*));
        for (int j = 0; j < nrVariaveis; j++){
            matriz[i][j] = (int*)malloc(2 * sizeof(int));
        }
    }

    //Matriz que guarda os valores dos sinais e do vetor B.
    int ***sinalVetorB = (int ***)malloc((nrRestricoes) * sizeof(int**));
    for(int i = 0; i < (nrRestricoes); i++) {
        sinalVetorB[i] = (int **)malloc(2 * sizeof(int*));
        for (int j = 0; j < 2; j++){
            sinalVetorB[i][j] = (int*)malloc(2 * sizeof(int));
        }
    }
    int nrFolga = 0;
    int nrArtificiais = 0;

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
                nrFolga++;
                break;
            case '>':
                sinalVetorB[i][0][0] = 1;
                nrFolga++;
                break;
            case '=':
                sinalVetorB[i][0][0] = 2;
                nrArtificiais++;
                break;
            default:
                break;
        }
        sinalVetorB[i][1][0] = 0;
        sinalVetorB[i][1][1] = 1;
        scanf("%s",sAux);
        incluiMatriz(i,1,sAux,sinalVetorB);

        if(sinalVetorB[i][0][0] != 2){
            if(sinalVetorB[i][0][0] == 0 && sinalVetorB[i][1][0] < 0){
                nrArtificiais++;
            }else if(sinalVetorB[i][0][0] == 1 && sinalVetorB[i][1][0] > 0){
                nrArtificiais++;
            }
        }
    }

    //Ver se a função é de max ou min
    if(tipoObjetivo[1] == 'a' && tipoObjetivo[2] == 'x'){
        //se for de máximo transformar em min
        for(int i=0; i<nrVariaveis; i++){
            matriz[nrRestricoes][i][0] *= -1;
        }
    }

    //Guarda os valores de folga.
    int ***matrizFolga = (int ***)malloc((nrRestricoes) * sizeof(int**));
    for(int i = 0; i < (nrRestricoes); i++) {
        matrizFolga[i] = (int **)malloc(nrFolga * sizeof(int*));
        for (int j = 0; j < nrFolga; j++){
            matrizFolga[i][j] = (int*)malloc(2 * sizeof(int));
        }
    }

    //if(nrArtificiais >0){
        //Guarda os valores de variaveis artificiais.
        int ***matrizArtificial = (int ***)malloc((nrRestricoes) * sizeof(int**));
        for(int i = 0; i < (nrRestricoes); i++) {
            matrizArtificial[i] = (int **)malloc(nrArtificiais * sizeof(int*));
            for (int j = 0; j < nrArtificiais; j++){
                matrizArtificial[i][j] = (int*)malloc(2 * sizeof(int));
            }
        }
    //}

    int contFolga = 0, contArtificial = 0;
    for(int i=0; i<nrRestricoes; i++){
        if(sinalVetorB[i][0][0] == 2){      //se for =
            //variavel artificial
            for(int j=0; j<nrRestricoes;j++){
                matrizArtificial[j][contArtificial][0] = j == i ? 1 : 0;
                matrizArtificial[j][contArtificial][1] = 1;   
            }
            contArtificial++;
        } else if(sinalVetorB[i][0][0] == 0){   //se for <=
            if(sinalVetorB[i][1][0] < 0){
                //multiplicar a linha por -1
                for(int j=0; j<nrVariaveis; j++){
                    matriz[i][j][0] *=-1;
                }
                sinalVetorB[i][0][0] = sinalVetorB[i][0][0] == 0 ? 1 : 0;
                sinalVetorB[i][1][0] *= -1;

                //variavel de folga
                for(int j=0; j<nrRestricoes;j++){
                    if(j == i){
                        matrizFolga[j][contFolga][0] = sinalVetorB[i][0][0] == 0 ? 1 : -1;
                    } else {
                        matrizFolga[j][contFolga][0] = 0;
                    }
                    matrizFolga[j][contFolga][1] = 1;
                }
                contFolga++;

                //variavel artificial
                for(int j=0; j<nrRestricoes;j++){
                    matrizArtificial[j][contArtificial][0] = j == i ? 1 : 0;
                    matrizArtificial[j][contArtificial][1] = 1;                
                }
                contArtificial++;
            }else{
                //variavel de folga
                for(int j=0; j<nrRestricoes;j++){
                    if(j == i){
                        matrizFolga[j][contFolga][0] = sinalVetorB[i][0][0] == 0 ? 1 : -1;
                    } else {
                        matrizFolga[j][contFolga][0] = 0;
                    }
                    matrizFolga[j][contFolga][1] = 1;
                }
                contFolga++;
            }
        } else if(sinalVetorB[i][0][0] == 1){   //se for >=
            if(sinalVetorB[i][1][0] <= 0){
                //multiplicar a linha por -1
                for(int j=0; j<nrVariaveis; j++){
                    matriz[i][j][0] *=-1;
                }
                sinalVetorB[i][0][0] = sinalVetorB[i][0][0] == 0 ? 1 : 0;
                sinalVetorB[i][1][0] *= -1;

                //variavel de folga
                for(int j=0; j<nrRestricoes;j++){
                    if(j == i){
                        matrizFolga[j][contFolga][0] = sinalVetorB[i][0][0] == 0 ? 1 : -1;
                    } else {
                        matrizFolga[j][contFolga][0] = 0;
                    }
                    matrizFolga[j][contFolga][1] = 1;
                }
                contFolga++;

            } else {
                //variavel de folga
                for(int j=0; j<nrRestricoes;j++){
                    if(j == i){
                        matrizFolga[j][contFolga][0] = sinalVetorB[i][0][0] == 0 ? 1 : -1;
                    } else {
                        matrizFolga[j][contFolga][0] = 0;
                    }
                    matrizFolga[j][contFolga][1] = 1;
                }
                contFolga++;

                //variavel artificial
                for(int j=0; j<nrRestricoes;j++){
                    matrizArtificial[j][contArtificial][0] = j == i ? 1 : 0;
                    matrizArtificial[j][contArtificial][1] = 1;
                    contArtificial++;                
                }
                contArtificial++;
            }
        }
    }
    //se tiver variaveis arificiais precisa fase 1
    //se b > 0 e sinal = '<'  ->  só folga
    //se b > 0 e sinal = '>'  ->  folga + artificial
    //se b < 0 e sinal = '<'  ->  multiplicar por -1 + folga + artificial
    //se b < 0 e sinal = '>'  ->  multiplicar por -1 + folga
    //sinal = '='  ->  artificial

    // Eu preciso saber quem são as arificiais
    // Eu preciso setar o valor de todas como 0 e artificiais como 1 e 
    // no fim da fase 1 voltar o valor ao normal




    //fase I
    //fase II

    printf("\n\nMatriz:\t\tsinal e B\tFolga\t\tArtificial\n");
    for(int i=0; i<=nrRestricoes; i++){
        for(int j=0; j<nrVariaveis; j++){
            printf("%d  ", matriz[i][j][0]);
        }
        if(i != nrRestricoes){
            printf("\t\t");
            for(int j=0; j<2; j++){
                printf("%d  ", sinalVetorB[i][j][0]);
            }
            printf("\t\t");
            for(int j=0; j<nrFolga; j++){
                printf("%d  ", matrizFolga[i][j][0]);
            }
            printf("\t\t");
            for(int j=0; j<nrArtificiais; j++){
                printf("%d  ", matrizArtificial[i][j][0]);
            }
        }
        printf("\n");
    }

    printf("\n\n NrArtificiais = %d\n\n", nrArtificiais);

    return 0;
}