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
    int nProd, mLinha;
    int maiorValor; //Valor em minutos da linha de produção que consumiu mais tempo
    vector<vector<Produto>> estrutura; //isso representa cada linha de produção e os seus produtos
        //estrutura[0][i] linha de produção 1 -> com um vector que guarda os seus j produtos
    vector<int> valorLinha; //cada linha[j] tem um valor de tempo X, e esse vector guarda esses valores
    vector<int> prodAdd; //se tiver 0 o produto[j] foi adicionado na linha X, se 1 o produto[j] ainda não foi adicionado
    
public:
    EDP(int n, int m, vector<Produto>* custoTempo);
    void guloso(vector<Produto>* custoTempo, vector<vector<int>>* matrix);
    int somaLinha(int l);
    void imprimeTudo();
    ~EDP();
};

EDP::~EDP(){
}

#endif
