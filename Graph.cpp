#include "Graph.h"
#include "Node.h"
#include "Edge.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <queue>
#include <list>

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
Graph* Graph::getVertexInducedDirect(int idSource, ofstream& output_file)
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
Graph* Graph::getVertexInducedIndirect(int idSource, ofstream& output_file)
{
    Graph* Sub_grafo;

    int* listIdNodes;
    int* auxListIdNodes = new int[order];
    int SubOrder = 0;

    for(int i = 0; i < order; i++)
    {
        Sub_grafo = getVertexInducedDirect(i, output_file);
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

}

//Algoritmo de Dijkstra (C) e auxiliares
float Graph::dijkstra(int idSource, int idTarget, ofstream& output_file)
{
    Node* nodeSource = getNode(idSource);
    Node* aux;
    float* pi = new float[order];
    float* piEstrela = new float[order];
    bool* S = new bool[order];
    
    //Verificande se existem ciclos negativos no grafo
    if(VerificaCiclos(output_file) == true)
    {
        if(VerificaCiclosNegativos(output_file) == true)
        {
            return NULL;
        }
    }

    //Chamando função para setar vertices onde a origem não chega como nao visitados;
    if(getDirected())
    {
        getVertexInducedDirect(idSource, output_file);
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
    while(AuxDijkstraVazio(S))
    {
        j = AuxDijkstraSeleciona(pi,S,nodeSource);
        S[j->getId()] = true;
        for(int i = 0; i < order; i++)
        {
            aux = getNode(i);
            if(j->searchEdge(aux->getId()))
            {
                piEstrela[aux->getId()] = pi[j->getId()] + j->getEdge(aux->getId())->getWeight();
                if(piEstrela[aux->getId()] < pi[aux->getId()])
                {
                    S[aux->getId()] = false;
                    pi[aux->getId()] = piEstrela[aux->getId()];
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
float Graph::floydWarshall(int idSource, int idTarget, ofstream& output_file)
{
    //Verificande se existem ciclos negativos no grafo
    if(VerificaCiclos(output_file) == true)
    {
        if(VerificaCiclosNegativos(output_file) == true)
        {
            return NULL;
        }
    }

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
Graph* Graph::Prim(int* ListIdNodes, int SubOrder, ofstream& output_file)
{
    Graph* Arv = new Graph(SubOrder, ListIdNodes, getDirected(), getWeightedEdge(), getWeightedNode());
    queue<Edge>PQ;
    Edge* edge;
    Node* aux;
    bool* V = new bool[order];
    float* Eweight = new float[number_edges]; // vetor com valor da aresta
    int* idSource = new int[number_edges]; //vetor com id de saida da aresta
    int* idTarget = new int[number_edges]; // vetor com id de chegada da aresta
    int k = 0;
    int ArvNE = order - 1;
    int cont = 0;
    float MSTcost = 0;

    for(int i = 0; i < order; i++)
    {
        V[i] = false;
    }

    //adicionando informações das arestas em 3 vetores
    for(int i = 0; i < order; i++)
    {
        aux = getNode(i);
        for(int j = 0; j < order; j++)
        {
            if(aux->searchEdge(j))
            {
                idSource[k] = i;
                idTarget[k] = j;
                Eweight[k] = aux->getEdge(j)->getWeight();
                k++;
            }
        }
    }

    //Organizando arestas em ordem crescente
    int tempW, tempS, tempT;
    for(int i = 0; i < number_edges; i++)
    {
        for( int j = i + 1; j < number_edges; j++)
        {
            if(Eweight[i] > Eweight[j])
                {
                    tempW = Eweight[i];
                    tempS = idSource[i];
                    tempT = idTarget[i];
                    Eweight[i] = Eweight[j];
                    idSource[i] = idSource[j];
                    idTarget[i] = idTarget[j];
                    Eweight[j] = tempW;
                    idSource[j] = tempS;
                    idTarget[j] = tempT;
                }
        }
    }

    //Preenchendo fila com arestas em ordem crescente
    for(int i = 0; i < number_edges; i++)
    {
        PQ.push(Edge(idSource[i],idTarget[i],Eweight[i]));
    }

    //Loop para percorrer fila 
    while(!PQ.empty() && cont != ArvNE)
    {   
        //Verifica se o no orgem e o no de chegada da aresta estao na arvore
        if(Arv->searchNode(PQ.front().getSourceId()) == true && Arv->searchNode(PQ.front().getTargetId()) == true)
        {
            //verificar se o no de chegada ainda nao possui aresta na arvore
            if(V[PQ.front().getTargetId()] == false)
            {
                aux = Arv->getNode(PQ.front().getSourceId());
                aux->insertEdge(PQ.front().getTargetId(), PQ.front().getWeight());//Inserindo aresta na arvore
                V[PQ.front().getTargetId()] = true;//informando que no agora possui aresta
                PQ.pop();
                cont++;
            }else
                {
                    PQ.pop();//caso ja tenha aresta na arvore remover da lista
                }
        }else
            {
                PQ.pop();//caso nao estejam na arvore remover da lista
            }
        
    }

    //imprimindo arvore na tela
    Arv->Print_Ad_list();

    //Salvando grafo no arquivo de saída
    if(output_file.is_open())
        {
            output_file << "ArvoreMinimaPrim {" << endl;
            Arv->Print_Graph_OF(output_file);
            output_file << "}" << endl;
        }
        else
            {
                cerr << "erro ao abrir " << endl;
                exit(1);
            }

    return Arv;
}

//Arvore Geradora Minima de Kruskal (F)
bool Graph::KruskalVerificaVazio(float* EdgeW)
{
    for(int i = 0; i < order; i++)
    {
        if(EdgeW[i] != NULL)
        {
            return false;//lista nao vazia
        }
    }

    return true;//lista vazia
}

bool Graph::KruskalVerificaSubArv(Graph* SubArv, int idSource, int idTarget)
{
    Node* aux;
    aux = SubArv->getNode(idSource);
    if(aux->searchEdge(idTarget))
    {
        return true;
    }
    return false;
}

void Graph::KruskalUneSubArv(Graph* SubArv, int idSource, int idTarget, float Eweight)
{
    Node* aux;
    aux = SubArv->getNode(idSource);
}

Graph* Graph::Kruskal(int* ListIdNodes, int SubOrder, ofstream& output_file)
{
    Node* aux;
    Graph* Arv;
    Arv = new Graph(SubOrder, ListIdNodes, getDirected(), getWeightedEdge(), getWeightedNode()); // criando N subarvores
    float* Eweight = new float[number_edges]; // vetor com valor da aresta
    int* idSource = new int[number_edges]; //vetor com id de saida da aresta
    int* idTarget = new int[number_edges]; // vetor com id de chegada da aresta
    int k = 0, cont = 0;
    for(int i = 0; i < order; i++)
    {
        aux = getNode(i);
        for(int j = 0; j < order; j++)
        {
            if(aux->searchEdge(j))
            {
                idSource[k] = i;
                idTarget[k] = j;
                Eweight[k] = aux->getEdge(j)->getWeight();
                k++;
            }
        }
    }

    //Organizando arestas em ordem crescente
    int tempW, tempS, tempT;
    for(int i = 0; i < number_edges; i++)
    {
        for( int j = i + 1; j < number_edges; j++)
        {
            if(Eweight[i] > Eweight[j])
                {
                    tempW = Eweight[i];
                    tempS = idSource[i];
                    tempT = idTarget[i];
                    Eweight[i] = Eweight[j];
                    idSource[i] = idSource[j];
                    idTarget[i] = idTarget[j];
                    Eweight[j] = tempW;
                    idSource[j] = tempS;
                    idTarget[j] = tempT;
                }
        }
    }
    
    


     while(cont < SubOrder-1 && KruskalVerificaVazio(Eweight) == false)
    {
        //ids da areta de menor peso
        idSource[cont];
        idTarget[cont];
        if(KruskalVerificaSubArv(Arv,idSource[cont],idTarget[cont]) == false)
        {
            KruskalUneSubArv(Arv,idSource[cont],idTarget[cont],Eweight[cont]);
            cont++;
        }
        //"removendo" aresta da lista
        Eweight[cont] = NULL;
    }

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

   //Salvando grafo no arquivo de saída
    if(output_file.is_open())
        {
            output_file << "ArvoreMinimaKruskal {" << endl;
            Arv->Print_Graph_OF(output_file);
            output_file << "}" << endl;
        }
        else
            {
                cerr << "erro ao abrir " << endl;
                exit(1);
            }
    return Arv;
}

//Busca em largura (G)
void Graph::breadthFirstSearch(int idSource, ofstream& output_file)
{
    Node* aux;
    std::queue<int> Q;
    bool* V = new bool[order];
    Graph* arv = new Graph(order, getDirected(), getWeightedEdge(), getWeightedNode());
    for(int i = 0; i < order; i++)
    {
        if(i == idSource)
        {
            V[i] = true;
        }else{
            V[i] = false;
        }
    }
    Q.push(idSource);
    while(!Q.empty())
    {
        Node* aux = getNode(Q.front());
        Q.pop();
        for(int i = 0; i < order; i++)
        {
            if(V[i] == false && aux->searchEdge(i))
            {
                arv->getNode(aux->getId())->insertEdge(i,aux->getEdge(i)->getWeight());
                Q.push(i);
                V[i] = true;
            }
        }
    }

    //Salvando grafo no arquivo de saída
    if(output_file.is_open())
        {
            output_file << "ArvoreBFS{" << endl;
            arv->Print_Graph_OF(output_file);
            output_file << "}" << endl;
        }
        else
            {
                cerr << "erro ao abrir " << endl;
                exit(1);
            }

}

//Ordenação topológica (H)
int Graph::AuxTopologicalSorting(int i, bool* V, int* ordTop, int N, ofstream& output_file)
{
    Node* aux;
    Node* node = getNode(i);
    V[i] = true;
    for(int j = 0; j < order; j++)
    {
        aux = getNode(j);
        if(V[j] == false && node->searchEdge(aux->getId()))
        {
            N = AuxTopologicalSorting(j,V,ordTop,N, output_file);
        }
        ordTop[N] = i;
    }
    return N-1;
}

void Graph::TopologicalSorting(ofstream& output_file)
{
    bool* V = new bool[order];
    int* ordTop = new int[order];
    int N = order-1;

    if(VerificaCiclos(output_file) && getDirected())
    {
        cout << "Grafo ciclico direcionado!" << endl;
        return;
    }

    for(int i = 0; i < order; i++)
    {
        V[i] = false;
        ordTop[i] = 0;
    }

    for(int i = 0; i < order; i++)
    {
        if(V[i] == false)
        {
           N = AuxTopologicalSorting(i,V,ordTop,N, output_file);
        }
    }

    cout << "Ordenacao topologica:" << endl;
    for(int i = 0; i < order; i++)
    {
        cout << ordTop[i] << "   ";
    }
    cout << endl;

}



//Outras

bool Graph::AuxVerificaCiclosNegativos(int i,bool* V, ofstream& output_file)
{
    float Soma = 0;
    V[i] = true;
    Graph* NosPais;
    NosPais = getVertexInducedIndirect(i, output_file);
    for(int j = 0; j < order; j++)
    {
        
        if(V[j] == false && getNode(i)->searchEdge(j))
        {
            if(NosPais->searchNode(j))
            {
                Node* aux;
                for(int u = 0; u < NosPais->getOrder(); u++)
                {
                    aux = NosPais->getNode(u);
                    for(int v = 0; v < NosPais->getOrder(); v++)
                    {
                        if(aux->searchEdge(NosPais->getNode(v)->getId()))
                        {
                            Soma = Soma + aux->getEdge(v)->getWeight();
                        }
                    }
                }
                if(Soma < 0)
                {
                    return true;
                }
            }
            AuxVerificaCiclos(j,V, output_file);
        }
    }
    return false;
}

bool Graph::VerificaCiclosNegativos(ofstream& output_file)
{
    bool* V = new bool[order];
    for(int i = 0; i < order; i++)
    {
        V[i] = false;
    }

    if(AuxVerificaCiclosNegativos(0,V, output_file))
    {
        return true;
    }
    return false;
}

bool Graph::AuxVerificaCiclos(int i,bool* V, ofstream& output_file)
{
    V[i] = true;
    Graph* NosPais;
    NosPais = getVertexInducedIndirect(i, output_file);
    for(int j = 0; j < order; j++)
    {
        
        if(V[j] == false && getNode(i)->searchEdge(j))
        {
            if(NosPais->searchNode(j))
            {
                return true; // em alguns grafos especificos esse return é ignorado
            }
            AuxVerificaCiclos(j,V,output_file);
        }
    }
    return false;
}

bool Graph::VerificaCiclos(ofstream& output_file)
{
    bool* V = new bool[order];
    for(int i = 0; i < order; i++)
    {
        V[i] = false;
    }

    if(AuxVerificaCiclos(0,V,output_file))
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

void Graph::Print_Graph_OF(ofstream& output_file)
{
    for(Node* aux = first_node; aux != nullptr; aux = aux->getNextNode())
    {
        if(getDirected())
        {
            if(aux->getNumberEdges() == 0)
            {
                output_file << "    " << aux->getId() << ";" << endl;
            }else
                {
                output_file << "    " << aux->getId() << "->";
                aux->Print_Edges_OF(output_file, true);
                output_file << endl;
                }
            
        }else
        {
            if(aux->getNumberEdges() == 0)
            {
                output_file << "    " << aux->getId() << ";" << endl;
            }else
                {
                output_file << "    " << aux->getId() << "--";
                aux->Print_Edges_OF(output_file, false);
                output_file << endl;
                }
        }
    }
}
