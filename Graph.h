#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED
#include "Node.h"
#include <fstream>
#include <stack>
#include <list>

using namespace std;

class Graph
{
    public:
        // Construtor & Destrutor
        Graph(int order, bool directed, bool weighted_edge, bool weighted_node);
        ~Graph();

        //construtor subgrafo
        Graph(int order, int* listIdNodes, bool directed, bool weighted_edge, bool weighted_node);

        //Getters
        int getOrder();
        int getNumberEdges();
        bool getDirected();
        bool getWeightedEdge();
        bool getWeightedNode();
        Node* getFirstNode();
        Node* getLastNode();

        //Other methods
        void insertNode(int id);
        void insertEdge(int id, int target_id, float weight);
        bool searchNode(int id);
        Node* getNode(int id);

        //methods phase1
        void topologicalSorting();

        void breadthFirstSearch(ofstream& output_file);

        //Funções (A) (B) e auxiliares
        Graph* getVertexInduced(int* listIdNodes, int OrderSubGraph);
        Graph* getVertexInducedDirect(int idSource);
        Graph* getVertexInducedIndirect(int idSource);
        void auxVertexInduced(Node* node);

        //Funções (C) e auxiliares
        float dijkstra(int idSource, int idTarget);
        void AuxDijkstra(Node* node, Node* nodeTarget, float* Dist);
        bool AuxDijkstraVazio(bool* NVisitado);
        Node* AuxDijkstraSeleciona(float* Dist, bool* NVisitado,  Node* source);

        Graph* agmKuskal();

        Graph* agmPrim();

        float floydWarshall(int idSource, int idTarget);


        

        //minhas funções
        void Print_Ad_list();

    private:
    
        int insert_aux;
        int order;
        int number_edges;
        bool directed;
        bool weighted_edge;
        bool weighted_node;
        Node* first_node;
        Node* last_node;
};

#endif // GRAPH_H_INCLUDED
