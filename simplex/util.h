#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>

int gcd(int a, int b){      // Algoritmo de Euclides - maximo(maior) divisor comum
    if (b == 0) 
        return a; 
    return gcd(b, a % b);  
}

int lcm(int a, int b){      // Minimo multiplo comum
    return a * (b / gcd(a, b));
}

void liberaMatrizDim2(int **matriz, int linhas, int colunas){
    for (int i = 0; i < linhas; i++){
        free(matriz[i]);
    }
    free(matriz);
}

void liberaMatrizDim3(int ***matriz, int linhas, int colunas, int largura){
    for (int i = 0; i < linhas; i++){
        for (int j = 0; j < colunas; j++){
            free(matriz[i][j]);
        }
        free(matriz[i]);
    }
    free(matriz);
}

#endif /* UTIL_H */