#ifndef LINHA_H
#define LINHA_H
#include "Produto.h"
#include <vector>
#include <string>

#define TRANSICAO(p1, p2) (*matrizDeAdj)[produtos.at(p1).indice][produtos.at(p2).indice]

#pragma once

class Linha
{
public:
    Linha(vector<vector<int>>* matrizDeAdj, int indice);
    ~Linha();

    void pushProduto(Produto* item);
    void recalculaTempoTotal();//Se fizermos alguma coisa com os produtos esse metodo recalcula o valor de tempoTotal!
    Produto* popProduto();
    int getTempoTotal();
    int getIndiceLinha();
    int getTempoParcial(int produto_da_linha); //retorna o tempo geral relativo ao produto, na posicao "indice" da linha, e suas transicoes
    vector<Produto> produtos;

private:
    int indice;
    unsigned tempoTotal;
    vector<vector<int>>* matrizDeAdj;
};

#endif