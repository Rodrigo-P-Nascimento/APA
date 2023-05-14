#include <iostream>
#include <fstream>
#include <vector>
#include "EDP.cpp"
#include "Linha.cpp"

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

vector<Linha> heuristica(int nLinhas, vector<Produto>& produtos, vector<vector<int>>& matrizDeAdj){
    int produtosRestantes = produtos.size();
    vector<Linha> linhas(nLinhas);
    for (int i = 0; i < nLinhas; i++){
        int indiceDoMaior = 0;
        int maiorTempo = 0; // Auxiliar para salvar o maior tempo atual

        // Recuperar o maior produto
        for (int j = 0; j < produtos.size(); j++){
            Produto& produtoAtual = produtos.at(j);
            if (produtoAtual.tempo > maiorTempo && !produtoAtual.foiAdicionado){
                maiorTempo = produtoAtual.tempo;
                indiceDoMaior = j;
            }
        }
        linhas.at(i).produtos.push_back(produtos.at(indiceDoMaior));
        linhas.at(i).custo += produtos.at(indiceDoMaior).tempo;
        produtos.at(indiceDoMaior).foiAdicionado = true;
        produtosRestantes--;
    }

    //int menorCustoDeLinha = 0; //Essa variável vai guardar qual o valor da menor linha dentre as m linhas possiveis da nossa estrutura
    int indiceMenorLinha = 0;//Guarda o indice da menor linha possivél
    int indiceUltimoProduto = -1;//Guarda o indice do ulitmo produto da menor linha
    int indiceDaMelhorTransicao = -1;//Guarda o indice do produto que oferece o custo de transisasao de produto[x] para o produto[y]
    int menorSoma = 0;//Guarda o valor da menor soma de custo + transição
    while (produtosRestantes){
        int menorCustoDeLinha = INT_MAX;
        for (int i = 0; i < linhas.size(); i++){
            Linha& linhaAtual = linhas.at(i);
            if (linhaAtual.custo < menorCustoDeLinha){
                menorCustoDeLinha = linhaAtual.custo;
                indiceMenorLinha = i;
            }
        }
        Linha& menorLinha = linhas.at(indiceMenorLinha);
        indiceUltimoProduto = menorLinha.produtos.back().indice;
        int menorSoma = INT_MAX;
        for (int i = 0; i < produtos.size(); i++){
            Produto produtoAtual = produtos.at(i);
            int tempoTransicaoAtual = matrizDeAdj.at(indiceUltimoProduto).at(i);
            if (produtoAtual.tempo + tempoTransicaoAtual < menorSoma && !produtoAtual.foiAdicionado){
                indiceDaMelhorTransicao = i;
                menorSoma = produtoAtual.tempo + tempoTransicaoAtual;
            }
        }
        produtos.at(indiceDaMelhorTransicao).foiAdicionado = true;
        menorLinha.produtos.push_back(produtos.at(indiceDaMelhorTransicao));
        menorLinha.custo += produtos.at(indiceDaMelhorTransicao).tempo + matrizDeAdj.at(indiceUltimoProduto).at(indiceDaMelhorTransicao);
        produtosRestantes--;
        //cout << produtosRestantes << endl;
    }
    return linhas;
}

void imprimirSolucao(vector<Linha>& linhas){
    int indiceMaiorLinha;
    int maiorCustoDeLinha = 0;
    cout << "\n";
    for (int i = 0; i < linhas.size(); i++){
        cout << "Elementos da Linha " << i << " = ";
        for (int j = 0; j < linhas.at(i).produtos.size(); j++){
            cout << linhas.at(i).produtos.at(j).tempo << " ";
        }
        if (linhas.at(i).custo > maiorCustoDeLinha){
            indiceMaiorLinha = i;
        }
        cout << "| Custo da linha " << i << ": " << linhas.at(i).custo << endl;
        //cout << "DEPOIS DE TER IMPRIMIDO UMA LINHA DE PRODUCAO" << endl;
    }
    cout << "Custo da maior linha (funcao objetivo): " << linhas.at(indiceMaiorLinha).custo << endl;
    cout << "\n";
}

int main(int argc, char const *argv[]) {
    int n_produtos, m_linhas;
    vector<int> produtos;
    vector<vector<int>> matriz;
    vector<Produto> matrizT;//matriz T para usar na ED

    lerEntrada(n_produtos, m_linhas, produtos, matriz);

    for(int i=0; i < n_produtos; i++){
        matrizT.push_back( Produto(i, produtos[i], false));
    }

    EDP dados = EDP(n_produtos, m_linhas);
    //cout << "antes da heuristica" << endl;
    vector<Linha> solucao = heuristica(m_linhas, matrizT, matriz);
    //cout << "depois da heuristica" << endl;
    imprimirSolucao(solucao);
    //cout << "depois de imprimirSolucao" << endl;

    //dados.guloso(&matrizT, &matriz);
    //dados.imprimirTudo();

    return 0;
}
