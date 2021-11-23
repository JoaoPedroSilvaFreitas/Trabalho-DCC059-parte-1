#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include "Graph.h"
#include "Node.h"
using namespace std;


/*ESSA FUNÇÂO PROVAVELMENTE SERÁ REMOVIDA

Graph* Cria_Grafo(Graph* grafo, int order, bool directed, bool weighted_edge, bool weighted_node)
{
    int n_edges, target_id;
    float edge_weight;
    grafo = new Graph(order, directed, weighted_edge, weighted_node);

    //Insere nós no grafo
    for(int i = 0; i < order; i++)
    {
        grafo->insertNode(i);
    }
    
    //insere arestas no grafo
    for(int i = 0; i < order; i++)
    {
        cout << "Node [" << i << "] number of edges:";
        cin >> n_edges;
        for(int j = 0; j < n_edges; j++)
        {
            if(weighted_edge == true)
            {
                cout << "edge from " << i << " to:";
                cin >> target_id;
                cout << "edge weight:";
                cin >> edge_weight;
                grafo->insertEdge(i,target_id,edge_weight);
            }
            else
            {
                cout << "edge from " << i << " to:";
                cin >> target_id;
                grafo->insertEdge(i,target_id,1);
            }                 
        }
    }

    //imprime a lista apos sua criação
    grafo->Print_Ad_list();
}

*/


Graph* Cria_Grafo(ifstream& input_file, int directed, int weightedEdge, int weightedNode)
{

    //Variáveis para auxiliar na criação de arestas no Grafo
    int idNodeSource;
    int idNodeTarget;
    int order;

    //Pegando a ordem do grafo
    input_file >> order;

    //Criando objeto grafo
    Graph* graph = new Graph(order, directed, weightedEdge, weightedNode);

    //Caso o grafo não tenha peso nos nós e arestas
    if(!graph->getWeightedEdge() && !graph->getWeightedNode())
    {
        while(input_file >> idNodeSource >> idNodeTarget) 
        {
            graph->insertEdge(idNodeSource, idNodeTarget, 1);
        }

    }else if(graph->getWeightedEdge() && !graph->getWeightedNode() )//Caso o grafo tenha peso nas arestas
    {

        float edgeWeight;

        while(input_file >> idNodeSource >> idNodeTarget >> edgeWeight) 
        {

            graph->insertEdge(idNodeSource, idNodeTarget, edgeWeight);

        }

    }else if(graph->getWeightedNode() && !graph->getWeightedEdge())//Caso o grafo tenha peso nos nós
    {

        float nodeSourceWeight, nodeTargetWeight;

        while(input_file >> idNodeSource >> nodeSourceWeight >> idNodeTarget >> nodeTargetWeight) 
        {

            graph->insertEdge(idNodeSource, idNodeTarget, 1);
            graph->getNode(idNodeSource)->setWeight(nodeSourceWeight);
            graph->getNode(idNodeTarget)->setWeight(nodeTargetWeight);

        }

    }else if(graph->getWeightedNode() && graph->getWeightedEdge())//Caso o grafo tenha peso nos nós e arestas
    {

        float nodeSourceWeight, nodeTargetWeight, edgeWeight;

        while(input_file >> idNodeSource >> nodeSourceWeight >> idNodeTarget >> nodeTargetWeight >> edgeWeight) 
        {

            graph->insertEdge(idNodeSource, idNodeTarget, edgeWeight);
            graph->getNode(idNodeSource)->setWeight(nodeSourceWeight);
            graph->getNode(idNodeTarget)->setWeight(nodeTargetWeight);

        }
    }

    return graph;
}


/* ESSA FUNÇÃO PROVAVELMENTE É DESNESSESSÁRIA
Graph* ler_Arquivo(ifstream& input_file, int directed, int weightedEdge, int weightedNode)
{
    //Variáveis para auxiliar na criação dos nós no Grafo
    int idNodeSource;
    int idNodeTarget;
    int order;

    //Pegando a ordem do grafo
    input_file >> order;

    //Criando grafo
    Graph* graph;
    graph = Cria_Grafo(input_file, directed, weightedEdge, weightedNode);
    //Graph* graph = new Graph(order, directed, weightedEdge, weightedNode);

    //Lê arquivo de entrada e insere arestas
    while(input_file >> idNodeSource >> idNodeTarget) 
    {
        graph->insertEdge(idNodeSource, idNodeTarget, 0);
    }

    return graph;
}
*/


char MainMenu(Graph* grafo)
{
    char opt = 'd';
    cout << "----------------Main Menu-----------------" << endl;
    cout << "1- Subgrafo induzido por conjunto de vertices" << endl;
    cout << "2- Caminho Minimo entre dois vertices - Dijkstra" << endl;
    cout << "3- Caminho Minimo entre dois vertices - Floyd" << endl;
    cout << "4- Arvore Geradora Minima de Prim" << endl;
    cout << "5- Arvore Geradora Minima de Kruskal" << endl;
    cout << "6- Imprimir caminhamento em largura" << endl;
    cout << "7- Imprimir ordenacao topologica" << endl;
    cout << "8- Algoritmo Guloso" << endl;
    cout << "9- Algoritmo Guloso Randomizado " << endl;
    cout << "A- Algoritmo Guloso Randomizado Reativo" << endl;
    cout << "B- Imprime lista de adjacencia" << endl;
    cout << "0- Sair" << endl;
    cout << "------------------------------------------" << endl;
    cin >> opt;
    return opt;
}


void AuxMainMenu(Graph* grafo)
{
    char opt = 'z';
        while(opt != '0')
        {
            opt = MainMenu(grafo);
            if(opt == '1')
            {

            }
            if(opt == '2')
            {
                
            }
            if(opt == '3')
            {
                
            }
            if(opt == '4')
            {
                
            }
            if(opt == '5')
            {
                
            }
            if(opt == '6')
            {
                
            }
            if(opt == '7')
            {
                
            }
            if(opt == '8')
            {
                
            }
            if(opt == '9')
            {
                
            }
            if(opt == 'a')
            {
                
            }
            if(opt == 'b')
            {
                grafo->Print_Ad_list();
            }
            else if(opt == '0')
            {
                return;
            }
        }
}


int main(int argc, const char*argv[])
{
    //Verifica se foram passados todos parametros
    if (argc != 6) 
    {
        cout << "ERROR: Expecting: ./<program_name> <input_file> <output_file> <directed> <weighted_edge> <weighted_node> " << endl;
        return 1;
    }

    //Declara duas strings que recebem nome do programa e o nome do arquivo de entrada
    string program_name(argv[0]);
    string input_file_name(argv[1]);

    //"Declara" arquivo de entrada e saida
    ifstream input_file;
    ofstream output_file;

    //Abre o aquivo de entrada e o arquivo de saida
    input_file.open(argv[1], ios::in);
    output_file.open(argv[2], ios::out | ios::trunc);

    //Declara um ponteiro de grafo
    Graph* grafo;

    //Verifica se o arquivo de entrada foi aberto com sucesso
    if(input_file.is_open())
    {
        //Lê o arquivo de entrada (atoi converte string em inteiro)
        grafo = Cria_Grafo(input_file, atoi(argv[3]), atoi(argv[4]), atoi(argv[5]));
    }
    else
    {
        cout << "Unable to open " << argv[1];
    } 

    //ADICIONAR MENU AQUI
    AuxMainMenu(grafo);

   //Fechando arquivos de entrada e saída
    input_file.close();
    output_file.close();
}
