#include <iostream>
#include <fstream>
#include <vector>
#include "EDP.h"

#define PATH "/home/bertrand/APA/APA/auxiliar/entrada.txt"

using namespace std;

void lerEntrada(int& n_produtos, int& m_linhas, vector<int>& tempo_produto, vector<vector<int>>& matriz_transicao) {
    ifstream entrada(PATH);

    if (!entrada.is_open()) {
        cout << "Erro ao abrir arquivo" << endl;
        return;
    }

    entrada >> m_linhas;
    entrada >> n_produtos;

    tempo_produto.resize(n_produtos);

    matriz_transicao.resize(n_produtos);
    for (int i = 0; i < n_produtos; i++) {
        matriz_transicao[i].resize(n_produtos);
    }

    for (size_t i = 0; i < n_produtos; i++) {
        entrada >> tempo_produto[i];
    }

    for (size_t i = 0; i < n_produtos; i++) {
        for (size_t j = 0; j < n_produtos; j++) {
            entrada >> matriz_transicao[i][j];
        }
    }

    entrada.close();
}

int main(int argc, char const *argv[]) {
    int n_produtos, m_linhas;
    vector<int> tempo_produto;
    vector<vector<int>> matriz_transicao;
    vector<Produto> produtos;

    lerEntrada(n_produtos, m_linhas, tempo_produto, matriz_transicao);

    for(int i=0; i < n_produtos; i++){
        produtos.push_back(Produto (i,tempo_produto[i], true));
    }

    EDP dados = EDP(n_produtos, m_linhas, &produtos, &matriz_transicao);

    dados.guloso();
    dados.imprimirTudo();

    return 0;
}
