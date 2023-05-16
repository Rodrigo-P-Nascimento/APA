#ifndef EDP_H_
#define EDP_H_

#include <iostream>
#include <vector>
#include <climits>
#include "Produto.h"
#include "Linha.h"

using namespace std;

/*
Essa classe serve como estrutura de dados, a sigla EDP significa Estrutra de Dados do Projeto.
*/

class EDP{
private:
    int nProd, mLinha;//nPRod é o numero de produtos, e mLinha é o numero de linhas de produçãoi
    int maiorValor; //Guarda o maior valor em minutos da linha de produção que consumiu mais tempo
    vector<vector<int>>* matrizDeAdj;
    vector<Produto>* produtos;
    vector<Linha> linhas_producao; //isso representa cada linha de produção e os seus produtos
    vector<bool> produtosAdicionados; //se tiver 0 o produto[j] foi adicionado na linha[x], se 1 o produto[j] ainda não foi adicionado
    

public:
    EDP(int n, int m, vector<Produto>* produtos, vector<vector<int>>* matriz);
    void guloso();
    int somaLinha(int l, vector<vector<int>>* matriz);
    void imprimirTudo();
    ~EDP();
};

#endif
