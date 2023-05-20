#ifndef LINHA_H
#define LINHA_H
#include "Produto.h"
#include <vector>
#include <string>

#pragma once

class Linha
{
public:
    Linha(vector<vector<int>>* matrizDeAdj);
    ~Linha();

    void pushProduto(Produto* item);
    Produto* popProduto();
    int getTempoTotal();
    string getProdutos(); // Retorna uma string com os produtos da linha
    int getTempoParcial(Produto* produtoCandidato); // Retorna o tempo a ser somado a linha caso adicionado o produtoCandidato
    vector<Produto> produtos;

private:
    unsigned tempoTotal;
    vector<vector<int>>* matrizDeAdj;
};

#endif