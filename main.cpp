#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include "Graph.h"
#include "Node.h"
using namespace std;

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

Graph* ler_Arquivo(ifstream& input_file, int directed, int weightedEdge, int weightedNode)
{

}

int main(int argc, const char*argv[])
{
    if (argc != 3) 
    {
        cout << "ERROR: Expecting: ./<program_name> <input_file> <output_file> <directed> <weighted_edge> <weighted_node> " << endl;
        return 1;
    }

    string program_name(argv[0]);
    string input_file_name(argv[1]);

    ifstream input_file;
    ofstream output_file;
    input_file.open(argv[1], ios::in);
    output_file.open(argv[2], ios::out | ios::trunc);

    Graph* grafo;

    if(input_file.is_open())
    {

        grafo = ler_Arquivo(input_file, atoi(argv[3]), atoi(argv[4]), atoi(argv[5]));

    }else
        cout << "Unable to open " << argv[1];

   //Fechando arquivo de entrada
    input_file.close();

    //Fechando arquivo de saída
    output_file.close();
}