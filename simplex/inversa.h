#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "determinante.h"

int*** calculaInversa(int dim, int*** matriz){
    //alocar as duas matrizes, copiar na matriz esquerda a matriz original e na direita a identidade
    int ***matrizEsquerda = (int ***)malloc(dim * sizeof(int**));
    for(int i = 0; i < dim; i++) {
        matrizEsquerda[i] = (int **)malloc(dim * sizeof(int*));
        for (int j = 0; j < dim; j++)
        {
            matrizEsquerda[i][j] = (int*)malloc(2 * sizeof(int));
        }
    }
        
    int ***matrizDireita = (int ***)malloc(dim * sizeof(int**));
    for(int i = 0; i < dim; i++) {
        matrizDireita[i] = (int **)malloc(dim * sizeof(int*));
        for (int j = 0; j < dim; j++)
        {
            matrizDireita[i][j] = (int*)malloc(2 * sizeof(int));
        }
    }
    
    for(int i = 0; i < dim; i++){
        for (int j = 0; j < dim; j++){
            matrizEsquerda[i][j][0] = matriz[i][j][0];
            matrizEsquerda[i][j][1] = matriz[i][j][1];
            matrizDireita[i][j][0] = i == j ? 1 : 0;
            matrizDireita[i][j][1] = 1;
        }
    }

    for (int i = 0; i < dim; i++){
        //posição 0 = númerador  --  posição 1 = denominador

        //Se o elemento pivo for 0, trocar essa linha com outra linha.
        if(matrizEsquerda[i][i][0] == 0){
            for(int j=i+1; j<dim; j++){
                if(matrizEsquerda[j][i][0] != 0){
                    //troca as linhas
                    int aux;
                    for(int k = 0; k<dim; k++){
                        aux = matrizEsquerda[j][k][0];
                        matrizEsquerda[j][k][0] = matrizEsquerda[i][k][0];
                        matrizEsquerda[i][k][0] = aux;

                        aux = matrizEsquerda[j][k][1];
                        matrizEsquerda[j][k][1] = matrizEsquerda[i][k][1];
                        matrizEsquerda[i][k][1] = aux;

                        aux = matrizDireita[j][k][0];
                        matrizDireita[j][k][0] = matrizDireita[i][k][0];
                        matrizDireita[i][k][0] = aux;

                        aux = matrizDireita[j][k][1];
                        matrizDireita[j][k][1] = matrizDireita[i][k][1];
                        matrizDireita[i][k][1] = aux;
                    }
                    break;
                }
                if(j == dim-1){
                    return NULL;
                    printf("Erro nos pivos: pivo != 0 não encontrado.");
                }
            }
        }

        //divido a primeira linha pra deixar com valor 1
        int fator[2];
        fator[0] = matrizEsquerda[i][i][0] == 0 ? 1 : matrizEsquerda[i][i][0]; 
        fator[1] = matrizEsquerda[i][i][1] == 0 ? 1 : matrizEsquerda[i][i][1];

        for (int j = 0; j < dim; j++){
            int mdc;
            if(j>=i){
                matrizEsquerda[i][j][0] = matrizEsquerda[i][j][0] * fator[1];
                matrizEsquerda[i][j][1] = matrizEsquerda[i][j][1] * fator[0];
                
                mdc = gcd(matrizEsquerda[i][j][0], matrizEsquerda[i][j][1]);
                matrizEsquerda[i][j][0] = matrizEsquerda[i][j][0]/mdc;
                matrizEsquerda[i][j][1] = matrizEsquerda[i][j][1]/mdc;
                
                //denominador negativo multiplica por -1 a fração.
                if(matrizEsquerda[i][j][1] < 0){ 
                    matrizEsquerda[i][j][0] = matrizEsquerda[i][j][0] * -1;
                    matrizEsquerda[i][j][1] = matrizEsquerda[i][j][1] * -1;
                }
            }
            matrizDireita[i][j][0] = matrizDireita[i][j][0] * fator[1];
            matrizDireita[i][j][1] = matrizDireita[i][j][1] * fator[0];

            mdc = gcd(matrizDireita[i][j][0], matrizDireita[i][j][1]);
            matrizDireita[i][j][0] = matrizDireita[i][j][0]/mdc;
            matrizDireita[i][j][1] = matrizDireita[i][j][1]/mdc;
            if(matrizDireita[i][j][1] < 0){
                matrizDireita[i][j][0] = matrizDireita[i][j][0] * -1;
                matrizDireita[i][j][1] = matrizDireita[i][j][1] * -1;
            }
        }
        
        for (int j = i+1; j < dim; j++){
            //zerar as outras linhas abaixo dessa
            //j = linha
            fator[0] = matrizEsquerda[j][i][0]; 
            fator[1] = matrizEsquerda[j][i][1];
            for (int k = 0; k < dim; k++)
            {  
                //k = coluna
                int *result;
                if(k>=i){
                    result = operaMatriz(matrizEsquerda[j][k][0], matrizEsquerda[j][k][1],
                    fator[0]*matrizEsquerda[i][k][0], fator[1]*matrizEsquerda[i][k][1], -1);

                    matrizEsquerda[j][k][0] = result[0];
                    matrizEsquerda[j][k][1] = result[1];

                    free(result);
                }
                
                result = operaMatriz(matrizDireita[j][k][0], matrizDireita[j][k][1],
                fator[0]*matrizDireita[i][k][0], fator[1]*matrizDireita[i][k][1], -1);
                
                matrizDireita[j][k][0] = result[0];
                matrizDireita[j][k][1] = result[1];
                
                free(result);
            }
        }  
    }

    for(int i = dim-1; i>=0; i--){
        int fator[2];
        
        int *result;
        
        for(int j = i-1; j>=0; j--){
            fator[0] = matrizEsquerda[j][i][0];
            fator[1] = matrizEsquerda[j][i][1];

            for (int k = i; k < dim; k++){
                result = operaMatriz(matrizEsquerda[j][k][0],matrizEsquerda[j][k][1], 
                fator[0]*matrizEsquerda[i][k][0], fator[1]*matrizEsquerda[i][k][1], -1);

                matrizEsquerda[j][k][0] = result[0];
                matrizEsquerda[j][k][1] = result[1];
                free(result);
            }

            for (int k = 0; k < dim; k++){
                result = operaMatriz(matrizDireita[j][k][0],matrizDireita[j][k][1], 
                fator[0]*matrizDireita[i][k][0], fator[1]*matrizDireita[i][k][1], -1);

                matrizDireita[j][k][0] = result[0];
                matrizDireita[j][k][1] = result[1];
                free(result);
            }
        }
    }

    free(matrizEsquerda);
    return matrizDireita;
}

int*** inversa(int dim, int*** matriz){
    int *determinante;
    determinante = calculaDeterminante(dim,matriz);

    int ***matrizInversa;
    if(determinante[0] != 0){
        matrizInversa = calculaInversa(dim, matriz);
        free(determinante);
        return matrizInversa;
    } else {
        free(determinante);
        return NULL;
    }
}

