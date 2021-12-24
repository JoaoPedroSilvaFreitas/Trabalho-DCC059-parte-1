#include "Edge.h"
#include <iostream>

using namespace std;

// Constructor & Destructor
Edge::Edge(int target_id)
{
    this->target_id = target_id;
    this->next_edge = nullptr;
    this->weight = 0;
}

Edge::Edge(int source_id, int target_id, float weight)//Contrutor para auxiliar na fila de arestas
{
    this->source_id = source_id;
    this->target_id = target_id;
    this->weight = weight;
    this->next_edge = nullptr;
    this->weight = 0;
}

Edge::~Edge()
{
    if (this->next_edge != nullptr)
    {
        delete this->next_edge;
        this->next_edge = nullptr;
    }
}

// Getters
int Edge::getTargetId()
{
    return this->target_id;
}

Edge* Edge::getNextEdge()
{
    return this->next_edge;
}

float Edge::getWeight()
{
    return this->weight;
}

int Edge::getSourceId()
{
    return this->source_id;
}

// Setters
void Edge::setNextEdge(Edge* edge)
{
    this->next_edge = edge;
}

void Edge::setWeight(float weight)
{
    this->weight = weight;
}
