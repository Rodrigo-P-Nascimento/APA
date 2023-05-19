#ifndef LINHA_H
#define LINHA_H

#pragma once

#include <vector>
#include "Produto.h"

class Linha
{
public:
    Linha();
    ~Linha();

    int custo;
    vector<Produto> produtos;

private:

};

#endif