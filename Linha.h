#ifndef LINHA_H
#define LINHA_H
#include "Produto.h"
#include <vector>
#include <string>

#pragma once



class Linha
{
public:
    Linha(vector<vector<int>>* matrizDeAdj, int indice);
    ~Linha();

    void pushProduto(Produto* item);
    Produto* popProduto();
    int getTempoTotal();
    int getTempoParcial(unsigned produto_da_linha); // Retorna o tempo a ser somado a linha caso adicionado o produtoCandidato
    vector<Produto> produtos;

private:
    #define TRANSICAO(p1, p2) (*matrizDeAdj)[produtos.at(p1).indice][produtos.at(p2).indice]
    int indice;
    unsigned tempoTotal;
    vector<vector<int>>* matrizDeAdj;
};

#endif