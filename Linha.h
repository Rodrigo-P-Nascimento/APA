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
    int getTempoParcial(Produto* produtoCandidato); // Retorna o tempo a ser somado a linha caso adicionado o produtoCandidato
    vector<Produto> produtos;

private:
    int indice;
    unsigned tempoTotal;
    vector<vector<int>>* matrizDeAdj;
};

#endif