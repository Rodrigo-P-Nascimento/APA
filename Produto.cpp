#include "Produto.h"

Produto::Produto(int indice, int tempo, bool disponibilidade){
    this->indice = indice;
    this->tempo = tempo;
    this->disponivel = disponibilidade;
}

Produto::~Produto(){
}