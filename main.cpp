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

int main(int argc, const char*argv[])
{
    Graph* grafo;
}