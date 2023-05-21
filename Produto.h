#ifndef Produto_H
#define Produto_H

#pragma once

#define PRODUTO_DISPONIVEL -1

using namespace std;

/*
    Essa classe foi criada pra ajudar na hora de recuperar um determinado produto, por exemplo:
    Se tivermos dois produtos com o mesmo tempo de frabricação, qual dos dois é o que eu de fato vou pegar?
    Por isso que o campo "ind" é importante, com ele podemos fazer com que cada produto tenha um ID unico, e por mais que 
    os produtos tenham o mesmo custo, temp, o ID unico faz com que pegamos apenas o que de fato queremos manipular!
*/

class Produto{

public:
    int indice; // indice  = indice UNICO do produto
    int tempo; // tempo = tempo de produção do produto
    int estado; //indica se o produto está disponivel, ou se esta em alguma linha

    Produto(int indice, int tempo, int estado);
    ~Produto();
};

#endif
