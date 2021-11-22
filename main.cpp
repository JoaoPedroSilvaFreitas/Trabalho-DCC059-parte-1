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

   //Fechando arquivos de entrada e saída
    input_file.close();
    output_file.close();
}