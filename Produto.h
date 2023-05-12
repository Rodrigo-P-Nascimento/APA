#ifndef Produto_H_
#define Produto_H_

using namespace std;

/*
    Essa classe foi criada pra ajudar na hora de recuperar um determinado produto, por exemplo:
    Se tivermos dois produtos com o mesmo tempo de frabricação, qual dos dois é o que eu de fato vou pegar?
    Por isso que o campo "ind" é importante, com ele podemos fazer com que cada produto tenha um ID unico, e por mais que 
    os produtos tenham o mesmo custo, temp, o ID unico faz com que pegamos apenas o que de fato queremos manipular!
*/

class Produto{

public:
    int ind;//ind  = indice UNICO do produto
    int temp;//temp = tempo de produção do produto
    bool disp;// disp = disponibilidade do produto

    Produto(int indice, int tempo, bool disponibilidade);
    ~Produto();
};

#endif
