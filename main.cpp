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
#include <iomanip>

// Valores das melhores soluções encontradas de cada instância
#define n10m2_A 763 // ótimo
#define n10m2_B 696 // ótimo
#define n15m3_A 1006
#define n15m3_B 1055
#define n15m4_A 688
#define n15m4_B 784
#define n29m4_A 1459
#define n29m4_B 1527
#define n29m6_A 1101
#define n29m6_B 1043
#define n40m5_A 447
#define n40m5_B 581
#define n52m5_A 7070
#define n52m5_B 1
#define n450m16_A 1
#define n500m10_A 1

using namespace std;

int nProdutos, mLinhas;
vector<vector<int>> matrizDeAdj; // Matriz com os tempos de manutenção, para receber da entrada de arquivo
vector<Produto> produtos; // Lista com a abstração completa dos produtos
map<string, int> arquivos = {
    //{"n10m2_A.txt", n10m2_A},
    //{"n10m2_B.txt", n10m2_B},
    //{"n15m3_A.txt", n15m3_A},
    //{"n15m3_B.txt", n15m3_B},
    //{"n15m4_A.txt", n15m4_A},
    //{"n15m4_B.txt", n15m4_B},
    //{"n29m4_A.txt", n29m4_A},
    //{"n29m4_B.txt", n29m4_B},
    //{"n29m6_A.txt", n29m6_A},
    {"n29m6_B.txt", n29m6_B},
    {"n40m5_A.txt", n40m5_A},
    {"n40m5_B.txt", n40m5_B},
    {"n52m5_A.txt", n52m5_A},
    {"n52m5_B.txt", n52m5_B},
    {"n450m16_A.txt", n450m16_A},
    {"n500m10_A.txt", n500m10_A}
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
    vector<Linha> linhas; // Estrutura de dados (floresta com matriz de adjacência) da solução, que inicia vazia
    
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

// Realiza a troca de produtos entre duas linhas
void trocarProdutos(Linha& L1, Linha& L2, int indiceProduto1, int indiceProduto2)
{
    Produto* aux = L2.produtos.at(indiceProduto2);                //guarda produto a ser trocado
    
    // Subtraindo o tempo dos produtos que estão sendo tirados de suas linhas originais
    L1.tempoTotal -= L1.getTempoParcial(indiceProduto1);
    L2.tempoTotal -= L2.getTempoParcial(indiceProduto2);

    // Trocando os produtos entre os objetos das linhas
    L2.produtos.at(indiceProduto2) = L1.produtos.at(indiceProduto1);  //linha destinataria recebe novo produto do remetente
    L1.produtos.at(indiceProduto1) = aux;                            //linha remetente recebe antigo produto do destinatario
    L1.produtos.at(indiceProduto1)->estado = L1.getIndiceLinha();//atualiza indice de linha dos produtos
    
    // Atualizando os tempos totais com os novos produtos de cada linha
    L1.tempoTotal += L1.getTempoParcial(indiceProduto1);
    L2.tempoTotal += L2.getTempoParcial(indiceProduto2);
}

/**
 * Movimento de Vizinhança que escolhe a melhor troca possível de produtos
 * entre duas linhas, sendo uma delas a que custa mais tempo de todas 
 * (a maior representa a função objetivo)
*/
bool SwapExterno(vector<Linha>& solucao_vnd)
{
    Linha& linhaMaior = maiorLinhaDeTodas(solucao_vnd);
    int maior_tempo = linhaMaior.getTempoTotal();
    bool melhorou = false;
    int melhor_produto_LMa;     //indice da melhor troca da linhaMaior
    int melhor_produto_LMe;     //indice da melhor troca da linhaMenor
    int melhor_linha;           //indice da melhor linhaMenor

    // Loop que percorre todas as linhas de produção
    for (size_t l = 0; l < solucao_vnd.size(); l++)
    {
        // Evitando que seja feita a busca de troca na maior linha
        if ((int)l == linhaMaior.getIndiceLinha()) 
            continue;
        
        Linha& linhaAtual = solucao_vnd.at(l);

        // Loop que percorre todos os produtos da maior linha
        for (size_t i = 0; i < linhaMaior.produtos.size(); i++)
        {
            // Loop que percorre a linha atual fazer trocas, procurando a menor (melhor) possível
            for (size_t j = 0; j < linhaAtual.produtos.size(); j++)
            {
                // Testando a troca com os produtos atuais
                trocarProdutos(linhaMaior, linhaAtual, i, j);
                int novo_tempo_LMa = linhaMaior.getTempoTotal();
                int novo_tempo_LMe = linhaAtual.getTempoTotal();

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
                trocarProdutos(linhaMaior, linhaAtual, i, j); // Desfaz a troca para verificar se mantém depois
            }
        }
    }
    
    // Verificando se deve fazer a melhor troca encontrada
    if (melhorou)
        trocarProdutos(linhaMaior, solucao_vnd.at(melhor_linha), melhor_produto_LMa, melhor_produto_LMe);
    
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
    return maiorLinhaDeTodas(linhas).getTempoTotal();
}

/**
 * Movimento de Vizinhança que escolhe a melhor troca possível de dois 
 * produtos na maior linha (a maior representa a função objetivo)
*/
bool SwapInterno(Linha& linha){

    int melhorValor = linha.getTempoTotal();
    int prodI = 0 , prodJ = 0;
    bool melhorou = false;
    
    for(size_t i = 0 ; i < linha.produtos.size()-1; i++){
        for(size_t j = i+1; j < linha.produtos.size(); j++){
            
            trocarProdutos(linha, linha, i, j);
            if(melhorValor > linha.getTempoTotal()){
                prodI = i;
                prodJ = j;
                melhorValor = linha.getTempoTotal();
                melhorou = true;
            }
            trocarProdutos(linha, linha, i, j);
        }
    }
    
    if (melhorou)
        trocarProdutos(linha, linha, prodI, prodJ);
    
    return melhorou;
}

vector<Linha> VND(int nMovimentos, vector<Linha>& solucao){
    vector<Linha> vndSolucao = solucao;
    int r = nMovimentos;
    int k = 1;
    bool melhorou;

    while (k <= r){

        switch (k){
            case 1:
                melhorou = SwapInterno(maiorLinhaDeTodas(vndSolucao));
                break;
            case 2:
                melhorou = SwapExterno(vndSolucao);
                break;
        }

        if(melhorou){
            k = 1;
        }else{
            k += 1;
        }
    }
    return vndSolucao;
}

string conversor(const chrono::nanoseconds& duracao)
{
    ostringstream oss;
    if (duracao >= chrono::seconds(1))
    {
        double duracaoS = static_cast<double>(duracao.count()) / 1'000'000'000.0;
        oss << fixed << setprecision(2) << duracaoS << " s";
    }
    else if (duracao >= chrono::milliseconds(1))
    {
        double duracaoMs = static_cast<double>(duracao.count()) / 1'000'000.0;
        oss << fixed << setprecision(2) << duracaoMs << " ms";
    }
    else if (duracao >= chrono::microseconds(1))
    {
        double duracaoMcs = static_cast<double>(duracao.count()) / 1'000.0;
        oss << fixed << setprecision(2) << duracaoMcs << " us";
    }
    else
    {
        double duracaoNs = static_cast<double>(duracao.count());
        oss << fixed << setprecision(2) << duracaoNs << " ns";
    }
    
    return oss.str();
}

int main() {
    for (auto& par : arquivos){
        string nomeEntrada = par.first;
        int valorOtimo = par.second;

        // Lendo o arquivo de entrada
        string caminhoDaEntrada = "./auxiliar/instancias/" + par.first;
        lerEntrada(caminhoDaEntrada);
        cout << "Instancia: " << par.first << " | Solucao Otima: " << par.second << endl;

        // Executando e obtendo os dados do algoritmo guloso
        cout << "\n\tSolucao da Heuristica Construtiva:" << endl;
        auto inicio = chrono::high_resolution_clock::now();
        vector<Linha> solucaoGulosa = heuristicaConstrutiva();
        auto fim = chrono::high_resolution_clock::now();
        auto duracao = chrono::duration_cast<std::chrono::nanoseconds>(fim - inicio);
        float valorDaSolucao = maiorLinhaDeTodas(solucaoGulosa).getTempoTotal();

        //imprimirSolucao(solucaoGulosa);
        cout << "\tValor da solucao encontrada: " << valorDaSolucao << endl;
        cout << "\tTempo de execucao: " << conversor(duracao) << endl;
        cout << "\tGAP: " << ((valorDaSolucao - valorOtimo) / valorOtimo) * 100 << endl;
        cout << "\n";

        // Executando e obtendo os dados do algoritmo VND
        cout << "\tSolucao do VND:" << endl;
        inicio = chrono::high_resolution_clock::now();
        vector<Linha> solucaoVND = VND(2, solucaoGulosa);
        fim = chrono::high_resolution_clock::now();
        duracao = std::chrono::duration_cast<std::chrono::nanoseconds>(fim - inicio);
        valorDaSolucao = maiorLinhaDeTodas(solucaoVND).getTempoTotal();
        
        //imprimirSolucao(solucaoVND);
        cout << "\tValor da solucao encontrada: " << valorDaSolucao << endl;
        cout << "Tempo de execucao: " << conversor(duracao) << endl;
        cout << "\tGAP: " << ((valorDaSolucao - valorOtimo) / valorOtimo) * 100.00 << endl;
        
        // Separador da exibição de soluções
        cout << "\n\n";
        for (int j = 0; j < 30; j++){
            cout << "++";
        }
        cout << "\n\n";
    }
    return 0;
}
