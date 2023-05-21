#include "Linha.h"

Linha::Linha(vector<vector<int>>* matrizDeAdj)
{
    this->matrizDeAdj = matrizDeAdj;
    this->tempoTotal = 0;
    this->produtos.clear();
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
    item->disponivel = false;
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
    ultimo->disponivel = true;
    produtos.pop_back();
    return ultimo;
}

int Linha::getTempoTotal()
{
    return tempoTotal;
}

string Linha::getProdutos()
{
    string linha = "";

    for (size_t i = 0; i < produtos.size(); i++)
    {
        linha += to_string(produtos[i].tempo);
        linha += " ";
    }
    return linha;
}

int Linha::getTempoParcial(Produto* produtoCandidato)
{
    if (produtos.size() > 0)   //se nao tiver vazia, calcula o tempo de transicao
        return (*matrizDeAdj)[produtos.back().indice][produtoCandidato->indice] + produtoCandidato->tempo;
    else
        return produtoCandidato->tempo;
}

//Esse metodo serve para recalcular o valor de uma linha se assim for desejado.
void Linha::recalculaTempoTotal(){
    int soma = 0;

    for(int i = 0; i < produtos.size(); i++){
        soma = soma + produtos.at(i).tempo;

        if(i < produtos.size() - 1){
            soma = soma + matrizDeAdj->at(produtos.at(i).indice).at(produtos.at(i+1).indice);
        }
    }

    tempoTotal = soma;
}
