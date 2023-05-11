#ifndef Produto_H_
#define Produto_H_

using namespace std;

class Produto{

public:
    int ind;
    int temp;
    bool disp;

    Produto(int indice, int tempo, bool disponibilidade);
    ~Produto();
};

#endif
