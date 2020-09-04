#ifndef MAINPULAMATRIZ_H
#define MAINPULAMATRIZ_H

#include<math.h>
#include<stdio.h>
#include "util.h"

void incluiMatriz(int linha, int coluna, char *entrada, int*** matriz){
    int lenght = 0;
    int barPos = -1;
    int signalFlag = 1;

    while(entrada[lenght] != '\0'){
        if(entrada[lenght] == '/'){
            barPos = lenght;
        }    
        lenght++;
    }

    if(barPos == -1){
        int j = 0;
        for(int i = lenght-1; i>=0; i--, j++){
            if(entrada[i] == '-'){
                signalFlag = signalFlag * -1;
                continue;
            }
            matriz[linha][coluna][0] +=  (entrada[i] - '0') * pow(10,j);
        }
    } else {
        int j = 0;
        for(int i = barPos-1; i>=0; i--, j++){
            if(entrada[i] == '-'){
                signalFlag = signalFlag * -1;
                continue;
            }
            matriz[linha][coluna][0] +=  (entrada[i] - '0') * pow(10,j);
        }
        j=0;
        matriz[linha][coluna][1] = 0;
        for(int i = lenght-1; i>barPos; i--, j++){
            if(entrada[i] == '-'){
                signalFlag = signalFlag * -1;
                continue;
            }
            matriz[linha][coluna][1] +=  (entrada[i] - '0') * pow(10,j);
        }
    }

    matriz[linha][coluna][0] = matriz[linha][coluna][0] * signalFlag;
}

void imprimeMatriz(int*** matriz, int dim){
    for(int i=0; i<dim; i++){
        for(int j=0; j<dim; j++){
            printf("%d",matriz[i][j][0]);
            if(matriz[i][j][1] != 1){
                printf("/%d\t",matriz[i][j][1]);
            } else {
                printf("\t");
            }
        }
        printf("\n");
    }
}

//Variavel fator define a operação, 1:soma    -1:subtração
int* operaMatriz(int numeradorA, int denominadorA, int numeradorB, int denominadorB, int fator){
    int *result;
    result = (int*) malloc(2 * sizeof(int));

    numeradorB = numeradorB * fator;

    result[1] = lcm(denominadorA, denominadorB);
    result[0] = ((result[1]/denominadorA) * numeradorA) + ((result[1]/denominadorB) * numeradorB);

    int mdc = gcd(result[0], result[1]);
    result[0] = result[0]/mdc;
    result[1] = result[1]/mdc;

    if(result[1] < 0){
        result[0] = result[0] * -1;
        result[1] = result[1] * -1;
    }

    return result;
}

#endif /*  MAINPULAMATRIZ_H  */