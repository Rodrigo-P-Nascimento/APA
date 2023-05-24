#include <iostream>
#include <fstream>
#include <vector>
#include "Linha.cpp"
#include "Produto.cpp"
#include <climits>
#include <algorithm>
#include <chrono>
#include <ctime> 
#include <map>

#define VALOR_OTIMO 1

using namespace std;

int nProdutos, mLinhas;
vector<vector<int>> matrizDeAdj; // Matriz com os tempos de manutenção, para receber da entrada de arquivo
vector<Produto> produtos; // Lista com a abstração completa dos produtos
map<string, int> arquivos = {
    {"n10m2_A.txt", VALOR_OTIMO},
    {"n10m2_B.txt", VALOR_OTIMO},
    {"n15m3_A.txt", VALOR_OTIMO},
    {"n15m3_B.txt", VALOR_OTIMO},
    {"n15m4_A.txt", VALOR_OTIMO},
    {"n15m4_B.txt", VALOR_OTIMO},
    {"n29m4_A.txt", VALOR_OTIMO},
    {"n29m4_B.txt", VALOR_OTIMO},
    {"n29m6_A.txt", VALOR_OTIMO},
    {"n29m6_B.txt", VALOR_OTIMO},
    {"n40m5_A.txt", VALOR_OTIMO},
    {"n40m5_B.txt", VALOR_OTIMO},
    {"n52m5_A.txt", VALOR_OTIMO},
    {"n52m5_B.txt", VALOR_OTIMO},
    {"n450m16_A.txt", VALOR_OTIMO},
    {"n500m10_A.txt", VALOR_OTIMO}
};

