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
        tempo_total += (*tempo_transicao)[produtos_na_linha.back().indice][item->indice];   //adiciona o tempo de transicao do ultimo produto para o proximo (item)
    }
    produtos_na_linha.push_back(*item);
    item->disponivel = false;
    tempo_total += item->tempo;
}

Produto* Linha::pop_Produto()
{
    Produto* ultimo = &produtos_na_linha.back();

    if (produtos_na_linha.size() >= 2)      //se tiver mais de um item, remove o item e o tempo de transicao
    {   
        Produto *penultimo = &produtos_na_linha[produtos_na_linha.size()-2];
        tempo_total -= (*tempo_transicao)[penultimo->indice][ultimo->indice];
    }
    tempo_total -= ultimo->tempo;
    ultimo->disponivel = true;
    produtos_na_linha.pop_back();
    return ultimo;
}

int Linha::get_tempo_total()
{
    return tempo_total;
}

string Linha::get_produtos()
{
    string linha = "";

    for (size_t i = 0; i < produtos_na_linha.size(); i++)
    {
        linha += to_string(produtos_na_linha[i].tempo);
        linha += " ";
    }
    return linha;
}

int Linha::get_tempo_parcial(Produto* produto_candidato)
{
    if (produtos_na_linha.size() > 0)   //se nao tiver vazia, calcula o tempo de transicao
        return (*tempo_transicao)[produtos_na_linha.back().indice][produto_candidato->indice] + produto_candidato->tempo;
    else
        return produto_candidato->tempo;
}