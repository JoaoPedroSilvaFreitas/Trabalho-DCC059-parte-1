#include "Graph.h"
#include "Node.h"
#include "Edge.h"
#include <iostream>
#include <fstream>
#include <vector>

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

//Construtor subgrafo vertice induzido
Graph::Graph(int order, int* listIdNodes, bool directed, bool weighted_edge, bool weighted_node)
{
    this->order = order;
    this->directed = directed;
    this->weighted_edge = weighted_edge;
    this->weighted_node = weighted_node;
    this->first_node = this->last_node = nullptr;
    this->number_edges = 0;

    //Adicionando os nós pela lista de Id
    for(int i = 0; i < order; i++)
    {
        insertNode(listIdNodes[i]);
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

    if(!getWeightedNode())
    {
        node->setWeight(1);
    }
}

//insere arestas
void Graph::insertEdge(int id, int target_id, float weight)
{
    //Caso o grafo seja direcionado
    if(getDirected())
    {
        Node* node = getNode(id);
        Node* node_target = getNode(target_id);
        node->insertEdge(target_id,weight);
        node->incrementOutDegree();
        node_target->incrementInDegree();
        this->number_edges++;
    }
    else
    {
        //Caso o grafo nao seja direcionado
        Node* node = getNode(id);
        Node* node_target = getNode(target_id);
        node->insertEdge(target_id,weight);
        node_target->insertEdge(id,weight);
        this->number_edges++;
    }
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

//FUNÇÕES PARTE 1

//Subgrafo induzido por subconjunto de vertices (A)
//Preciso concertar essa função porque esta adicionando duas vezes
Graph* Graph::getVertexInduced(int* listIdNodes, int OrderSubGraph)
{
    //Criando um novo grafo
    Graph* sub_grafo = new Graph(OrderSubGraph, listIdNodes, getDirected(), getWeightedEdge(), getWeightedNode());
    Node* node;
    if(!sub_grafo->getWeightedEdge() && !sub_grafo->getWeightedNode())//Caso o grafo não tenha peso nos nós e arestas
    {
        for(int i = 0; i < sub_grafo->getOrder(); i++)
        {
            node = getNode(listIdNodes[i]);

            for(int j = 0; j < sub_grafo->getOrder(); j++)
            {
                if(node->hasEdgeBetween(listIdNodes[j]))
                {
                    sub_grafo->insertEdge(listIdNodes[i],listIdNodes[j],1);
                }
            }
        }
    }
    else if(sub_grafo->getWeightedEdge() && !sub_grafo->getWeightedNode() )//Caso o grafo tenha peso nas arestas
    {
        Edge* edge;
        for(int i = 0; i < sub_grafo->getOrder(); i++)
        {
            node = getNode(listIdNodes[i]);
            for(int j = 0; j < sub_grafo->getOrder(); j++)
            {
                if(node->hasEdgeBetween(listIdNodes[j]))
                {
                    edge = node->getEdge(listIdNodes[j]);
                    sub_grafo->insertEdge(listIdNodes[i],listIdNodes[j],edge->getWeight());
                }
            }
        }
    }
    else if(sub_grafo->getWeightedNode() && !sub_grafo->getWeightedEdge())//Caso o grafo tenha peso nos nós
    {
        for(int i = 0; i < sub_grafo->getOrder(); i++)
        {
            node = getNode(listIdNodes[i]);
            for(int j = 0; j < sub_grafo->getOrder(); j++)
            {
                if(node->hasEdgeBetween(listIdNodes[j]))
                {
                    node->setWeight(getNode(listIdNodes[i])->getWeight());
                    sub_grafo->insertEdge(listIdNodes[i],listIdNodes[j],1);
                }
            }
        }
    }
    else if(sub_grafo->getWeightedNode() && sub_grafo->getWeightedEdge())//Caso o grafo tenha peso nos nós e arestas
    {
        Edge* edge;
        for(int i = 0; i < sub_grafo->getOrder(); i++)
        {
            node = sub_grafo->getNode(listIdNodes[i]);
            for(int j = 0; j < sub_grafo->getOrder(); j++)
            {
                if(node->hasEdgeBetween(listIdNodes[j]))
                {
                    edge = node->getEdge(listIdNodes[j]);
                    node->setWeight(getNode(listIdNodes[i])->getWeight());
                    sub_grafo->insertEdge(listIdNodes[i],listIdNodes[j],edge->getWeight());
                }
            }
        }
    }
    return sub_grafo;
}

//Caminho Minimo entre dois vertices - Dijkstra (C)
float Graph::dijkstra(int idSource, int idTarget)
{
    
}

//Caminho Minimo entre dois vertices - Floyd (D)
float Graph::floydMarshall(int idSource, int idTarget)
{
    
}

//Arvore Geradora Minima de Prim (E)
Graph* agmPrim()
{

}

//Arvore Geradora Minima de Kruskal (F)
Graph* agmKuskal()
{

}

void Graph::breadthFirstSearch(ofstream &output_file)
{
    
}

void breadthFirstSearch(ofstream& output_file)
{

}

void topologicalSorting()
{

}

//Imprime a lista de adjacencia
void Graph::Print_Ad_list()
{
    for(Node* aux = first_node; aux != nullptr; aux = aux->getNextNode())
    {
        cout << "[" << aux->getId() << "]-->";
        aux->Print_Edges();
        cout << endl;
    }
}
