#ifndef EDP_H_
#define EDP_H_

#pragma once

#include <iostream>
#include <vector>
#include "Produto.cpp"
#include "Linha.cpp"

using namespace std;

/*
Essa classe serve como estrutura de dados, a sigla EDP significa Estrutra de Dados do Projeto.
*/

class EDP{
private:
    int nProd;                          //numero de produtos
    int mLinha;                         //numero de linhas de produção
    int tempo_de_solucao;               //Guarda o tempo da linha de produção que consumiu mais tempo
    vector<vector<int>>* matrizDeAdj;   //matriz do tempo de transferencia
    vector<Produto>* produtos;          //vetor dos produtos a disposicao
    vector<Linha> linhas_producao;      //isso representa cada linha de produção e os seus produtos
    vector<bool> produtosAdicionados;   //se tiver 0 o produto[j] foi adicionado na linha[x], se 1 o produto[j] ainda não foi adicionado
    

public:
    EDP(int n, int m, vector<Produto>* produtos, vector<vector<int>>* matriz);
    void guloso();
    void imprimirTudo();
    ~EDP();
};

#endif
