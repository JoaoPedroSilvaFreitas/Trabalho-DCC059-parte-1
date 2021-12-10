#include "Graph.h"
#include "Node.h"
#include "Edge.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <limits>

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
    this->insert_aux = 0;

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
    this->insert_aux = 0;

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

//insere nó
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

//insere aresta
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


/*Subgrafo induzido por subconjunto de vertices (J) 
Erro quando grafo não é direcionado, a função adiciona a mesma aresta duas vezes*/
Graph* Graph::getVertexInduced(int* listIdNodes, int OrderSubGraph)
{
    //Criando um novo grafo
    Graph* sub_grafo = new Graph(OrderSubGraph, listIdNodes, getDirected(), getWeightedEdge(), getWeightedNode());
    Node* node;
    Node* aux;
    if(!sub_grafo->getWeightedEdge() && !sub_grafo->getWeightedNode())//Caso o grafo não tenha peso nos nós e arestas
    {
        for(int i = 0; i < sub_grafo->getOrder(); i++)
        {
            //aponta para o nó do grafo antigo que possui o mesmo id no grafo novo
            node = getNode(listIdNodes[i]);
            for(int j = 0; j < sub_grafo->getOrder(); j++)
            {
                aux = getNode(listIdNodes[j]);
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

//Auxiliar das funções (A) e (B)
void Graph::auxVertexInduced(Node* node)
{
    node->setVisitado(true);
    for(Node* aux = first_node; aux != nullptr; aux = aux->getNextNode())
    {
        if((aux->getVisitado() == false) && (node->searchEdge(aux->getId())))
        {
            auxVertexInduced(aux);
        }
    }
}

//Subgrafo vertice-induzido pelo fecho transitivo direto (A)
Graph* Graph::getVertexInducedDirect(int idSource)
{
    Node* node = getNode(idSource);
    Node* aux;
    Graph* Sub_grafo;

    int* listIdNodes;
    int SubOrder = 0;

    //Setando todos nos como não visitados
    for(int i = 0; i < order; i++)
    {
        aux = getNode(i);
        aux->setVisitado(false);
    }
    auxVertexInduced(node);

    //for para pegar quantos nós foram visitados
    for(int i = 0; i < order; i++)
    {
        node = getNode(i);
        if(node->getVisitado() == true)
        {
            SubOrder++;
        }
    }
    listIdNodes = new int[SubOrder];

    //for para preencher a lista de inteiro com o id dos nós visitados
    int j = 0;
    for(int i = 0; i < order; i++)
    {
        node = getNode(i);
        if(node->getVisitado() == true)
        {
            listIdNodes[j] = node->getId();
            j++;
        }
    }

    //criando subgrafo vertice induzido com os nós visitados
    Sub_grafo = getVertexInduced(listIdNodes,SubOrder);
    return Sub_grafo;
}

//Subgrafo vertice-induzido pelo fecho transitivo indireto (B)
/*
Onde eu olhei estava errado tenho que refazer essa função
*/
Graph* Graph::getVertexInducedIndirect(int idSource)
{
    Node* node = getNode(idSource);
    Node* aux;
    Graph* Sub_grafo;

    int* listIdNodes;
    int SubOrder = 0;

    //Setando todos nos como não visitados
    for(int i = 0; i < order; i++)
    {
        aux = getNode(i);
        aux->setVisitado(false);
    }
    auxVertexInduced(node);

    //for para pegar quantos nós foram visitados
    for(int i = 0; i < order; i++)
    {
        node = getNode(i);
        if(node->getVisitado() == false)
        {
            SubOrder++;
        }
    }
    listIdNodes = new int[SubOrder];

    //for para preencher a lista de inteiro com o id dos nós visitados
    int j = 0;
    for(int i = 0; i < order; i++)
    {
        node = getNode(i);
        if(node->getVisitado() == false)
        {
            listIdNodes[j] = node->getId();
            j++;
        }
    }

    //criando subgrafo vertice induzido com os nós visitados
    Sub_grafo = getVertexInduced(listIdNodes,SubOrder);
    return Sub_grafo;
}


//Auxiliares da função (C) 
bool Graph::AuxDijkstraVazio(bool* Visitado)
{
    for(int i = 0; i < order; i++)
    {
        if(Visitado[i] == false)
        {
            return true;
        }
    }
    return false;
}

Node* Graph::AuxDijkstraSeleciona(float* Dist, bool* Visitado, Node* source)
{
    Node* aux;
    Node* aux2;
    float min = std::numeric_limits<float>::infinity();
    for(int i = 0; i < order; i++)
    {
        aux = getNode(i);
        if(Visitado[aux->getId()] == false)
        {
            if(Dist[aux->getId()] < min)
            {
                min = Dist[aux->getId()];
                aux2 = aux;
                
            }
        }
    }

    cout << "[" << aux2->getId() << "]: "<< min << endl;

    return aux2;
}

//Algoritmo de Dijkstra (C)
float Graph::dijkstra(int idSource, int idTarget)
{
    Node* nodeSource = getNode(idSource);
    Node* aux;
    float* pi = new float[order];
    bool* S = new bool[order];

    for(int i = 0; i < order; i++)
    {
        aux = getNode(i);
        if(aux == nodeSource)
        {
            S[aux->getId()] = true;
            pi[aux->getId()] = 0;
        }else
            {
                S[aux->getId()] = false;
                pi[aux->getId()] = std::numeric_limits<float>::infinity();//seta a distancia como infinito
                if(nodeSource->searchEdge(aux->getId()))
                {
                    pi[aux->getId()] = nodeSource->getEdge(aux->getId())->getWeight();
                }
            }
    }

    Node* j;
    while(AuxDijkstraVazio(S))
    {
        j = AuxDijkstraSeleciona(pi,S,nodeSource);
        S[j->getId()] = true;
        for(int i = 0; i < order; i++)
        {
            aux = getNode(i);
            if(S[aux->getId()] == false && j->searchEdge(aux->getId()))
            {
                if(pi[aux->getId()] > pi[j->getId()] + j->getEdge(aux->getId())->getWeight())
                {
                    pi[aux->getId()] = pi[j->getId()] + j->getEdge(aux->getId())->getWeight();
                }
            }
        }
    }

    return pi[idTarget];

}


/*Ta mais pra um algoritmo de prim doque dijkstra
void Graph::AuxDijkstra(Node* nodeSource, Node* nodeTarget, float* Dist)
{
    for(Node* aux = first_node; aux != nullptr; aux = aux->getNextNode())
    {
        if((aux->getVisitado() == false) && (nodeSource->searchEdge(aux->getId())))
        {
            if(Dist[aux->getId()] > Dist[nodeSource->getId()] + nodeSource->getEdge(aux->getId())->getWeight())
            {
                Dist[aux->getId()] = Dist[nodeSource->getId()] + nodeSource->getEdge(aux->getId())->getWeight();
            }

            AuxDijkstra(aux,nodeTarget,Dist);
        }
    }
}
float Graph::dijkstra(int idSource, int idTarget)
{
    Node* nodeSource = getNode(idSource);
    Node* nodeTarget = getNode(idTarget);
    Node* aux;
    float* Dist = new float[order];
    
    //Setando todos os nós como não visitados
    for(int i = 0; i < order; i++)
    {
        aux = getNode(i);
        aux->setVisitado(false);
        if(aux == nodeSource)
        {
            aux->setVisitado(true);
            Dist[aux->getId()] = 0;
        }else
        Dist[aux->getId()] = std::numeric_limits<float>::infinity();//seta a distancia como infinito
    }
    AuxDijkstra(nodeSource,nodeTarget,Dist);

    cout << "Menor distancia entre [" << nodeSource->getId() << "] e [" << nodeTarget->getId() << "]: " << Dist[nodeTarget->getId()] << endl;
}
//*/


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
