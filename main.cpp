#include <iostream>
#include <fstream>
#include <vector>
#include "Linha.cpp"
#include "Produto.cpp"
#include <climits>
#include <algorithm>

#define PATH "./auxiliar/instancias/n15m4_A.txt"

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
    for (size_t i = 0; i < linhas.size(); i++){
        Linha linhaAtual = linhas.at(i);
        if (linhaAtual.getTempoTotal() < menorCustoDeLinha){
            menorCustoDeLinha = linhaAtual.getTempoTotal();
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
        Linha linhaAtual = linhas.at(i);
        if (linhaAtual.getTempoTotal() > maiorCustoDeLinha){
            maiorCustoDeLinha = linhaAtual.getTempoTotal();
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

    //int ultimoProduto; // Índice do último produto da menor linha
    int indiceDaMelhorTransicao; // Índice do produto que oferece o melhor custo de manutenção em relação ao último produto
    //int menorSoma = 0; // Auxiliar para a menor soma: custo do produto + tempo de manutenção

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

void imprimirSolucao(vector<Linha>& linhas){
    cout << "\n";
    for (size_t i = 0; i < linhas.size(); i++){
        cout << "Linha de producao " << i+1 << ": ";
        
        for (size_t j = 0; j < linhas.at(i).produtos.size(); j++){
            cout << "Produto " << linhas.at(i).produtos.at(j)->indice+1;
            if (j < linhas.at(i).produtos.size()-1){
                cout << " -> ";
            }
        }
        cout << " | Custo = " << linhas.at(i).getTempoTotal() << endl;
    }
    cout << "Custo da maior linha (funcao objetivo): " << maiorLinhaDeTodas(linhas).getTempoTotal() << endl;
    cout << "\n";
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

        if(k == 1){     //Se o k for igual a 1 temos que usar o SWAP1
            cout << "\nSWAP1!" << endl;
            melhor = swap1(maiorLinhaDeTodas(vndSolucao));
            imprimirSolucao(vndSolucao); //! teste
        }else if(k == 2){
            cout << "\nSWAP1 Falhou! Usando SWAP2!" << endl;
            melhor = swap2(vndSolucao);
            imprimirSolucao(vndSolucao); //! teste
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
    // Lendo o arquivo de entrada
    lerEntrada();

    vector<Linha> solucao = heuristicaConstrutiva();
    imprimirSolucao(solucao);

    vector<Linha> vndsolucao = VND(2, solucao);
    imprimirSolucao(vndsolucao);

    return 0;
}
