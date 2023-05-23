#include <iostream>
#include <fstream>
#include <vector>
#include "Linha.cpp"
#include "Produto.cpp"
#include <climits>
#include <algorithm>

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
 * Retorna uma referência do objeto da linha mais custosa em relação
 * ao tempo de produção, dada uma lista (vector) de linhas
 * de produção
*/
Linha& maiorLinhaDeTodas(vector<Linha>& linhas){
    int maiorCustoDeLinha = -1, indiceDaMaiorLinha;
    for (int i = 0; i < linhas.size(); i++){
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

//realiza a troca de produtos entre duas linhas
void trocaProduto(Linha& de, Linha& para, int indice_de, int indice_para)
{
    Produto aux = para.produtos.at(indice_para);                //guarda produto a ser trocado

    para.produtos.at(indice_para) = de.produtos.at(indice_de);  //linha destinataria recebe novo produto do remetente
    de.produtos.at(indice_de) = aux;                            //linha remetente recebe antigo produto do destinatario

    para.produtos.at(indice_para).estado = para.getIndiceLinha();//atualiza indice de linha dos produtos
    de.produtos.at(indice_de).estado = de.getIndiceLinha();
}

/*  BUG:
terminate called after throwing an instance of 'std::out_of_range'
what():  vector::_M_range_check: __n (which is 663590400) >= this->size() (which is 3)
Aborted
*/
void swap2(vector<Linha>& solucao_vnd)
{
    Linha& linhaMaior = maiorLinhaDeTodas(solucao_vnd);
    Linha& linhaMenor = menorLinhaDeTodas(solucao_vnd);
    int produtoPesado = maiorTransicao(linhaMaior);
    int maior_tempo = linhaMaior.getTempoParcial(produtoPesado);
    bool melhorou = false;
    int melhor_troca;   //indice da melhor troca na linhaMenor

    //realiza trocas entre todos os produtos da menor linha para procurar a melhor
    for (size_t i = 0; i < linhaMenor.produtos.size(); i++)
    {
        trocaProduto(linhaMaior, linhaMenor, produtoPesado, i);
        int novo_tempo_LMa = linhaMaior.getTempoParcial(produtoPesado);
        int novo_tempo_LMe = linhaMenor.getTempoParcial(i);

        //se apos a troca o tempo for melhor que o anterior em ambas as linhas, salva informacoes de troca e tempo
        if(novo_tempo_LMa < maior_tempo && novo_tempo_LMe < maior_tempo)
        {
            melhorou = true;
            melhor_troca = i;
            if (novo_tempo_LMa >= novo_tempo_LMe)
                maior_tempo = novo_tempo_LMa;
            else
                maior_tempo = novo_tempo_LMe;
        }
        trocaProduto(linhaMenor, linhaMaior, i, produtoPesado); //desfaz a troca
    }
    
    if (melhorou)
    {
        trocaProduto(linhaMaior, linhaMenor, produtoPesado, melhor_troca);
        linhaMaior.recalculaTempoTotal();
        linhaMenor.recalculaTempoTotal();
    }
}

void imprimirSolucao(vector<Linha>& linhas){
    cout << "\n";
    for (int i = 0; i < linhas.size(); i++){
        cout << "Linha de producao " << i+1 << ": ";
        
        for (int j = 0; j < linhas.at(i).produtos.size(); j++){
            cout << "Produto " << linhas.at(i).produtos.at(j).indice+1;
            if (j < linhas.at(i).produtos.size()-1){
                cout << " -> ";
            }
        }
        cout << " | Custo = " << linhas.at(i).getTempoTotal() << endl;
    }
    cout << "Custo da maior linha (funcao objetivo): " << maiorLinhaDeTodas(linhas).getTempoTotal() << endl;
    cout << "\n";
}

Linha swap1(Linha LE){

    int novoValorF = LE.getTempoTotal();;
    int prodI = 0 , prodJ = 0;
    
    Linha teste = LE;
    /*
    cout << "Tempo total incial da linha antes do SWAP: " << novoValorF << endl;
    cout << "Quantidade de produtos da linha: " << LE.produtos.size() << endl;

    for(int i = 0; i < LE.produtos.size(); i++){
        cout << "[" << i << "] = " << LE.produtos.at(i).indice << " " ; 
    }

    cout << "\n" ;
    */
    for(int i = 0 ; i < LE.produtos.size()-1; i++){
        for(int j = i+1; j < LE.produtos.size(); j++){
            teste = LE;
            iter_swap(teste.produtos.begin() + i, teste.produtos.begin() + j);

            /*for(int i = 0; i < teste.produtos.size(); i++){
                cout << "[" << i << "] = " << teste.produtos.at(i).indice << " " ; 
            }*/

            teste.recalculaTempoTotal();

            //cout << " Novo Valor do Teste[" << i <<"]: " << teste.getTempoTotal() << endl;

            if(novoValorF > teste.getTempoTotal()){
                prodI = i;
                prodJ = j;
                novoValorF = teste.getTempoTotal();
            }
        }
    }
    //cout << "\nTempo total final da linha depois do SWAP: " << novoValorF << endl;
    iter_swap(LE.produtos.begin() + prodI, LE.produtos.begin() + prodJ);
    LE.recalculaTempoTotal();

    return LE;
}

vector<Linha> VND(int numR, vector<Linha>& solucao){
    
    vector<Linha> vndSolucao = solucao;//fazemos uma copia que vai ser retornada apos o fim do VND
    
    int r = numR;
    int k = 1;
    //int maiorLinha = -1;
    //int indiceMaiorLinha = -1;

    Linha soluF = solucao.at(0); //so pra iniciar a (s')

    while (k <= r){
        Linha& maiorLinha = maiorLinhaDeTodas(vndSolucao);

        // k escolhe qual movimento de vizinhança é chamado
        switch (k){
            case 1:
                soluF = swap1(maiorLinhaDeTodas(vndSolucao));
                cout << "\nSWAP1!" << endl;
                break;
            case 2:
                swap2(vndSolucao);
                cout << "\nSWAP1 Falhou! Usando SWAP2!" << endl;
                //swap2(vndSolucao);
                break;
        }

        cout << "\nSoluf: " << soluF.getTempoTotal() << " vs VND: " << maiorLinha.getTempoTotal() << endl;
        if(soluF.getTempoTotal() < maiorLinha.getTempoTotal()){
            maiorLinha = soluF;
            maiorLinha.recalculaTempoTotal();
            k = 1;
        }else{
            k += 1;
        }
    }
    return vndSolucao;
}

int main(int argc, char const *argv[]) {
    // Lendo o arquivo de entrada
    lerEntrada();

    vector<Linha> solucao = heuristicaConstrutiva();
    imprimirSolucao(solucao);

    vector<Linha> vndsolution = VND(2, solucao);
    imprimirSolucao(vndsolution);

    return 0;
}
