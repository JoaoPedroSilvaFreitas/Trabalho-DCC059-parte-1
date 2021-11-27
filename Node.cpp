#include "Node.h"
#include "Edge.h"
#include <iostream>

using namespace std;

// Constructor & Destructor
Node::Node(int id)
{

    this->id = id;
    this->in_degree = 0;
    this->out_degree = 0;
    this->weight = 0;
    this->first_edge = nullptr;
    this->last_edge = nullptr;
    this->next_node = nullptr;

    this->number_edges = 0;

};

Node::~Node()
{

    Edge* next_edge = this->first_edge;

    while(next_edge != nullptr)
    {
        Edge* aux_edge = next_edge->getNextEdge();
        delete next_edge;
        next_edge = aux_edge;
    }

};

// Getters
Edge* Node::getFirstEdge()
{
    return this->first_edge;
}

Edge* Node::getLastEdge()
{
    return this->last_edge;
}

int Node::getId()
{
    return this->id;
}

int Node::getInDegree()
{
    return this->in_degree;
}

int Node::getOutDegree()
{
    return this->out_degree;
}

float Node::getWeight()
{
    return this->weight;
}

Node* Node::getNextNode()
{
    return this->next_node;
}

int Node::getNumberEdges()
{
    return number_edges;
}

Edge* Node::getEdge(int target_id)
{
    for(Edge *auxEdge = this->first_edge; auxEdge != nullptr; auxEdge = auxEdge->getNextEdge())
    {
        if(auxEdge->getTargetId() == target_id)
        {
            return auxEdge;
        }     
    }
    return nullptr;
}

bool Node::getVisitado()
{
    return visitado;
}

// Setters
void Node::setNextNode(Node* next_node)
{
    this->next_node = next_node;
}

void Node::setWeight(float weight)
{
    this->weight = weight;
}

void Node::setVisitado(bool val)
{
    visitado = val;
}

//insere aresta
void Node::insertEdge(int target_id, float weight)
{
    if(this->first_edge != nullptr)
    {
        Edge* edge = new Edge(target_id);
        edge->setWeight(weight);
        this->last_edge->setNextEdge(edge);
        this->last_edge = edge;

    }
    else
    {
        this->first_edge = new Edge(target_id);
        this->first_edge->setWeight(weight);
        this->last_edge = this->first_edge;

    }
    this->number_edges++;
}

//remove todas arestas
void Node::removeAllEdges()
{
    if(this->first_edge != nullptr){

        Edge* next = nullptr;
        Edge* aux = this->first_edge;
        while(aux != nullptr)
        {
            next = aux->getNextEdge();
            delete aux;
        }
    }
    this->first_edge = this->last_edge = nullptr;
}

//Procura uma aresta pelo id
bool Node::searchEdge(int target_id)
{
    if(this->first_edge != nullptr)
    {
        for(Edge* aux = this->first_edge; aux != nullptr; aux = aux->getNextEdge())
            if(aux->getTargetId() == target_id)
                return true;

    }

    return false;

}

//Incrementa e decremente grau de um nó
void Node::incrementInDegree()
{
    this->in_degree++;
}

void Node::incrementOutDegree()
{
    this->out_degree++;
}

void Node::decrementInDegree()
{
    this->in_degree--;
}

void Node::decrementOutDegree()
{
    this->out_degree--;
}

//Procura aresta entre
Edge* Node::hasEdgeBetween(int target_id)
{

    for(Edge *auxEdge = this->first_edge; auxEdge != nullptr; auxEdge = auxEdge->getNextEdge())
    {
        if(auxEdge->getTargetId() == target_id)
            return auxEdge;
    }
    return nullptr;
}

//imprime arestas
void Node::Print_Edges()
{
    for(Edge* edge = first_edge; edge != nullptr; edge = edge->getNextEdge())
    {
        if(edge->getNextEdge() == nullptr)
        {
            cout << "[" << edge->getTargetId() << "]";
        }
        else
        cout << "[" << edge->getTargetId() << "]-->";
    }
}