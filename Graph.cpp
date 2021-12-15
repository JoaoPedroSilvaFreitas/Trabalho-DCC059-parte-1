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
//Subgrafo induzido por subconjunto de vertices (J)
Graph* Graph::getVertexInduced(int* listIdNodes, int OrderSubGraph)
{
    //Criando um novo grafo
    Graph* sub_grafo = new Graph(OrderSubGraph, listIdNodes, getDirected(), getWeightedEdge(), getWeightedNode());
    Node* node;
    Node* aux;
    //GRAFO DIRECIONADO
    if(sub_grafo->getDirected() == true)
    {
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
    }
    
    //GRAFO NAO DIRECIONADO
    if(sub_grafo->getDirected() == false)
    {
        if(!sub_grafo->getWeightedEdge() && !sub_grafo->getWeightedNode())//Caso o grafo não tenha peso nos nós e arestas
        {
            for(int i = 0; i < sub_grafo->getOrder(); i++)
            {
                node = getNode(listIdNodes[i]);
                for(int j = 0; j < sub_grafo->getOrder(); j++)
                {
                    aux = getNode(listIdNodes[j]);
                    if(node->searchEdge(listIdNodes[j]))
                    {
                        if(sub_grafo->getNode(listIdNodes[j])->searchEdge(node->getId()) ==  false)
                        {
                            sub_grafo->insertEdge(listIdNodes[i],listIdNodes[j],1);
                        }
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
                    if(node->searchEdge(listIdNodes[j]))
                    {
                        if(sub_grafo->getNode(listIdNodes[j])->searchEdge(node->getId()) ==  false)
                        {
                            edge = node->getEdge(listIdNodes[j]);
                            sub_grafo->insertEdge(listIdNodes[i],listIdNodes[j],edge->getWeight());
                        }
                        
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
                    if(node->searchEdge(listIdNodes[j]))
                    {
                        if(sub_grafo->getNode(listIdNodes[j])->searchEdge(node->getId()) ==  false)
                        {
                            node->setWeight(getNode(listIdNodes[i])->getWeight());
                            sub_grafo->insertEdge(listIdNodes[i],listIdNodes[j],1);
                        }
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
                    if(node->searchEdge(listIdNodes[j]))
                    {
                        if(sub_grafo->getNode(listIdNodes[j])->searchEdge(node->getId()) ==  false)
                        {
                            edge = node->getEdge(listIdNodes[j]);
                            node->setWeight(getNode(listIdNodes[i])->getWeight());
                            sub_grafo->insertEdge(listIdNodes[i],listIdNodes[j],edge->getWeight());
                        }
                    }
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
IDEIA: vou chamar a função do fecho direto varias vezes e depois verificar quais nós conseguem acessar 
meu nó desejado mas nao acessados pelo meu nó desejado, e depois montar um grafo com os nós que atendem 
esses requesitos

*/
Graph* Graph::getVertexInducedIndirect(int idSource)
{
    Graph* Sub_grafo;

    int* listIdNodes;
    int* auxListIdNodes = new int[order];
    int SubOrder = 0;

    for(int i = 0; i < order; i++)
    {
        Sub_grafo = getVertexInducedDirect(i);
        if(Sub_grafo->searchNode(idSource))
        {
            auxListIdNodes[i] = i;
            SubOrder++;
        }
        else
            {
                auxListIdNodes[i] = -1;
            }
    }
    listIdNodes = new int[SubOrder];
    int j = 0;
    for(int i = 0; i < order; i++)
    {
        if(auxListIdNodes[i] != -1)
        {
            listIdNodes[j] = auxListIdNodes[i];
            j++;
        }
    }


    Sub_grafo = getVertexInduced(listIdNodes,SubOrder);
    return Sub_grafo;

/*
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
*/
}

//Algoritmo de Dijkstra (C) e auxiliares
float Graph::dijkstra(int idSource, int idTarget)
{
    //PRECISO VERIFICAR SE TEM CICLOS NEGATIVOS NO GRAFO
    Node* nodeSource = getNode(idSource);
    Node* aux;
    float* pi = new float[order];
    bool* S = new bool[order];

    //Chamando função para setar vertices onde a origem não chega como nao visitados;
    if(getDirected())
    {
        getVertexInducedDirect(idSource);
    }
    
    //Inicializando vertices
    for(int i = 0; i < order; i++)
    {
        aux = getNode(i);

        S[aux->getId()] = false;
        pi[aux->getId()] = std::numeric_limits<float>::infinity();//seta a distancia como infinito

        if(aux == nodeSource)
        {
            S[aux->getId()] = true;
            pi[aux->getId()] = 0;
        }

        if(nodeSource->searchEdge(aux->getId()))
        {
            pi[aux->getId()] = nodeSource->getEdge(aux->getId())->getWeight();
            S[aux->getId()] = false;
        }
    }

    //Setando nós onde a origem nunca chega como visitados
    if(getDirected())
    {
        for(int i = 0; i < order; i++)
        {
            aux = getNode(i);
            if(aux->getVisitado() == false)
            {
                S[aux->getId()] = true;
            }
        }
    }

    Node* j;
    //CREIO QUE SE EU ADICIONAR UM || S[idTarget] == true   NO WHILE RESOLVERIA MEU PROBLEMA
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
                    //PRECISO ADICIONAR EXTENSAO PARA VALORES NEGATIVOS
                }
            }
        }
    }

    return pi[idTarget];

}

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
        if(Visitado[aux->getId()] == false && Dist[aux->getId()] < min)
        {
                min = Dist[aux->getId()];
                aux2 = aux;
        }
    }

    return aux2;
}

//Algoritmo de Floyd (D)
float Graph::floydWarshall(int idSource, int idTarget)
{
    float** Dist = new float*[order];
    for (int i = 0; i < order; i++)
    {
        Dist[i] = new float[order];
    }
    
    Node* aux;
    Node* aux2;
    //INICIANDO MATRIZ
    for(int i = 0; i < order; i++)
    {
        aux = getNode(i);
        for(int j = 0; j < order; j++)
        {
            aux2 = getNode(j);
            if(aux->searchEdge(aux2->getId()))
            {
                Dist[i][j] = aux->getEdge(aux2->getId())->getWeight();
            }else
                {
                    Dist[i][j] = std::numeric_limits<float>::infinity();
                }

            if(i == j)
            {
                Dist[i][j] = 0;
            } 
        }
    }


    for(int k = 0; k < order; k++)
    {
        aux = getNode(k);
        for(int i = 0; i < order; i++)
        {
            for(int j = 0; j < order; j++)
            {
                if(Dist[i][j] > Dist[i][k] + Dist[k][j])
                {
                    Dist[i][j] =  Dist[i][k] + Dist[k][j];
                }
            }
        }
    }

    return Dist[idSource][idTarget];
}

//Arvore Geradora Minima de Prim (E)
bool Graph::AuxPrimVazio()
{

}

void Graph::AuxPrim()
{

}

Graph* Graph::Prim(int* ListIdNodes, int SubOrder)
{
    Node* aux;
    Graph* Sub_grafo;
    bool* S = new bool[SubOrder];
    float* pi = new float[SubOrder];
    int* ant = new int[SubOrder];

    for(int i = 0; i < SubOrder; i++)
    {
        aux = getNode(ListIdNodes[i]);
        if(i == 0)
        {
            pi[i] = 0;
            S[i] = true;
        }else
            {
                pi[i] = std::numeric_limits<float>::infinity();
                S[i] = false;
            }
    }

    while(AuxPrimVazio())
    {

    }
}

//Arvore Geradora Minima de Kruskal (F)
void Graph::KruskalVerificaSubArv()
{

}

void Graph::KruskalUneSubArv()
{
    
}

Graph* Graph::Kruskal(int* ListIdNodes, int SubOrder)
{
    int cont = 0;
    Node* aux;
    Graph* AuxGrafo = getVertexInduced(ListIdNodes,SubOrder);
    
    //Criar Lista L com pesos das arestas em ordem crescente
    //Criar (order) subárvores contendo cada uma um nó isolado
    /*
    while(cont < order-1 &&  Lista L != Vazio)
    {
        //Pego aresta (u,v) de menor peso na lista L
        //Lista L = Lista L - aresta(u,v)
        if(Aresta (u,v) nao estão na mesma subarvore)
        {
            //F = F + (u,v) Unir as subarvores que contem u e v
            cont++;
        }
    }
    */
}

//Ordenação topológica (H)
int Graph::AuxTopologicalSorting(int i, bool* V, int* ordTop, int N)
{
    Node* aux;
    Node* node = getNode(i);
    V[i] = true;
    node->setVisitado(true);
    for(int j = 0; j < order; j++)
    {
        aux = getNode(j);
        if(V[j] == false && node->searchEdge(aux->getId()))
        {
            N = AuxTopologicalSorting(j,V,ordTop,N);
        }
        ordTop[N] = i;
    }
    return N-1;
}

void Graph::TopologicalSorting()
{
    bool* V = new bool[order];
    int* ordTop = new int[order];
    int N = order-1;

    // Creio que essa função depende da vertice induzido indireto;
    if(VerificaCiclos())
    {
        cout << "Grafo ciclico!" << endl;
        return;
    }

    for(int i = 0; i < order; i++)
    {
        getNode(i)->setVisitado(false);
        V[i] = false;
        ordTop[i] = 0;
    }

    for(int i = 0; i < order; i++)
    {
        if(V[i] == false)
        {
           N = AuxTopologicalSorting(i,V,ordTop,N);
        }
    }

    for(int i = 0; i < order; i++)
    {
        cout << ordTop[i] << "   ";
    }
    cout << endl;

}


bool Graph::AuxVerificaCiclos(int i,bool* V)
{
    V[i] = true;
    for(int j = 0; j < order; j++)
    {
        if(V[j] == false && getNode(i)->searchEdge(j))
        {
            cout << j << endl;
            AuxVerificaCiclos(j,V);
        }
        
    }
    return false;
}

bool Graph::VerificaCiclos()
{
    bool* V = new bool[order];
    bool* backEdge = new bool[order];
    for(int i = 0; i < order; i++)
    {
        V[i] = false;
        backEdge[i] = false;
    }

    if(AuxVerificaCiclos(0,V))
    {
        return true;
    }
    return false;
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
