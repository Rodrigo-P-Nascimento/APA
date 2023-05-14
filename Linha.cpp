#include "Linha.h"

Linha::Linha()
{
    this->custo = 0;
    //this->produtos.resize(0, Produto(0, 0, false));
    this->produtos.clear();
}

Linha::~Linha()
{

}