#include <iostream>
#include <fstream>
#include <vector>
#include "Linha.cpp"
#include "Produto.cpp"
#include <climits>

#define PATH "./auxiliar/instancias/entrada.txt"

using namespace std;

int nProdutos, mLinhas;
vector<vector<int>> matrizDeAdj; // Matriz com os tempos de manutenção, para receber da entrada de arquivo
vector<Produto> produtos; // Lista com a abstração completa dos produtos

/**
 * Função para ler arquivos .txt de entrada, com o seguinte formato:
 * numero_de_linhas
 * numero_de_produtos
 * 
 * array t
 * 
 * matriz s
*/
void lerEntrada() {
    ifstream entrada(PATH);
    int tempo;

    if (!entrada.is_open()) {
        cout << "Erro ao abrir arquivo" << endl;
        return;
    }

    entrada >> mLinhas;
    entrada >> nProdutos;

    matrizDeAdj.resize(nProdutos);
    for (int i = 0; i < nProdutos; i++) {
        matrizDeAdj[i].resize(nProdutos);
    }

    for (size_t i = 0; i < nProdutos; i++) {
        entrada >> tempo;
        produtos.push_back(Produto(i, tempo, PRODUTO_DISPONIVEL));
    }

    for (size_t i = 0; i < nProdutos; i++) {
        for (size_t j = 0; j < nProdutos; j++) {
            entrada >> matrizDeAdj[i][j];
        }
    }

    entrada.close();
}

/**
 * Retorna o índice do produto mais custoso em relação
 * ao tempo de produção, dada uma lista (vector) de produtos
*/
int maiorProduto(vector<Produto>& produtos){
    int indiceDoMaiorProduto = 0;
    int maiorCustoEncontrado = 0;
    for (int j = 0; j < produtos.size(); j++){
        Produto produtoAtual = produtos.at(j);
        if (produtoAtual.tempo > maiorCustoEncontrado && produtoAtual.estado == PRODUTO_DISPONIVEL){
            maiorCustoEncontrado = produtoAtual.tempo;
            indiceDoMaiorProduto = j;
        }
    }
    return indiceDoMaiorProduto;
}

/**
 * Retorna uma referência do objeto da linha menos custosa em relação
 * ao tempo de produção, dada uma lista (vector) de linhas
 * de produção
*/
Linha& menorLinhaDeTodas(vector<Linha>& linhas){
    int menorCustoDeLinha = INT_MAX, indiceDaMenorLinha;
    for (int i = 0; i < linhas.size(); i++){
        Linha linhaAtual = linhas.at(i);
        if (linhaAtual.getTempoTotal() < menorCustoDeLinha){
            menorCustoDeLinha = linhaAtual.getTempoTotal();
            indiceDaMenorLinha = i;
        }
    }
    return linhas.at(indiceDaMenorLinha);
}

