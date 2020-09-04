#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "util.h"
#include "manipulaMatriz.h"


int* teoremaLaplace(int , int*** );

int* regraDeSarrus(int dim, int*** matriz){
    int produto1[3][2];
    int produto2[3][2];

    for(int i=0; i<dim; i++){
        for(int j=0; j<2; j++){
            produto1[i][j] = 1;
            produto2[i][j] = 1;
        }
    }

    for(int i=0; i<dim; i++){
        for(int j=0;j<dim; j++){
            int auxI, auxJ;
            auxI = j >=3 ? j-3 : j;
            auxJ = i+j >=3 ? i+j-3 : i + j;
            produto1[i][0] =  produto1[i][0] * matriz[auxI][auxJ][0];
            produto1[i][1] =  produto1[i][1] * matriz[auxI][auxJ][1];

            int maiorDivisor;
            maiorDivisor = gcd(produto1[i][0],produto1[i][1]);

            produto1[i][0] = produto1[i][0]/maiorDivisor;
            produto1[i][1] = produto1[i][1]/maiorDivisor;             
        }
    }

    for(int i=dim-1; i>=0; i--){
        for(int j=0; j<dim; j++){
            int auxI, auxJ;
            auxI = j >=3 ? j-3 : j;
            auxJ = (2 - j + 2 - i) >=3 ? (2 - j + 2 - i)-3 :(2 - j + 2 - i);
            produto2[i][0] =  produto2[i][0] * matriz[auxI][auxJ][0];
            produto2[i][1] =  produto2[i][1] * matriz[auxI][auxJ][1];

            int maiorDivisor;
            maiorDivisor = gcd(produto2[i][0],produto2[i][1]);

            produto2[i][0] = produto2[i][0]/maiorDivisor;
            produto2[i][1] = produto2[i][1]/maiorDivisor;           
        }
    }

    int *result1, *result2;
    result1 = operaMatriz(produto1[0][0],produto1[0][1], produto1[1][0], produto1[1][1], 1);
    result1 = operaMatriz(result1[0], result1[1], produto1[2][0], produto1[2][1], 1);

    result2 = operaMatriz(produto2[0][0],produto2[0][1], produto2[1][0], produto2[1][1], 1);
    result2 = operaMatriz(result2[0], result2[1], produto2[2][0], produto2[2][1], 1);

    int *determinate;
    determinate = operaMatriz(result1[0], result1[1], result2[0], result2[1], -1);
    
    free(result1);
    free(result2);

    return determinate;
}

int* regraDeSarrusMenor(int dim, int*** matriz){
    int aux1[2];
    int aux2[2];
    
    aux1[0] = matriz[0][0][0] * matriz[1][1][0];
    aux1[1] = matriz[0][0][1] * matriz[1][1][1];

    aux2[0] = matriz[1][0][0] * matriz[0][1][0];
    aux2[1] = matriz[1][0][1] * matriz[0][1][1];

    return operaMatriz(aux1[0], aux1[1], aux2[0], aux2[1], -1);
}

int* calculaDeterminante(int dim, int*** matriz){
    if(dim > 3){
        return teoremaLaplace(dim, matriz);
    } else  if(dim == 3){
        return regraDeSarrus(dim,matriz);
    } else if(dim == 2){
        return regraDeSarrusMenor(dim, matriz);
    } else {
        return operaMatriz(matriz[0][0][0],matriz[0][0][1],0,1,1);
    }
}

int* teoremaLaplace(int dim, int*** matriz){
    /*cofator = determinante da matriz de ordem n-1 eliminando 
    a linha e a coluna do elemento selecionado*/
    int *determinante;
    determinante = (int*) malloc(2 * sizeof(int));
    determinante[0] = 0;
    determinante[1] = 1;

    //para cada elemento da primeira linha.
    for(int i = 0; i<dim; i++){
        //matrizAux = matriz ordem n -1.
        int ***matrizAux = (int ***)malloc((dim-1) * sizeof(int**));
        for(int j = 0; j < dim-1; j++) {
            matrizAux[j] = (int **)malloc((dim-1) * sizeof(int*));
            for (int k = 0; k < dim-1; k++)
            {
                matrizAux[j][k] = (int*)malloc(2 * sizeof(int));
            }
        }
        //copiar os valores sem a linha 0 e coluna i.
        for(int j = 0, k=0; j<dim; j++, k++){
            if(j == i){
                j++;
                if(j==dim){
                    break;
                }
            }
            for(int l=1; l<dim; l++){
                matrizAux[l-1][k][0] = matriz[l][j][0];
                matrizAux[l-1][k][1] = matriz[l][j][1];
            }
        }

        //determinante = determinante + elemento(0,i)*(-1)^(i+1+j+1) * det(matrizAux)
        int *a;
        a = calculaDeterminante(dim-1, matrizAux);

        a[0] = a[0] * pow(-1,i+1+1);
        
        a[0] = a[0] * matriz[0][i][0];
        a[1] = a[1] * matriz[0][i][1];

        int *liberar;
        liberar = determinante;
        determinante = operaMatriz(determinante[0],determinante[1], a[0], a[1], 1);

        free(liberar);
        free(matrizAux);  
   }
   return determinante;
}