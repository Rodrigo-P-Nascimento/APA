#include <iostream>
#include <climits>
#include "EDP.h"

using namespace std;

EDP::EDP(int n, int m, vector<Produto>* custoTempo){

    maiorValor=0;//valor de inicio 
    nProd = n;
    mLinha = m;
    valorPorLinha.resize(n, 0); //inicilializar o valor do tempo total de cada linha com 0
    produtoAdicionado.resize(n, 1);//todos os produtos estão disponiveis
    estrutura.resize(m);//iniciamos a estrutura com 0
}

void EDP::guloso(vector<Produto>* custoTempo, vector<vector<int>>* matrix){

    /*
        COMO FUNCIONA O NOSSO ALGORITMO GULOSO?

        Ele foi dividido em dois passos:

        1º Ele vai adicionar a todas as m linhas o produto que tem o maior valor de custo de tempo de produção
        2º Ele vai verificar qual linha dentre as m linhas, tem o menor valor de custo e vai adicionar a ela o produto que oferece o menor custo de transição e menor custo de fabricação.

        Ele fica nesse loop do 2º passo até que não tenha mais produtos disponiveis, ou seja, todos os produtos estão em alguma linha.
        O intuito dessa logica é TENTAR fazer com que as linhas ficam mais ou menos que os mesmo valores de custo de produção!
    
    */

    int maiorTempo=0;//Ela guarda o maior custo de produção de um produto
    int indice=0;//Ela guarda o indice do produto que oferece maior custo
    int produtosRestantes = nProd;

    //Esse |for| abaixo, serve para colocar em todas as linhas o produto que consume mais tempo.
    for(int i=0; i < mLinha; i++){

        maiorTempo = 0;//sempre que inciamos uma nova interação do for esse dois valores são resetados
        indice = 0;

        //Esse |for| abaixo, serve para procurarmos dentres os produtos DISPONIVEIS qual deles oferece o maior tempo
        for(int j=0; j < nProd; j++){
            //Avaliamos se o custo do produto é maior do qe o valor da variavel "maiorTempo" e se ele está disponivel
            //Se sim, atualizamos o valor de "maiorTempo" e salvamos o seu indice.
            if(custoTempo->at(j).temp > maiorTempo && produtoAdicionado[j] != 0){
                maiorTempo = custoTempo->at(j).temp;
                indice = j;
            }
        }

        /* Depois de termos pego o produto com o maior custo, está na hora de adicionarmos ele na linha[i], como o i representa uma linha, cada linha só terá apenas um unico produto.

        Nos criamos um item do tipo Produto, com as informações do seu indice, referente ao seu ID unico do vector t de entrada e do seu custo de produção.
        */

        estrutura[i].push_back(Produto (indice, maiorTempo, false));
        valorPorLinha[i] = somaLinha(i, matrix);//Atualizamos o valo total de cada linha[i]
        produtoAdicionado[indice] = 0;//Para termos um controle de informações, setamos como 0 o valor referente ao indice do produto.
        produtosRestantes--;//Dimiunimos a quantidade de produtos disponiveis
    }

    int valorMenorLinha = 0; //Essa variavel vai guardar qual o valor da menor linha dentre as m linhas possiveis da nossa estrutura
    int indiceMenorLinha = 0;//Guarda o indice da menor linha possivél
    int indiceUltimo = -1;//Guarda o indice do ulitmo produto da menor linha
    int indiceDaMenorTransicao = -1;//Guarda o indice do produto que oferece o custo de transisasao de produto[x] para o produto[y]
    int menorsoma = 0;//Guarda o valor da menor soma de custo + transição

    //Enquanto tivermos produtos disponiveis esse laço |while| ficará ativo, a sua função é ir adicionando os produtos que ainda não foram adicionados a nenhuma das linhas de produção!
    while (produtosRestantes){
        valorMenorLinha = INT_MAX; //Setamos essa varaivel como INFINITA

        //Sempre atualizamos o valor das linhas para utilizar esses valores no proximos |for|
        for(int i =0; i < mLinha; i++){
            valorPorLinha[i] = somaLinha(i, matrix);
        }
        //Esse |for| pega o valor e o indice da menor linha dentre as m linhas possiveis
        for(int i=0; i < mLinha; i++){
            if(valorPorLinha[i] <= valorMenorLinha){
                valorMenorLinha = valorPorLinha[i];
                indiceMenorLinha = i;
            }
        }
        //pegar o indice da linha que contem o menor valor de tempo
        //cout << "Indice da linha com o menor valor: " << indiceMenorLinha << endl;
        //vamos pegar o ultimo produto dessa lista
        //olhar na matriz com é o produto que tem o menor custo de transição
        //colocar esse produto na linha x
        //refazer a soma total
        
        indiceUltimo = estrutura[indiceMenorLinha].back().ind;//Pegamos o indice do ultimo produto da linha escolhida
        indiceDaMenorTransicao = -1;//Sempre resetamos esse valor
        menorsoma = INT_MAX;//Setamos essa valor como INFINITO


        //Nesse |for| verificamos se ao pegarmos o custo de um produto[x] + o seu custo de transição, dado pela matriz, se esse custo é o menor custo dentre as nossa possibilidades entre os produtos ainda restantes.
        //Ao encontrar tal valor de soma, salvamos o indice do produto que oferece tal soma, e adiciomas ele na linha que tem o menor custo de produção
        for (int i=0; i < nProd; i++){
            if((matrix->at(indiceUltimo).at(i) + custoTempo->at(i).temp < menorsoma) && produtoAdicionado[i] != 0){
                indiceDaMenorTransicao = i;
                menorsoma = matrix->at(indiceUltimo).at(i) + custoTempo->at(i).temp;
            }
        }

        estrutura[indiceMenorLinha].push_back(Produto (indiceDaMenorTransicao, custoTempo->at(indiceDaMenorTransicao).temp, false));
        valorPorLinha[indiceMenorLinha] = somaLinha(indiceMenorLinha, matrix);//atualizamos o valor de custo total da linha que recebeu um novo produto
        produtoAdicionado[indiceDaMenorTransicao] = 0;//Já que um novo produto foi adicionado na linha, temos que marcar que ele já foi escolhido e não está mais disponivél
        produtosRestantes--;//Assim como também devemos dizer que a quantidade de produtos disponivies foi diminuida em -1
    } 

}

void EDP::imprimeTudo(){
    cout << "Maior valor de soma ateh agora: " << maiorValor << endl;
    cout << "Estrutura de Dados" << endl;

    
    for(int i=0; i < mLinha; i++ ){
        cout << "Elementos da Linha[" << i << "] = ";
        for(size_t j = 0; j < estrutura[i].size(); j++){
            cout << estrutura[i][j].temp << " ";
        }
        cout << "| valor da linha[" << i << "]: " << valorPorLinha[i];
        cout << "\n" ;
    }
    
}

int EDP::somaLinha(int l, vector<vector<int>>* matrix){
    int soma=0;

    for(int i=0; i < estrutura[l].size(); i++){
        soma = soma + estrutura[l][i].temp;

        //Esse |if| serve pra anailizarmos o seguinte se tiver um produto posterior ao atual então devmos somar o valor da transicação
        //Caso i atual seja exatamente o ultimo ele não funciona, já que temos valor de transicação para o ultimo produto.
        if(i < (estrutura[l].size()-1)){
            soma = soma + matrix->at(estrutura[l][i].ind).at(estrutura[l][i+1].ind);
        }
    }

    //Para atualizarmos o valor do maiorValor a cada chamada dessa função
    if(soma > maiorValor){
        maiorValor = soma;
    }

    return soma;
}