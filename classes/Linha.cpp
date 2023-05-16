#include "Linha.h"

Linha::Linha(vector<vector<int>>* matriz_transicao)
{
    tempo_transicao = matriz_transicao;
    tempo_total = 0;
}

Linha::~Linha()
{

}

void Linha::push_Produto(Produto* item)
{
    if (!produtos_na_linha.empty())     //se nao vazia, adiciona tambem o tempo de transicao
    {
        Produto item_anterior = produtos_na_linha.back();
        tempo_total += (*tempo_transicao)[item_anterior.indice][item->indice];
    }
    produtos_na_linha.push_back(*item);
    tempo_total += item->tempo;
}

void Linha::pop_Produto()
{
    if (produtos_na_linha.size() >= 2)      //se tiver mais de um item, remove o item e o tempo de transicao
    {
        Produto penultimo = produtos_na_linha[produtos_na_linha.size()-2];
        tempo_total -= (*tempo_transicao)[penultimo.indice][produtos_na_linha.back().indice];
    }
    tempo_total -= produtos_na_linha.back().tempo;
    produtos_na_linha.pop_back();
}

int Linha::get_tempo_total()
{
    return tempo_total;
}

string Linha::get_produtos()
{
    string linha;

    for (size_t i = 0; i < produtos_na_linha.size(); i++)
    {
        linha += produtos_na_linha[i].tempo;
        linha += " ";
    }
    return linha;
}