#include "Graph.h"
#include "Node.h"
#include "Edge.h"
#include <iostream>
#include <fstream>

using namespace std;

// Constructor & Destructor
Graph::Graph(int order, bool directed, bool weighted_edge, bool weighted_node)
{

    this->order = order;
    this->directed = directed;
    this->weighted_edge = weighted_edge;
    this->weighted_node = weighted_node;
    this->first_node = this->last_node = nullptr;
    this->number_edges = 0;

    //Função para inserir nós
    for(int i = 0; i < order; i++)
    {
        insertNode(i);
    }
}

Graph::~Graph()
{

    Node *next_node = this->first_node;

    while (next_node != nullptr)
    {
        next_node->removeAllEdges();
        Node *aux_node = next_node->getNextNode();
        delete next_node;
        next_node = aux_node;
    }
}

// Getters
int Graph::getOrder()
{
    return this->order;
}

int Graph::getNumberEdges()
{
    return this->number_edges;
}

Node *Graph::getFirstNode()
{
    return this->first_node;
}

Node *Graph::getLastNode()
{
    return this->last_node;
}

bool Graph::getDirected()
{

    return this->directed;
}

bool Graph::getWeightedEdge()
{

    return this->weighted_edge;
}

bool Graph::getWeightedNode()
{

    return this->weighted_node;
}

//insere nós
void Graph::insertNode(int id)
{
    Node* node = new Node(id);
    node->setNextNode(nullptr);

    if(last_node != nullptr) last_node->setNextNode(node);
    last_node = node;

    insert_aux++;
    if(insert_aux == 1) first_node = node;

    //PRECISO ALTERAR ISSO AQUI PORQUE O PESO É FORNECIDO PELO ARQUIVO .DOT
    /*
    if(getWeightedNode())
    {
        float NodeWeight;
        cout << "Node [" << node->getId() << "] weight:";
        cin >> NodeWeight;
        node->setWeight(NodeWeight);
    }
    else
    {
        node->setWeight(1);
    }
    */
}

//insere arestas
void Graph::insertEdge(int id, int target_id, float weight)
{
    Node* node = getNode(id);
    node->insertEdge(target_id,weight);
    this->number_edges++;
}

//procura um nó pelo id
bool Graph::searchNode(int id)
{
    Node* node;
    for(node = first_node; node != nullptr; node = node->getNextNode())
    {
        if(node->getId() == id)
        {
            return true;
        }
    }
    return false;
}

//Seleciona um nó pelo id
Node *Graph::getNode(int id)
{
    Node* node;
    for(node = first_node; node != nullptr; node = node->getNextNode())
    {
        if(node->getId() == id)
        {
            break;
        }
    }
    return node;
}

//Funções parte 1
void Graph::breadthFirstSearch(ofstream &output_file)
{
    
}

float Graph::floydMarshall(int idSource, int idTarget)
{
    
}

float Graph::dijkstra(int idSource, int idTarget)
{
    
}

void topologicalSorting()
{

}

void breadthFirstSearch(ofstream& output_file)
{

}

Graph* getVertexInduced(int* listIdNodes)
{

}

Graph* agmKuskal()
{

}

Graph* agmPrim()
{

}

//Minhas funções de teste
void Graph::Print_Ad_list()
{
    for(Node* aux = first_node; aux != nullptr; aux = aux->getNextNode())
    {
        cout << "[" << aux->getId() << "]-->";
        aux->Print_Edges();
        cout << endl;
    }
}
