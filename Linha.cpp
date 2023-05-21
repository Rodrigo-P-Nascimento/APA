#include "Linha.h"

Linha::Linha(vector<vector<int>>* matrizDeAdj, int indice)
{
    this->matrizDeAdj = matrizDeAdj;
    this->tempoTotal = 0;
    this->produtos.clear();
    this->indice = indice;
}

Linha::~Linha()
{
    this->tempoTotal = 0;
    this->produtos.clear();
}

void Linha::pushProduto(Produto* item)
{
    if (!produtos.empty())     //se nao vazia, adiciona tambem o tempo de transicao
    {
        tempoTotal += (*matrizDeAdj)[produtos.back().indice][item->indice];   //adiciona o tempo de transicao do ultimo produto para o proximo (item)
    }
    produtos.push_back(*item);
    item->estado = indice;
    tempoTotal += item->tempo;
}

Produto* Linha::popProduto()
{
    Produto* ultimo = &produtos.back();

    if (produtos.size() >= 2)      //se tiver mais de um item, remove o item e o tempo de transicao
    {   
        Produto *penultimo = &produtos[produtos.size()-2];
        tempoTotal -= (*matrizDeAdj)[penultimo->indice][ultimo->indice];
    }
    tempoTotal -= ultimo->tempo;
    ultimo->estado = PRODUTO_DISPONIVEL;
    produtos.pop_back();
    return ultimo;
}

int Linha::getTempoTotal()
{
    return tempoTotal;
}

int Linha::getTempoParcial(Produto* produtoCandidato)
{
    if (produtos.size() > 0)   //se nao tiver vazia, calcula o tempo de transicao
        return (*matrizDeAdj)[produtos.back().indice][produtoCandidato->indice] + produtoCandidato->tempo;
    else
        return produtoCandidato->tempo;
}