/**
 * Função para ler arquivos .txt de entrada, com o seguinte formato:
 * numero_de_linhas
 * numero_de_produtos
 * 
 * array t
 * 
 * matriz s
*/
void lerEntrada(string PATH) {
    ifstream entrada(PATH);
    int tempo;
    produtos.clear();
    matrizDeAdj.clear();

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

    for (int i = 0; i < nProdutos; i++) {
        entrada >> tempo;
        produtos.push_back(Produto(i, tempo, PRODUTO_DISPONIVEL));
    }

    for (int i = 0; i < nProdutos; i++) {
        for (int j = 0; j < nProdutos; j++) {
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
    for (size_t j = 0; j < produtos.size(); j++){
        if (produtos[j].tempo > maiorCustoEncontrado && produtos[j].estado == PRODUTO_DISPONIVEL){
            maiorCustoEncontrado = produtos[j].tempo;
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
    for (size_t i = 0; i < linhas.size(); i++){
        if (linhas[i].getTempoTotal() < menorCustoDeLinha){
            menorCustoDeLinha = linhas[i].getTempoTotal();
            indiceDaMenorLinha = i;
        }
    }
    return linhas.at(indiceDaMenorLinha);
}

/**
 * Retorna uma referência do objeto da linha mais custosa em relação
 * ao tempo de produção, dada uma lista (vector) de linhas
 * de produção
*/
Linha& maiorLinhaDeTodas(vector<Linha>& linhas){
    int maiorCustoDeLinha = -1, indiceDaMaiorLinha;
    for (size_t i = 0; i < linhas.size(); i++){
        if (linhas[i].getTempoTotal() > maiorCustoDeLinha){
            maiorCustoDeLinha = linhas[i].getTempoTotal();
            indiceDaMaiorLinha = i;
        }
    }
    return linhas.at(indiceDaMaiorLinha);
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
    
    for (int i = 0; i < mLinhas; i++)
        linhas.push_back(Linha(&matrizDeAdj, i));
    
    // Adicionando, em cada linha, o produto mais custoso disponível
    for (int i = 0; i < mLinhas; i++){
        int indiceDoMaiorProduto = maiorProduto(produtos); // Recuperando o índice produto mais custoso não adicionado
        linhas.at(i).pushProduto(produtos.at(indiceDoMaiorProduto)); // Adicionando o maior produto a linha atual
        produtosRestantes--; // Decrementando o contador de produtos disponíveis
    }

    int indiceDaMelhorTransicao; // Índice do produto que oferece o melhor custo de manutenção em relação ao último produto

    /**
     * Parte principal do algoritmo guloso.
     * Adiciona todos os produtos restantes de acordo com o critério guloso desenvolvido
     */
    while (produtosRestantes){
        Linha& menorLinha = menorLinhaDeTodas(linhas); // Recuperando o objeto da menor linha, que é modificado posteriormente
        Produto ultimoProduto = *menorLinha.produtos.back(); // Recuperando o objeto do último produto da menor linha
        int menorSoma = INT_MAX; // Auxiliar para achar a menor soma: tempo + manutenção

        /**
         * Recupera o produto candidato que cumpre o critério guloso.
         * Faz isso percorrendo a linha da matriz de tempos de manutenção (matriz de adjacência)
         * correspondente ao último produto da menor linha.
        */
        for (size_t i = 0; i < produtos.size(); i++){
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
        menorLinha.pushProduto(produtos.at(indiceDaMelhorTransicao));

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

//realiza a troca de produtos
void trocaProduto(Linha& L1, Linha& L2, int indice_L1, int indice_L2)
{
    Produto* aux = L2.produtos.at(indice_L2);                //guarda produto a ser trocado

    L2.produtos.at(indice_L2) = L1.produtos.at(indice_L1);  //linha destinataria recebe novo produto do remetente
    L1.produtos.at(indice_L1) = aux;                            //linha remetente recebe antigo produto do destinatario

    L1.produtos.at(indice_L1)->estado = L1.getIndiceLinha();//atualiza indice de linha dos produtos
    L1.recalculaTempoTotal();

    if(&L1 != &L2){  //caso seja feita trocas entre linhas diferentes
        L2.recalculaTempoTotal();
        L2.produtos.at(indice_L2)->estado = L2.getIndiceLinha();
    }
}

bool swap2(vector<Linha>& solucao_vnd)
{
    Linha& linhaMaior = maiorLinhaDeTodas(solucao_vnd);
    int maior_tempo = linhaMaior.getTempoTotal();
    bool melhorou = false;
    int melhor_produto_LMa;     //indice da melhor troca da linhaMaior
    int melhor_produto_LMe;     //indice da melhor troca da linhaMenor
    int melhor_linha;           //indice da melhor linhaMenor


    for (size_t l = 0; l < mLinhas; l++)
    {
        if (l == linhaMaior.getIndiceLinha()) 
            continue;
        
        Linha& linhaMenor = solucao_vnd[l];

        for (size_t i = 0; i < linhaMaior.produtos.size(); i++)
        {
            //realiza trocas entre todos os produtos da menor linha para procurar a melhor
            for (size_t j = 0; j < linhaMenor.produtos.size(); j++)
            {
                trocaProduto(linhaMaior, linhaMenor, i, j);
                int novo_tempo_LMa = linhaMaior.getTempoTotal();
                int novo_tempo_LMe = linhaMenor.getTempoTotal();

                //se apos a troca o tempo for melhor que o anterior em ambas as linhas, salva informacoes de troca e tempo
                if(novo_tempo_LMa < maior_tempo && novo_tempo_LMe < maior_tempo)
                {
                    melhorou = true;
                    melhor_produto_LMa = i;
                    melhor_produto_LMe = j;
                    melhor_linha = l;
                    
                    if (novo_tempo_LMa >= novo_tempo_LMe)
                        maior_tempo = novo_tempo_LMa;
                    else
                        maior_tempo = novo_tempo_LMe;
                }
                trocaProduto(linhaMaior, linhaMenor, i, j); //desfaz a troca
            }
        }
    }
    
    if (melhorou)
        trocaProduto(linhaMaior, solucao_vnd[melhor_linha], melhor_produto_LMa, melhor_produto_LMe);
    
    return melhorou;
}

/**
 * Imprime os produtos nas linhas de produção (representação da solução)
 * Retorna o valor da solução
*/
int imprimirSolucao(vector<Linha>& linhas){
    cout << "\n";
    for (size_t i = 0; i < linhas.size(); i++){
        cout << "\tLinha de producao " << i+1 << ": ";
        
        for (size_t j = 0; j < linhas.at(i).produtos.size(); j++){
            cout << "P" << linhas.at(i).produtos.at(j)->indice+1;
            if (j < linhas.at(i).produtos.size()-1){
                cout << " -> ";
            }
        }
        cout << " | Custo = " << linhas.at(i).getTempoTotal() << endl;
    }
    //cout << "\tCusto da maior linha (funcao objetivo): " << maiorLinhaDeTodas(linhas).getTempoTotal() << endl;
    //cout << "\n";
    return maiorLinhaDeTodas(linhas).getTempoTotal();
}

bool swap1(Linha& LE){

    int melhorValor = LE.getTempoTotal();;
    int prodI = 0 , prodJ = 0;
    bool melhorou = false;
    
    for(size_t i = 0 ; i < LE.produtos.size()-1; i++){
        for(size_t j = i+1; j < LE.produtos.size(); j++){
            
            trocaProduto(LE, LE, i, j);
            if(melhorValor > LE.getTempoTotal()){
                prodI = i;
                prodJ = j;
                melhorValor = LE.getTempoTotal();
                melhorou = true;
            }
            trocaProduto(LE, LE, i, j);
        }
    }
    
    if (melhorou)
        trocaProduto(LE, LE, prodI, prodJ);
    
    return melhorou;
}

vector<Linha> VND(int numR, vector<Linha>& solucao){
    vector<Linha> vndSolucao = solucao;
    int r = numR;
    int k = 1;
    bool melhor;

    while (k <= r){

        switch (k){
            case 1:
                //cout << "\nSWAP1!" << endl;
                melhor = swap1(maiorLinhaDeTodas(vndSolucao));
                //imprimirSolucao(vndSolucao); //! teste
                break;
            case 2:
                //cout << "\nSWAP1 Falhou! Usando SWAP2!" << endl;
                melhor = swap2(vndSolucao);
                //imprimirSolucao(vndSolucao); //! teste
                break;
        }

        if(melhor){
            k = 1;
        }else{
            k += 1;
        }
    }
    return vndSolucao;
}

int main() {
    for (auto& par : arquivos){
        string nomeEntrada = par.first;
        int valorOtimo = par.second;

        // Lendo o arquivo de entrada
        string caminhoDaEntrada = "./auxiliar/instancias/" + par.first;
        lerEntrada(caminhoDaEntrada);
        cout << "Instancia: " << par.first << " | Solucao Otima: " << par.second << endl;

        // Execução e dados do algoritmo guloso
        cout << "\n\tSolucao da Heuristica Construtiva:" << endl;
        auto inicio = chrono::high_resolution_clock::now();
        vector<Linha> solucaoGulosa = heuristicaConstrutiva();
        auto fim = chrono::high_resolution_clock::now();
        auto duracao = std::chrono::duration_cast<std::chrono::milliseconds>(fim - inicio);
        int valorDaSolucao = imprimirSolucao(solucaoGulosa);
        cout << "\tValor da solucao encontrada: " << valorDaSolucao << endl;
        cout << "\tTempo de execucao: " << duracao.count() << " ms" << endl;
        cout << "\tGAP: " << ((valorDaSolucao - valorOtimo) / valorOtimo) * 100.00 << endl;

        cout << "\n";

        // Execução e dados do algoritmo VND
        cout << "\tSolucao do VND:" << endl;
        inicio = chrono::high_resolution_clock::now();
        vector<Linha> solucaoVND = VND(2, solucaoGulosa);
        fim = chrono::high_resolution_clock::now();
        duracao = std::chrono::duration_cast<std::chrono::milliseconds>(fim - inicio);
        valorDaSolucao = imprimirSolucao(solucaoVND);
        cout << "\tValor da solucao encontrada: " << valorDaSolucao << endl;
        cout << "\tTempo de execucao: " << duracao.count() << " ms" << endl;
        cout << "\tGAP: " << ((valorDaSolucao - valorOtimo) / valorOtimo) * 100.00 << endl;
        
        // Separador da exibição de soluções
        cout << "\n\n";
        for (int j = 0; j < 40; j++){
            cout << "+-+";
        }
        cout << "\n\n";
    }
    return 0;
}