/**
 * Grande heurística construtiva, para o seguinte problema:
 * 
 * Alocação de produtos entre linhas de produção, que têm
 * tempo de manutenção diferentes entre cada produto.
 * 
 * Implementada como um algoritmo guloso.
*/
vector<Linha> heuristicaConstrutiva(){
    int produtosRestantes = produtos.size(); // Número de produtos que não foram adicionados à solução
    vector<Linha> linhas; // Estrutura de dados (floresta com matriz de adjacência) da solução vazia
    
    for (size_t i = 0; i < mLinhas; i++)
        linhas.push_back(Linha(&matrizDeAdj, i));
    
    // Adicionando, em cada linha, o produto mais custoso disponível
    for (int i = 0; i < mLinhas; i++){
        int indiceDoMaiorProduto = maiorProduto(produtos); // Recuperando o índice produto mais custoso não adicionado
        linhas.at(i).pushProduto(&produtos.at(indiceDoMaiorProduto)); // Adicionando o maior produto a linha atual
        produtosRestantes--; // Decrementando o contador de produtos disponíveis
    }

    //int ultimoProduto; // Índice do último produto da menor linha
    int indiceDaMelhorTransicao; // Índice do produto que oferece o melhor custo de manutenção em relação ao último produto
    int menorSoma = 0; // Auxiliar para a menor soma: custo do produto + tempo de manutenção

    /**
     * Parte principal do algoritmo guloso.
     * Adiciona todos os produtos restantes de acordo com o critério guloso desenvolvido
     */
    while (produtosRestantes){
        Linha& menorLinha = menorLinhaDeTodas(linhas); // Recuperando o objeto da menor linha, que é modificado posteriormente
        Produto ultimoProduto = menorLinha.produtos.back(); // Recuperando o objeto do último produto da menor linha
        int menorSoma = INT_MAX; // Auxiliar para achar a menor soma: tempo + manutenção

        /**
         * Recupera o produto candidato que cumpre o critério guloso.
         * Faz isso percorrendo a linha da matriz de tempos de manutenção (matriz de adjacência)
         * correspondente ao último produto da menor linha.
        */
        for (int i = 0; i < produtos.size(); i++){
            Produto produtoAtual = produtos.at(i);
            int tempoTransicaoAtual = matrizDeAdj.at(ultimoProduto.indice).at(i);

            /**
             * Critério guloso: salvar o índice do produto que resultaria no
             * menor custo para a soma do tempo de produção mais tempo de manutenção
             * em relação ao último da menor linha
             */
            if (produtoAtual.tempo + tempoTransicaoAtual < menorSoma && produtoAtual.estado == PRODUTO_DISPONIVEL){
                indiceDaMelhorTransicao = i;
                menorSoma = produtoAtual.tempo + tempoTransicaoAtual;
            }
        }
        menorLinha.pushProduto(&produtos.at(indiceDaMelhorTransicao));

        //produtos.at(indiceDaMelhorTransicao).foiAdicionado = true; // Marcando o produto como adicionado
        //menorLinha.produtos.push_back(produtos.at(indiceDaMelhorTransicao)); // Adicionando o produto à solução
        //menorLinha.custo += produtos.at(indiceDaMelhorTransicao).tempo + matrizDeAdj.at(ultimoProduto).at(indiceDaMelhorTransicao); // Atualizando o custo total da linha atual
        produtosRestantes--; // Decrementando o contador de produtos disponíveis
    }
    return linhas; // Retornando os vértices da solução: um grafo desconexo, não simétrico e acíclico; em outras palavras, uma floresta que é implementada com uma matriz de adjacência
}

int maiorTransicao(Linha& linhaObjetivo)    //procura o produto que causa o maior custo em transicoes na linha
{
    int maiorSoma = 0;
    int indiceMaiorSoma;
    for (size_t i = 0; i < linhaObjetivo.produtos.size(); i++) 
    {
        int soma = linhaObjetivo.getTempoParcial(i);
        if (maiorSoma < soma)
        {
            maiorSoma = soma;
            indiceMaiorSoma = i;
        }
    }
    return indiceMaiorSoma;
}

void swap_2(vector<Linha>& solucao_inicial, unsigned indice_linha)
{
    Linha& linhaObjetivo = solucao_inicial.at(indice_linha);
    int produtoObjetivo = maiorTransicao(linhaObjetivo);
    

    
   



    
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
        if (linhas.at(i).getTempoTotal() > maiorCustoDeLinha){
            maiorCustoDeLinha = linhas.at(i).getTempoTotal();
            indiceMaiorLinha = i;
        }
        cout << " | Custo = " << linhas.at(i).getTempoTotal() << endl;
    }
    cout << "Custo da maior linha (funcao objetivo): " << linhas.at(indiceMaiorLinha).getTempoTotal() << endl;
    cout << "\n";
}

int main(int argc, char const *argv[]) {
    // Lendo o arquivo de entrada
    lerEntrada();

    vector<Linha> solucao = heuristicaConstrutiva();
    imprimirSolucao(solucao);

    swap_2(solucao, 1);
    imprimirSolucao(solucao);

    return 0;
}
