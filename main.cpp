#include <iostream>
#include <fstream>
#include <vector>
#include "EDP.cpp"
#include "Linha.cpp"

#define PATH "./instancias/n10m2_A.txt"

using namespace std;

/**
 * Função para ler arquivos .txt de entrada, com o seguinte formato:
 * numero_de_linhas
 * numero_de_produtos
 * 
 * array t
 * 
 * matriz s
*/
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

/**
 * Retorna o índice do produto mais custoso em relação
 * ao tempo de produção, dada uma lista (vector) de produtos
*/
int maiorProduto(vector<Produto> produtos){
    int indiceDoMaiorProduto = 0;
    int maiorCustoEncontrado = 0;
    for (int j = 0; j < produtos.size(); j++){
        Produto produtoAtual = produtos.at(j);
        if (produtoAtual.tempo > maiorCustoEncontrado && !produtoAtual.foiAdicionado){
            maiorCustoEncontrado = produtoAtual.tempo;
            indiceDoMaiorProduto = j;
        }
    }
    return indiceDoMaiorProduto;
}

/**
 * Retorna o índice da linha menos custosa em relação
 * ao tempo de produção, dada uma lista (vector) de linhas
 * de produção
*/
int menorLinha(vector<Linha> linhas){
    int menorCustoDeLinha = INT_MAX, indiceDaMenorLinha;
    for (int i = 0; i < linhas.size(); i++){
        Linha linhaAtual = linhas.at(i);
        if (linhaAtual.custo < menorCustoDeLinha){
            menorCustoDeLinha = linhaAtual.custo;
            indiceDaMenorLinha = i;
        }
    }
    return indiceDaMenorLinha;
}

/**
 * Grande heurística construtiva para o problema:
 * 
 * Alocação de produtos entre linhas de produção, que têm
 * tempo de manutenção diferentes entre cada produto.
 * 
 * Implementada como um algoritmo guloso
*/
vector<Linha> heuristicaConstrutiva(int nLinhas, vector<Produto>& produtos, vector<vector<int>>& matrizDeAdj){
    int produtosRestantes = produtos.size(); // Número de produtos que não foram adicionados à solução
    vector<Linha> linhas(nLinhas); // Estrutura de dados (floresta com matriz de adjacência) da solução vazia
    
    // Adicionando o produto mais custoso disponível em cada linha
    for (int i = 0; i < nLinhas; i++){
        int indiceDoMaiorProduto = maiorProduto(produtos); // Recuperando o índice produto mais custoso não adicionado

        produtos.at(indiceDoMaiorProduto).foiAdicionado = true; // Marcando o produto mais custoso como adicionado
        linhas.at(i).produtos.push_back(produtos.at(indiceDoMaiorProduto)); // Adicionando o produto mais custoso a linha atual da solução
        linhas.at(i).custo += produtos.at(indiceDoMaiorProduto).tempo; // Atualizando o custo total da linha atual
        produtosRestantes--; // Decrementando o contador de produtos disponíveis
    }

    int indiceUltimoProduto = -1; // Índice do último produto da menor linha
    int indiceDaMelhorTransicao = -1; // Índice do produto que oferece o melhor custo de manutenção em relação ao último produto
    int menorSoma = 0; // Auxiliar para a menor soma: custo do produto + tempo de manutenção

    /**
     * Parte principal do algoritmo guloso.
     * Adiciona todos os produtos restantes de acordo com o critério guloso desenvolvido
     */
    while (produtosRestantes){
        int indiceMenorLinha = menorLinha(linhas); // Recuperando o índice da menor linha
        Linha& menorLinha = linhas.at(indiceMenorLinha); // Salvando o objeto da menor linha, que é modificado posteriormente
        indiceUltimoProduto = menorLinha.produtos.back().indice; // Recuperando o índice do último produto da menor linha
        int menorSoma = INT_MAX; // Auxiliar para achar a menor soma: tempo + manutenção

        /**
         * Recupera o produto candidato que cumpre o critério guloso.
         * Faz isso percorrendo a linha da matriz de tempos de manutenção (matriz de adjacência)
         * correspondente ao último produto da menor linha.
        */
        for (int i = 0; i < produtos.size(); i++){
            Produto produtoAtual = produtos.at(i);
            int tempoTransicaoAtual = matrizDeAdj.at(indiceUltimoProduto).at(i);

            /**
             * Critério guloso: salvar o índice do produto que resultaria no
             * menor custo para a soma do tempo de produção mais tempo de manutenção
             * em relação ao último da menor linha
             */
            if (produtoAtual.tempo + tempoTransicaoAtual < menorSoma && !produtoAtual.foiAdicionado){
                indiceDaMelhorTransicao = i;
                menorSoma = produtoAtual.tempo + tempoTransicaoAtual;
            }
        }
        produtos.at(indiceDaMelhorTransicao).foiAdicionado = true; // Marcando o produto como adicionado
        menorLinha.produtos.push_back(produtos.at(indiceDaMelhorTransicao)); // Adicionando o produto à solução
        menorLinha.custo += produtos.at(indiceDaMelhorTransicao).tempo + matrizDeAdj.at(indiceUltimoProduto).at(indiceDaMelhorTransicao); // Atualizando o custo total da linha atual
        produtosRestantes--; // Decrementando o contador de produtos disponíveis
    }
    return linhas; // Retornando os vértices da solução: um grafo desconexo, não simétrico e acíclico; em outras palavras, uma floresta que é implementada com uma matriz de adjacência
}

void imprimirSolucao(vector<Linha>& linhas){
    int indiceMaiorLinha;
    int maiorCustoDeLinha = 0;
    cout << "\n";
    for (int i = 0; i < linhas.size(); i++){
        cout << "Linha de producao " << i+1 << ": ";
        for (int j = 0; j < linhas.at(i).produtos.size(); j++){
            cout << "Produto " << linhas.at(i).produtos.at(j).indice+1;
            if (j < linhas.at(i).produtos.size()-1){
                cout << " -> ";
            }
        }
        if (linhas.at(i).custo > maiorCustoDeLinha){
            maiorCustoDeLinha = linhas.at(i).custo;
            indiceMaiorLinha = i;
        }
        cout << " | Custo = " << linhas.at(i).custo << endl;
    }
    cout << "Custo da maior linha (funcao objetivo): " << linhas.at(indiceMaiorLinha).custo << endl;
    cout << "\n";
}

int main(int argc, char const *argv[]) {
    int n_produtos, m_linhas;
    vector<int> temposDosProdutos; // Lista com os tempos de produção de cada produto, para receber da entrada de arquivo
    vector<vector<int>> matrizDeAdj; // Matriz com os tempos de manutenção, para receber da entrada de arquivo
    vector<Produto> produtos; // Lista com a abstração completa dos produtos

    // Lendo o arquivo de entrada
    lerEntrada(n_produtos, m_linhas, temposDosProdutos, matrizDeAdj);

    // Populando a lista de produtos com os tempos de produção correspondentes
    for (int i = 0; i < n_produtos; i++){
        produtos.push_back(Produto(i, temposDosProdutos[i], false));
    }

    //EDP dados = EDP(n_produtos, m_linhas);
    vector<Linha> solucao = heuristicaConstrutiva(m_linhas, produtos, matrizDeAdj);
    imprimirSolucao(solucao);

    //dados.guloso(&produtos, &matriz);
    //dados.imprimirTudo();

    return 0;
}
