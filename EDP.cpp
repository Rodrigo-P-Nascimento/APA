#include <iostream>
#include <stdio.h>
#include "EDP.h"

using namespace std;

EDP::EDP(int n, int m, vector<Produto>* custoTempo){

    maiorValor=0;//valor de inicio 
    nProd = n;
    mLinha = m;
    valorLinha.resize(n, 0); //inicilializar o valor do tempo total de cada linha com 0
    prodAdd.resize(n, 1);//todos os produtos estão disponiveis
    estrutura.resize(m);//iniciamos a estrutura com 0


    //for(int i=0; i < m; i++){
        //estrutura[i].resize(1);
    //}
}

void EDP::guloso(vector<Produto>* custoTempo, vector<vector<int>>* matrix){

    int maiorTempo=0;
    int indice=0;
    int produtosRestantes = nProd;

    for(int i=0; i < mLinha; i++){

        maiorTempo = 0;//Ela guarda o maior custo de produção de um produto
        indice = 0;//Ela guarda o indice do produto que oferece maior custo

        for(int j=0; j < nProd; j++){
            if(custoTempo->at(j).temp > maiorTempo && prodAdd[j] != 0){
                maiorTempo = custoTempo->at(j).temp;
                indice = j;
            }
        }

        estrutura[i].push_back(Produto (indice, maiorTempo, false));//Como o i travado garantimos que todas as linhas recebem apenas um unico produto
        valorLinha[i] = somaLinha(i);//damos o valor do custo do produto
        prodAdd[indice] = 0;//Setamos os produtos que já foram colocados nas linhas como 0, para controle de informações
        produtosRestantes--;//Dimiunimos a quantidade de produtos disponiveis
    }//Esse |for| serve para colocar em todas as linhas o produto que consume mais tempo.

    int menorLinha = 999999;
    int indiceMenorLinha = 0;
    int indiceUltimo = -1;
    int indiceDaMelhorTroca = -1;
    int menorsoma = 99999;

    while (produtosRestantes){
        menorLinha = 999999;
        //cout << "valor da linha 1: " << valorLinha[0] << endl;
        //cout << "valor da linha 2: " << valorLinha[1] << endl;
        for(int i =0; i < mLinha; i++){
            valorLinha[i] = somaLinha(i);
        }

        for(int i=0; i < mLinha; i++){
            if(valorLinha[i] <= menorLinha){
                menorLinha = valorLinha[i];
                indiceMenorLinha = i;
            }

            printf("Prod [%d] - Menor linha: %d \nIndice da menor linha: %d \n", produtosRestantes,menorLinha, indiceMenorLinha);


        }
        //pegar o indice da linha que contem o menor valor de tempo
        //cout << "Indice da linha com o menor valor: " << indiceMenorLinha << endl;
        //vamos pegar o ultimo produto dessa lista
        //olhar na matriz com é o produto que tem o menor custo de transição
        //colocar esse produto na linha x
        //refazer a soma total

        indiceUltimo = estrutura[indiceMenorLinha].back().ind;
        indiceDaMelhorTroca = -1;
        menorsoma = 99999;

        for (int i=0; i < nProd; i++){
            if((matrix->at(indiceUltimo).at(i) + custoTempo->at(i).temp < menorsoma) && prodAdd[i] != 0){
                indiceDaMelhorTroca = i;
                menorsoma = matrix->at(indiceUltimo).at(i) + custoTempo->at(i).temp;
            }
        }

        //cout << "Melhor troca possivel eh: [" << indiceDaMelhorTroca << "] valor:" << custoTempo->at(indiceDaMelhorTroca).temp << endl;

        estrutura[indiceMenorLinha].push_back(Produto (indiceDaMelhorTroca, custoTempo->at(indiceDaMelhorTroca).temp, false));
        valorLinha[indiceMenorLinha] = somaLinha(indiceMenorLinha);//damos o valor do custo do produto
        prodAdd[indiceDaMelhorTroca] = 0;//Setamos os produtos que já foram colocados nas linhas como 0, para controle de informações
        produtosRestantes--;
    } 

}

void EDP::imprimeTudo(){
    cout << "Maior valor de soma ateh agora: " << maiorValor << endl;
    cout << "Estrutura de Dados" << endl;

    
    for(int i=0; i < mLinha; i++ ){
        cout << "Elementos da Linha[" << i << "] = ";
        for(size_t j = 0; j < estrutura[i].size(); j++){
            cout << estrutura[i][j].temp << " ";
        }
        cout << "| valor da linha[" << i << "]: " << valorLinha[i];
        cout << "\n" ;
    }
    
}

int EDP::somaLinha(int j){
    int soma=0;

    for(int i=0; i < estrutura[j].size(); i++){
        soma = soma + estrutura[j][i].temp;
    }

    return soma;
}