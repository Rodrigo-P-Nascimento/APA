#ifndef LINHA_H
#define LINHA_H

#include "Produto.h"
#include <vector>
#include <string>

#pragma once

class Linha
{
public:
    Linha(vector<vector<int>>* matriz_transicao);
    ~Linha();
    void push_Produto(Produto* item);
    Produto* pop_Produto();
    int get_tempo_total();
    string get_produtos();                              //retorna uma string com os produtos da linha
    int get_tempo_parcial(Produto* produto_candidato);  //retorna o tempo a ser somado a linha caso adicionado o produto_candidato

private:
    unsigned tempo_total;
    vector<vector<int>>* tempo_transicao;
    vector<Produto> produtos_na_linha;

};

#endif