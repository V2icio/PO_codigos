#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int DIMENSION;

void incluiMatriz(int linha, int coluna, char *entrada, int matriz[DIMENSION][DIMENSION][2]){
    int lenght = 0;
    int barPos = -1;

    while(entrada[lenght] != '\0'){
        if(entrada[lenght] == '/'){
            barPos = lenght;
        }        
        lenght++;
    }

    if(barPos == -1){
        int j = 0;
        for(int i = lenght-1; i>=0; i--, j++){
            matriz[linha][coluna][0] +=  (entrada[i] - '0') * pow(10,j);
        }
    } else {
        int j = 0;
        for(int i = barPos-1; i>=0; i--, j++){
            matriz[linha][coluna][0] +=  (entrada[i] - '0') * pow(10,j);
        }
        j=0;
        matriz[linha][coluna][1] = 0;
        for(int i = lenght-1; i>barPos; i--, j++){
            matriz[linha][coluna][1] +=  (entrada[i] - '0') * pow(10,j);
        }
    }
}


void imprimeMatriz(int matriz[DIMENSION][DIMENSION][2], int dim){
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


int gcd(int a, int b){      // Algoritmo de Euclides - maximo(maior) divisor comum
    if (b == 0) 
        return a; 
    return gcd(b, a % b);  
}

int lcm(int a, int b){      // Minimo multiplo comum
    return a * (b / gcd(a, b));
}


int* operaMatriz(int numeradorA, int denominadorA, int numeradorB, int denominadorB, int fator){
    //Variavel fator define a operação, 1:soma    -1:subtração
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

int* regraDeSarrus(int dim, int matriz[dim][dim][2]){
    int produto1[3][2];
    int produto2[3][2];

    for(int i=0; i<dim; i++){
        for(int j=0; j<2; j++){
            produto1[i][j] = 1;
            produto2[i][j] = 1;
        }
    }

    for(int i=0; i<dim; i++){
        for(int j=0;j<dim; j++)
        {
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
        for(int j=0; j<dim; j++)
        {
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
/*
    for(int i=0; i<dim; i++){
        printf("%d/%d\t",produto1[i][0],produto1[i][1]);        
    }
    printf("\n\n");
    for(int i=0; i<dim; i++){
        printf("%d/%d\t",produto2[i][0],produto2[i][1]);        
    }

    printf("\n\n\n");
*/
    int *result1, *result2;
    result1 = operaMatriz(produto1[0][0],produto1[0][1], produto1[1][0], produto1[1][1], 1);
    result1 = operaMatriz(result1[0], result1[1], produto1[2][0], produto1[2][1], 1);

    result2 = operaMatriz(produto2[0][0],produto2[0][1], produto2[1][0], produto2[1][1], 1);
    result2 = operaMatriz(result2[0], result2[1], produto2[2][0], produto2[2][1], 1);
    
    //printf("result1:   %d/%d\n", result1[0], result1[1]);
    //printf("result2:   %d/%d\n\n", result2[0], result2[1]);

    int *determinate;

    determinate = operaMatriz(result1[0], result1[1], result2[0], result2[1], -1);
    
    free(result1);
    free(result2);

    return determinate;
}

int* calculaDeterminante(int dim, int matriz[dim][dim][2]){
    if(dim > 3){
        printf("La place não implementado\n");
        return NULL;
    } else {
        return regraDeSarrus(dim,matriz);
    }
}

void calculaInversa(int dim, int matriz[dim][dim][2]){
    //alocar as duas matrizes, copiar na matriz esquerda a matriz original e na direita a identidade
    int ***matrizEsquerda = (int ***)malloc(dim * sizeof(int**));
    for(int i = 0; i < dim; i++) {
        matrizEsquerda[i] = (int **)malloc(dim * sizeof(int*));
        for (int j = 0; j < dim; j++)
        {
            matrizEsquerda[i][j] = (int*)malloc(2 * sizeof(int));
        }
    }
        
/*
    int **matrizDireita = (int **)malloc(dim * sizeof(int*));
    for(int i = 0; i < dim; i++) 
        matrizDireita[i] = (int *)malloc(dim * sizeof(int));
*/
    for(int i = 0; i < dim; i++){
        for (int j = 0; j < dim; j++){
            matrizEsquerda[i][j] = matriz[i][j];
            //matrizDireita[i][j] = i == j ? 1 : 0;
        }
    }
    printf("\n\n");
    imprimeMatriz(matrizEsquerda, dim);
    printf("\n\n");
    //imprimeMatriz(matrizDireita,dim);
    printf("\n\n");
}

int main() {

    int dim;
    printf("Entre com a dimensao da matriz quadrada\n");
    scanf("%d",&dim);
    printf("dimensao: %d",dim);
    DIMENSION = dim;
    int matriz[dim][dim][2];

    printf("\n\nEntre com a matriz\n");

    char sAux[30];
    for(int i=0; i<dim; i++){
        for(int j=0; j<dim; j++){
            matriz[i][j][0] = 0;
            matriz[i][j][1] = 1;
            scanf("%s",&sAux);
            incluiMatriz(i, j, sAux, matriz);
        }
    }
    printf("matriz:\n");
    imprimeMatriz(matriz, dim);

    int *determinante;
    determinante = calculaDeterminante(dim,matriz);

    printf("\nO determinante da matriz e: %d/%d", determinante[0], determinante[1]);

    if(determinante[0] != 0){
        
    } else {
        printf("Como o determinante = 0, a matriz nao possui inversa\n");
    }


    
    free(determinante);
    return 0;
;}