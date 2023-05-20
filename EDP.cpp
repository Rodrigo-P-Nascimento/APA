#include "EDP.h"

EDP::EDP(int n, int m, vector<Produto>* produtos, vector<vector<int>>* matriz){

    tempo_de_solucao = 0;
    nProd = n;
    mLinha = m;
    this->produtos = produtos;
    matrizDeAdj = matriz;

    for (size_t i = 0; i < mLinha; i++)     //inicializa as linhas com o construtor da classe Linha
    {
        linhas_producao.push_back(Linha(matrizDeAdj));
    }
}

EDP::~EDP()
{

}

void EDP::guloso(){

    /*
        COMO FUNCIONA O NOSSO ALGORITMO GULOSO?

        Ele foi dividido em dois passos:

        1º Ele vai adicionar a todas as m linhas o produto que tem o maior valor de custo de tempo de produção
        2º Ele vai verificar qual linha dentre as m linhas, tem o menor valor de custo e vai adicionar a ela o produto que oferece o menor custo de transição e menor custo de fabricação.

        Ele fica nesse loop do 2º passo até que não tenha mais produtos disponíveis, ou seja, todos os produtos estão em alguma linha.
        O intuito dessa lógica é TENTAR fazer com que as linhas ficam mais ou menos que os mesmo valores de custo de produção!
    
    */

    int produtosRestantes = nProd;

    //Esse |for| abaixo, serve para colocar em todas as linhas o produto que consome mais tempo.
    for(int i = 0; i < mLinha; i++){
        
        int maiorTempo = 0;     //Ela guarda o maior custo de produção de um produto
        int indiceDoMaior = 0;  //Ela guarda o indice do produto que oferece maior custo

        //Esse |for| abaixo, serve para procurarmos dentres os produtos DISPONIVEIS qual deles oferece o maior tempo
        for(int j = 0; j < nProd; j++){
            //Avaliamos se o custo do produto é maior do qe o valor da variavel "maiorTempo" e se ele está disponivel
            //Se sim, atualizamos o valor de "maiorTempo" e salvamos o seu indice.

            if(produtos->at(j).tempo > maiorTempo && produtos->at(j).disponivel){
                maiorTempo = produtos->at(i).tempo;
                indiceDoMaior = j;
            }
        }

        /* Depois de termos pego o produto com o maior custo, está na hora de adicionarmos ele na linha[i], como o i representa uma linha, cada linha só terá apenas um unico produto.

        Nos criamos um item do tipo Produto, com as informações do seu indice, referente ao seu ID unico do vector t de entrada e do seu custo de produção.
        */

        linhas_producao.at(i).push_Produto(&produtos->at(indiceDoMaior));
        produtos->at(indiceDoMaior).disponivel = false;
        produtosRestantes--;//Dimiunimos a quantidade de produtos disponiveis
    }

    int valorMenorLinha = 0; //Essa variavel vai guardar qual o valor da menor linha dentre as m linhas possiveis da nossa linhas_producao
    int indiceMenorLinha = 0;//Guarda o indice da menor linha possivél
    int indiceDaMenorTransicao = -1;//Guarda o indice do produto que oferece o custo de transisasao de produto[x] para o produto[y]
    int menorsoma = 0;//Guarda o valor da menor soma de custo + transição

    //Enquanto tivermos produtos disponiveis esse laço |while| ficará ativo, a sua função é ir adicionando os produtos que ainda não foram adicionados a nenhuma das linhas de produção!
    while (produtosRestantes){
        valorMenorLinha = INT32_MAX; //Setamos essa variável como INFINITA

        //Esse |for| pega o valor e o indice da menor linha dentre as m linhas possiveis
        for(int i=0; i < mLinha; i++){
            if(linhas_producao[i].get_tempo_total() <= valorMenorLinha){
                valorMenorLinha = linhas_producao[i].get_tempo_total();
                indiceMenorLinha = i;
            }
        }
        
        indiceDaMenorTransicao = -1; //Sempre resetamos esse valor
        menorsoma = INT32_MAX;       //Setamos essa valor como INFINITO

        //Nesse |for| verificamos se ao pegarmos o custo de um produto[x] + o seu custo de transição, dado pela matriz, se esse custo é o menor custo dentre as nossa possibilidades entre os produtos ainda restantes.
        //Ao encontrar tal valor de soma, salvamos o indice do produto que oferece tal soma, e adiciomas ele na linha que tem o menor custo de produção
        for (int i=0; i < nProd; i++){
            if((linhas_producao.at(indiceMenorLinha).get_tempo_parcial(&produtos->at(i)) < menorsoma) && produtos->at(i).disponivel){
                indiceDaMenorTransicao = i;
                menorsoma = linhas_producao.at(indiceMenorLinha).get_tempo_parcial(&produtos->at(i));
            }
        }

        linhas_producao[indiceMenorLinha].push_Produto(&produtos->at(indiceDaMenorTransicao));
        produtos->at(indiceDaMenorTransicao).disponivel = false; //Já que um novo produto foi adicionado na linha, temos que marcar que ele já foi escolhido e não está mais disponível
        produtosRestantes--; //Assim como também devemos dizer que a quantidade de produtos disponíveis foi diminuída em -1
    } 

    for (size_t i = 0; i < mLinha; i++)     //busca nas linhas de producao o maior tempo total
    {
        if(linhas_producao.at(i).get_tempo_total() > tempo_de_solucao)
            tempo_de_solucao = linhas_producao.at(i).get_tempo_total();
    }
    
}

void EDP::imprimirTudo(){
    cout << "Maior valor de soma ateh agora: " << tempo_de_solucao << endl;
    cout << "Estrutura de Dados" << endl;
    
    for(int i=0; i < mLinha; i++ ){
        cout << "Elementos da Linha[" << i << "] = " << linhas_producao[i].get_produtos();
        cout << "| valor da linha[" << i << "]: " << linhas_producao[i].get_tempo_total();
        cout << "\n" ;
    }
}