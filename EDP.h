#ifndef EDP_H_
#define EDP_H_

#include <vector>
#include "Produto.cpp"

using namespace std;

/*
Essa classe serve como estrutura de dados, a sigla EDP significa Estrutra de Dados do Projeto.
*/

class EDP{
private:
    int nProd, mLinha;//nPRod é o numero de produtos, e mLinha é o numero de linhas de produçãoi
    int maiorValor; //Guarda o maior valor em minutos da linha de produção que consumiu mais tempo
    vector<vector<Produto>> estrutura; //isso representa cada linha de produção e os seus produtos
        //estrutura[0][i] linha de produção 1 -> com um vector que guarda os seus j produtos
    vector<int> valorPorLinha; //cada linha[j] tem um valor de tempo X, e esse vector guarda esses valores
    vector<int> produtoAdicionado; //se tiver 0 o produto[j] foi adicionado na linha[x], se 1 o produto[j] ainda não foi adicionado
    
public:
    EDP(int n, int m, vector<Produto>* custoTempo);
    void guloso(vector<Produto>* custoTempo, vector<vector<int>>* matriz);
    int somaLinha(int l, vector<vector<int>>* matriz);
    void imprimeTudo();
    ~EDP();
};

EDP::~EDP(){
}

#endif
