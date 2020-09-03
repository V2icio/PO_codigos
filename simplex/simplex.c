#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "manipulaMatriz.h"
#include "inversa.h"


void multiplicaLinha(int i, int*** matriz, int*** sinalVetorB, int nrVariaveis){
    for(int j=0; j<nrVariaveis; j++){
        matriz[i][j][0] *=-1;
    }
    sinalVetorB[i][0][0] = sinalVetorB[i][0][0] == 0 ? 1 : 0;
    sinalVetorB[i][1][0] *= -1;
}

void addFolgaArtificial(int i, int j, int valor, int*** matriz, int nrRestricoes){
    for(int k=0; k<nrRestricoes; k++){
        matriz[k][j][0] = k == i ? valor : 0;
        matriz[k][j][1] = 1;   
    }    
}
void preTratamentoSimplex(int*** matriz, int*** sinalVetorB, int*** matrizFolga, 
                          int*** matrizArtificial, int nrRestricoes, int nrVariaveis){
    int contFolga = 0, contArtificial = 0;
    int aux;
    for(int i=0; i<nrRestricoes; i++){
        if(sinalVetorB[i][0][0] == 2){      //se for =
            //variavel artificial
            addFolgaArtificial(i, contArtificial, 1, matrizArtificial, nrRestricoes);
            contArtificial++;
        } else if(sinalVetorB[i][0][0] == 0){   //se for <=
            if(sinalVetorB[i][1][0] < 0){
                //multiplicar a linha por -1
                multiplicaLinha(i, matriz, sinalVetorB, nrVariaveis);

                //variavel de folga
                aux = sinalVetorB[i][0][0] == 0 ? 1 : -1;
                addFolgaArtificial(i, contFolga, aux, matrizFolga, nrRestricoes);
                contFolga++;

                //variavel artificial
                addFolgaArtificial(i, contArtificial, 1,matrizArtificial, nrRestricoes);
                contArtificial++;
            }else{
                //variavel de folga
                aux = sinalVetorB[i][0][0] == 0 ? 1 : -1;
                addFolgaArtificial(i, contFolga, aux, matrizFolga, nrRestricoes);
                contFolga++;
            }
        } else if(sinalVetorB[i][0][0] == 1){   //se for >=
            if(sinalVetorB[i][1][0] <= 0){
                //multiplicar a linha por -1
                multiplicaLinha(i, matriz, sinalVetorB,nrVariaveis);

                //variavel de folga
                aux = sinalVetorB[i][0][0] == 0 ? 1 : -1;
                addFolgaArtificial(i, contFolga, aux, matrizFolga, nrRestricoes);
                contFolga++;
            } else {
                //variavel de folga
                aux = sinalVetorB[i][0][0] == 0 ? 1 : -1;
                addFolgaArtificial(i, contFolga, aux, matrizFolga, nrRestricoes);
                contFolga++;

                //variavel artificial
                addFolgaArtificial(i, contArtificial, 1, matrizArtificial, nrRestricoes);
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
}


void inicializaBase(int *basicas, int *naoBasicas, int nrVariaveis, 
                    int nrFolga, int nrArtificiais, int*** matrizFolga, int*** matrizArtificial){

    int contBasicas = 0, contNaoBasicas = 0, contArtificial = 0;

    for (int i = 0; i < nrVariaveis; i++){
        naoBasicas[contNaoBasicas] = i;
        contNaoBasicas++;  
    }

    for(int i = 0, j = 0; i< nrFolga; i++, j++){
        if(matrizFolga[j][i][0] == 1){
            basicas[contBasicas] = nrVariaveis + i;
            contBasicas++;
        } else if(matrizFolga[j][i][0] == -1){
            naoBasicas[contNaoBasicas] = nrVariaveis + i;
            naoBasicas++;

            basicas[contBasicas] = contArtificial + nrVariaveis + nrFolga;
            contArtificial++;
            contBasicas++;
        } else{
            basicas[contBasicas] = contArtificial + nrVariaveis + nrFolga;
            contArtificial++;
            contBasicas++;
            i--;
        }
    }

    for(int i = contArtificial; i < nrArtificiais; i++){
        basicas[contBasicas] = contArtificial + nrVariaveis + nrFolga;
        contArtificial++;
        contBasicas++;
    }
}


int faseI_II(int *basicas, int *naoBasicas, int*** matriz, int*** sinalVetorB, int*** matrizFolga, 
           int*** matrizArtificial, int nrVariaveis, int nrRestricoes, int nrFolga, int nrArtificiais){

    int flagFase;

    if(nrArtificiais > 0){
        flagFase = 1;
    } else{
        flagFase = 2;
    }


    //loop começa aqui.

    while (1){    
        
        int ***matrizBasica = (int ***)malloc(nrRestricoes * sizeof(int**));
        for(int i = 0; i < nrRestricoes; i++) {
            matrizBasica[i] = (int **)malloc(nrRestricoes * sizeof(int*));
            for (int j = 0; j < nrRestricoes; j++){
                matrizBasica[i][j] = (int*)malloc(2 * sizeof(int));
            }
        }

        int *aux2;
        int aux[2];

        for(int i=0; i<nrRestricoes; i++){
            for(int j=0; j<nrRestricoes; j++){
                if(basicas[i] >= nrVariaveis + nrFolga){
                    //procurar o valor nas variaveis artificiais
                    matrizBasica[j][i][0] = matrizArtificial[j][basicas[i] - nrVariaveis - nrFolga][0];
                    matrizBasica[j][i][1] = matrizArtificial[j][basicas[i] - nrVariaveis - nrFolga][1];
                } else if(basicas[i] >= nrVariaveis){
                    //procurar o valor nas variaveis básicas
                    matrizBasica[j][i][0] = matrizFolga[j][basicas[i]-nrVariaveis][0];
                    matrizBasica[j][i][1] = matrizFolga[j][basicas[i]-nrVariaveis][1];
                } else {
                    matrizBasica[j][i][0] = matriz[j][basicas[i]][0];
                    matrizBasica[j][i][1] = matriz[j][basicas[i]][1];
                }
            }
        }

        int ***matrizBasicaInversa = inversa(nrRestricoes, matrizBasica);

        printf("Matriz basica inversa:\n\n");

        imprimeMatriz(matrizBasicaInversa, nrRestricoes);

        //Calculo do X_B = B^-1 * b
        int **X_B = (int**) malloc(nrRestricoes*sizeof(int));
        for (int i = 0; i < nrRestricoes; i++){
            X_B[i] = (int*)malloc(2 * sizeof(int));
        }

        for(int i=0 ; i<nrRestricoes; i++){
            X_B[i][0] = 0;
            X_B[i][1] = 1;
            for(int j=0; j<nrRestricoes; j++){
                aux[0] = matrizBasicaInversa[i][j][0] * sinalVetorB[j][1][0];
                aux[1] = matrizBasicaInversa[i][j][1] * sinalVetorB[j][1][1];

                aux2 = operaMatriz(aux[0],aux[1],X_B[i][0],X_B[i][1],1);

                X_B[i][0] = aux[0];
                X_B[i][1] = aux[1];
            }
        }

        //Calculo do lambda^T = C_b * B^-1
        int **lambdaT = (int**) malloc(nrRestricoes*sizeof(int));
        for (int i = 0; i < nrRestricoes; i++){
            lambdaT[i] = (int*)malloc(2 * sizeof(int));
        }

        int **C_b = (int**) malloc(nrRestricoes*sizeof(int));
        for (int i = 0; i < nrRestricoes; i++){
            C_b[i] = (int*)malloc(2 * sizeof(int));
        }

        for(int i=0; i< nrRestricoes; i++){
            if(basicas[i] >= nrVariaveis + nrFolga){
                //valor = 1 na fase I e valor = 0 na fase II
                C_b[i][0] = 1;
                C_b[i][1] = 1;
            }else if(basicas[i] >= nrVariaveis){
                //valor = 0 sempre.
                C_b[i][0] = 0;
                C_b[i][1] = 1;
            } else{
                // valor = 0 na fase I e valor = C_i na fase II.
                C_b[i][0] = 0;
                C_b[i][1] = 1;
                //C_b[i] = matriz[nrVariaveis][basicas[i]];
            }
        }

        for(int i=0; i<nrRestricoes; i++){
            lambdaT[i][0] = 0;
            lambdaT[i][1] = 1;
            for (int j = 0; j < nrRestricoes; j++){
                aux[0] = C_b[j][0] * matrizBasicaInversa[i][j][0];
                aux[1] = C_b[j][1] * matrizBasicaInversa[i][j][1];

                aux2 = operaMatriz(aux[0],aux[1],lambdaT[i][0],lambdaT[i][1],1);

                lambdaT[i][0] = aux2[0];
                lambdaT[i][1] = aux2[1];
            }
        }

        //Pegar os custos das variaveis não basicas
        int **C_nb = (int**) malloc((nrArtificiais + nrFolga) *sizeof(int));
        for (int i = 0; i < (nrArtificiais + nrFolga); i++){
            C_nb[i] = (int*)malloc(2 * sizeof(int));
        }

        for(int i=0; i< (nrArtificiais + nrFolga); i++){
            if(naoBasicas[i] >= nrVariaveis + nrFolga){
                //valor = 1 na fase I e valor = 0 na fase II
                C_nb[i][0] = 1;
                C_nb[i][1] = 1;
            }else if(basicas[i] >= nrVariaveis){
                //valor = 0 sempre.
                C_nb[i][0] = 0;
                C_nb[i][1] = 1;
            } else{
                // valor = 0 na fase I e valor = C_i na fase II.
                if(flagFase == 1){
                    C_nb[i][0] = 0;
                    C_nb[i][1] = 1;
                } else {
                    C_nb[i][0] = matriz[nrVariaveis][basicas[i]][0];
                    C_nb[i][1] = matriz[nrVariaveis][basicas[i]][1];
                }
                
                //C_b[i] = matriz[nrVariaveis][basicas[i]];
            }
        }

        //calcular o custo relativo das variaveis não basicas
        int **custosRelativos = (int**) malloc((nrArtificiais + nrFolga) *sizeof(int));
        for (int i = 0; i < (nrArtificiais + nrFolga); i++){
            custosRelativos[i] = (int*)malloc(2 * sizeof(int));
        }

        int posiMenorCustoRelativo;
        double menorCustoRelativo = INFINITY;

        for(int i=0; i < (nrArtificiais + nrVariaveis); i++){
            if(naoBasicas[i] >= (nrVariaveis + nrFolga) && flagFase == 2){
                //Não considerar as variaveis artificiais, se existirem.
                continue;
            }

            //Custo relativo_i = custo_i - lambdaT*coluna
            int **colunaAux = (int**) malloc(nrRestricoes *sizeof(int));
            for (int j = 0; j < nrRestricoes; j++){
                colunaAux[j] = (int*)malloc(2 * sizeof(int));
            }

            for (int j = 0; j < nrRestricoes; j++){
                if(naoBasicas[i] >= nrVariaveis + nrFolga){
                    //procurar o valor nas variaveis artificiais
                    colunaAux[j][0] = matrizArtificial[j][naoBasicas[i] - nrVariaveis - nrFolga][0];
                    colunaAux[j][1] = matrizArtificial[j][naoBasicas[i] - nrVariaveis - nrFolga][1];
                } else if(naoBasicas[i] >= nrVariaveis){
                    //procurar o valor nas variaveis básicas   
                    colunaAux[j][0] = matrizFolga[j][naoBasicas[i] - nrVariaveis][0];
                    colunaAux[j][1] = matrizFolga[j][naoBasicas[i] - nrVariaveis][1];
                } else {
                    colunaAux[j][0] = matriz[j][naoBasicas[i]][0];
                    colunaAux[j][1] = matriz[j][naoBasicas[i]][1];
                }            
            }

            custosRelativos[i][0] = 0;
            custosRelativos[i][1] = 1;

            for (int j = 0; j < nrRestricoes; j++){
                aux[0] = lambdaT[j][0]*colunaAux[j][0];
                aux[1] = lambdaT[j][1]*colunaAux[j][1];
                aux2 = operaMatriz(aux[0],aux[1], custosRelativos[i][0], custosRelativos[i][1], 1);
                custosRelativos[i][0] = aux2[0];
                custosRelativos[i][1] = aux2[1];
            }
            aux2 = operaMatriz(C_nb[i][0], C_nb[i][1], custosRelativos[i][0], custosRelativos[i][1], -1);      
            custosRelativos[i][0] = aux2[0];
            custosRelativos[i][1] = aux2[1];

            if(menorCustoRelativo > (custosRelativos[i][0]/custosRelativos[i][1])){
                menorCustoRelativo = custosRelativos[i][0]/custosRelativos[i][1];
                posiMenorCustoRelativo = i;
            }

            free(colunaAux);
        }
        printf("Custos relativos:   ");
        for(int i=0; i<(nrArtificiais + nrVariaveis); i++){
            printf("%d  ",custosRelativos[i][0]);
        }

        if(menorCustoRelativo > 0){
            if(flagFase == 2){
                printf("solucao ótima");
                return 1;
            } else {
                //se tiver na fase I e não tiver variaveis artificiais na base -> fase II
                //se não problema infactivel
                for(int i=0; i<nrRestricoes; i++){
                    if(basicas[i] >= nrVariaveis + nrFolga){
                        printf("problema sem solução");
                        return -1;
                    }
                }
                flagFase = 2;
                printf("\n-----   FASE-II   -----\n");
            }
        }

        //calcular o Y = B^-1 * a(posiMenorCustoRelativo) = direção simplex
        int **direcaoSimplex = (int**) malloc(nrRestricoes *sizeof(int));
        for (int i = 0; i < nrRestricoes; i++){
            direcaoSimplex[i] = (int*)malloc(2 * sizeof(int));
        }

        int **colunaAux = (int**) malloc(nrRestricoes *sizeof(int));
        for (int i = 0; i < nrRestricoes; i++){
            colunaAux[i] = (int*)malloc(2 * sizeof(int));
        }

        for (int j = 0; j < nrRestricoes; j++){
            if(naoBasicas[posiMenorCustoRelativo] >= nrVariaveis + nrFolga){
                //procurar o valor nas variaveis artificiais
                colunaAux[j][0] = matrizArtificial[j][naoBasicas[posiMenorCustoRelativo] - nrVariaveis - nrFolga][0];
                colunaAux[j][1] = matrizArtificial[j][naoBasicas[posiMenorCustoRelativo] - nrVariaveis - nrFolga][1];
            } else if(naoBasicas[posiMenorCustoRelativo] >= nrVariaveis){
                //procurar o valor nas variaveis básicas   
                colunaAux[j][0] = matrizFolga[j][naoBasicas[posiMenorCustoRelativo] - nrVariaveis][0];
                colunaAux[j][1] = matrizFolga[j][naoBasicas[posiMenorCustoRelativo] - nrVariaveis][1];
            } else {
                colunaAux[j][0] = matriz[j][naoBasicas[posiMenorCustoRelativo]][0];
                colunaAux[j][1] = matriz[j][naoBasicas[posiMenorCustoRelativo]][1];
            }            
        }
        //calcular e guardar o passo enquando estou calculando a direção simplex.
        double menorPasso = INFINITY;
        int posiMenorPasso;
        double auxDouble;
        for (int i = 0; i < nrRestricoes; i++){
            direcaoSimplex[i][0] = 0;
            direcaoSimplex[i][1] = 1;
            for (int j = 0; j < nrRestricoes; j++){
                aux[0] = matrizBasicaInversa[i][j][0] * colunaAux[j][0];
                aux[1] = matrizBasicaInversa[i][j][1] * colunaAux[j][1];
                aux2 = operaMatriz(aux[0],aux[1],direcaoSimplex[i][0],direcaoSimplex[i][1], 1);
                direcaoSimplex[i][0] = aux2[0];
                direcaoSimplex[i][1] = aux2[1];
            }
            
            if((direcaoSimplex[i][0]/direcaoSimplex[i][1]) > 0){
                auxDouble = (X_B[i][0]*direcaoSimplex[i][1])/(X_B[i][1]*direcaoSimplex[i][0]);
                if(menorPasso > auxDouble){
                    menorPasso = auxDouble;
                    posiMenorPasso = i;
                }
            }
        }
        if(menorPasso == INFINITY){
            printf("problema sem solução");
            return -1;
        }

        printf("\n%d sai da base\n\n",basicas[posiMenorPasso]);

        printf("%d entra na base\n\n",naoBasicas[posiMenorCustoRelativo]);

        aux[0] = basicas[posiMenorPasso];
        basicas[posiMenorPasso] = naoBasicas[posiMenorCustoRelativo];
        naoBasicas[posiMenorCustoRelativo] = aux[0];

        //Se ainda tiver variaveis artificiais na base -> fase I, se não, fase II. 
        if(flagFase == 1){
            flagFase = 2;
            for(int i=0; i<nrRestricoes; i++){
                if(basicas[i] >= nrVariaveis + nrFolga){
                    flagFase = 1;
                    break;
                }
            }
            if(flagFase == 2){
                printf("\n-----   FASE-II   -----\n");
            }
        }
    }
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

    //Guarda os valores de variaveis artificiais.
    int ***matrizArtificial = (int ***)malloc((nrRestricoes) * sizeof(int**));
    for(int i = 0; i < (nrRestricoes); i++) {
        matrizArtificial[i] = (int **)malloc(nrArtificiais * sizeof(int*));
        for (int j = 0; j < nrArtificiais; j++){
            matrizArtificial[i][j] = (int*)malloc(2 * sizeof(int));
        }
    }

    preTratamentoSimplex(matriz,sinalVetorB,matrizFolga,matrizArtificial, nrRestricoes, nrVariaveis);

    
    int *basicas;
    basicas = (int*) malloc(nrRestricoes*sizeof(int));
    int *naoBasicas;
    int nrNaobasicas = nrVariaveis + nrFolga + nrArtificiais - nrRestricoes;
    naoBasicas = (int*) malloc(nrNaobasicas*sizeof(int));


    inicializaBase(basicas,naoBasicas,nrVariaveis,nrFolga,nrArtificiais, matrizFolga, matrizArtificial);


    printf("\n\nMatriz:\t\tsinal e B\tFolga\t\tArtificial\n");
    for(int i=0; i<=nrRestricoes; i++){
        for(int j=0; j<nrVariaveis; j++){
            printf("%d/%d  ", matriz[i][j][0],matriz[i][j][1]);
        }
        if(i != nrRestricoes){
            printf("\t\t");
            for(int j=0; j<2; j++){
                printf("%d/%d  ", sinalVetorB[i][j][0],sinalVetorB[i][j][1]);
            }
            printf("\t\t");
            for(int j=0; j<nrFolga; j++){
                printf("%d/%d  ", matrizFolga[i][j][0],matrizFolga[i][j][1]);
            }
            printf("\t\t");
            for(int j=0; j<nrArtificiais; j++){
                printf("%d/%d  ", matrizArtificial[i][j][0], matrizArtificial[i][j][1]);
            }
        }
        printf("\n");
    }

    printf("\n\nbase: ");
    for(int i=0; i<nrRestricoes; i++){
        printf("%d  ", basicas[i]);
    }

    printf("\n\nFora da base: ");
    for(int i=0; i<nrNaobasicas; i++){
        printf("%d  ", naoBasicas[i]);
    }

    printf("\n\n NrArtificiais = %d\n\n", nrArtificiais);


    int retorno = faseI_II(basicas,naoBasicas,matriz,sinalVetorB, matrizFolga, matrizArtificial, 
              nrVariaveis, nrRestricoes, nrFolga, nrArtificiais);
    

    printf("retorno: %d\n\n", retorno);

   

    return 0;
}