#include <iostream>
#include <fstream>
#include <vector>
#include "EDP.cpp"

#define PATH "entrada.txt"

using namespace std;

void lerEntrada(int& n_produtos, int& m_linhas, vector<int>& produtos, vector<vector<int>>& matriz) {
    ifstream entrada(PATH);

    if (!entrada.is_open()) {
        cout << "Erro ao abrir arquivo" << endl;
        return;
    }

    entrada >> m_linhas;
    entrada >> n_produtos;

    produtos.resize(n_produtos);

    matriz.resize(n_produtos);
    for (int i = 0; i < n_produtos; i++) {
        matriz[i].resize(n_produtos);
    }

    for (size_t i = 0; i < n_produtos; i++) {
        entrada >> produtos[i];
    }

    for (size_t i = 0; i < n_produtos; i++) {
        for (size_t j = 0; j < n_produtos; j++) {
            entrada >> matriz[i][j];
        }
    }

    entrada.close();
}

int main(int argc, char const *argv[]) {
    int n_produtos, m_linhas;
    vector<int> produtos;
    vector<vector<int>> matriz;
    vector<Produto> matrizT;//matriz T para usar na ED

    lerEntrada(n_produtos, m_linhas, produtos, matriz);

    for(int i=0; i < n_produtos; i++){
        matrizT.push_back(Produto (i,produtos[i], true));
    }

    EDP dados = EDP(n_produtos, m_linhas);

    dados.guloso(&matrizT, &matriz);
    dados.imprimeTudo();

    return 0;
}
