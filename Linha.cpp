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

int Linha::getIndiceLinha()
{
    return indice;
}

int Linha::getTempoParcial(unsigned indice) //retorna o tempo geral relativo ao produto, na posicao "indice" da linha, e suas transicoes
{
    if (produtos.size()-1 >= indice)   //verifica se o elemento pertence ao vetor
        return -1;

    int tempo = produtos.at(indice).tempo;

    if (indice-1 >= 0)
        tempo += TRANSICAO(indice-1, indice);
    
    if (indice+1 <= produtos.size()-1)
        tempo += TRANSICAO(indice, indice+1);

    return tempo;
}

void Linha::recalculaTempoTotal(){ //Esse metodo serve para recalcular o valor de uma linha se assim for desejado.
    int soma = 0;

    for(int i = 0; i < produtos.size(); i++){
        soma = soma + produtos.at(i).tempo;

        if(i < produtos.size() - 1){
            soma = soma + matrizDeAdj->at(produtos.at(i).indice).at(produtos.at(i+1).indice);
        }
    }

    tempoTotal = soma;
}
