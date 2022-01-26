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
                sub_grafo->getNode(listIdNodes[i])->setWeight(node->getWeight()); //PARA ARRUMAR O ERRO ABAIXO USEI ISSO
                for(int j = 0; j < sub_grafo->getOrder(); j++)
                {
                    if(node->searchEdge(listIdNodes[j]))
                    {
                        if(sub_grafo->getNode(listIdNodes[j])->searchEdge(node->getId()) ==  false)
                        {
                            //node->setWeight(getNode(listIdNodes[i])->getWeight()); ESSA PARTE ESTAVA ERRADA PQ NAO ADICIONAVA O PESO NO SUBGRAFO
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
                sub_grafo->getNode(listIdNodes[i])->setWeight(node->getWeight()); //PARA ARRUMAR O ERRO ABAIXO USEI ISSO
                for(int j = 0; j < sub_grafo->getOrder(); j++)
                {
                    if(node->searchEdge(listIdNodes[j]))
                    {
                        if(sub_grafo->getNode(listIdNodes[j])->searchEdge(node->getId()) ==  false)
                        {
                            edge = node->getEdge(listIdNodes[j]);
                            //node->setWeight(getNode(listIdNodes[i])->getWeight()); ESSA PARTE ESTAVA ERRADA PQ NAO ADICIONAVA O PESO NO SUBGRAFO
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
    
    //Verificando se existem ciclos negativos no grafo
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

//Arvore Geradora Minima de Prim (E) (incompleto)
bool Graph::AuxPrim(float min, float** custo, int* prox)
{
    float Min = std::numeric_limits<float>::infinity();
    for(int i = 0; i < order; i++)
    {
        for(int j = 0; j < order; j++)
        {
            if(prox[i] != -1 && custo[i][j] < Min)
            {
                Min = custo[i][j];
            }
        }
    }
    if(min == Min)
    {
        return true;
    }else
        {
            return false;
        }
}

Graph* Graph::Prim(int* ListIdNodes, int SubOrder, ofstream& output_file)
{
    Graph* Arv = new Graph(SubOrder, ListIdNodes, getDirected(), getWeightedEdge(), getWeightedNode());
    int* prox = new int[SubOrder];
    float** custo = new float*[SubOrder];
    float EdgeMenorCusto = std::numeric_limits<float>::infinity();
    int idMenorCusto[2];
    int cont, J;
    int k = 0;
    Node* aux;
    for(int i = 0; i < SubOrder; i++)
    {
        aux = getNode(ListIdNodes[i]);
        custo[i] = new float[SubOrder];
        for(int j = 0; j < SubOrder; j++)
        {
            if(aux->searchEdge(ListIdNodes[j]))
            {
                custo[ListIdNodes[i]][ListIdNodes[j]] = aux->getEdge(ListIdNodes[j])->getWeight();
                if((aux->getEdge(ListIdNodes[j])->getWeight() < EdgeMenorCusto))
                {
                    EdgeMenorCusto = aux->getEdge(ListIdNodes[j])->getWeight();
                    idMenorCusto[0] = ListIdNodes[i];
                    idMenorCusto[1] = ListIdNodes[j];
                }
            }else
                {
                    custo[ListIdNodes[i]][ListIdNodes[j]] = std::numeric_limits<float>::infinity();
                }
        }
    }

    aux = Arv->getNode(idMenorCusto[0]);//Aponta para o nó de menor custo
    aux->insertEdge(idMenorCusto[1],getNode(idMenorCusto[0])->getEdge(idMenorCusto[1])->getWeight());//Insere na solução a aresta de menor custo
    for(int i = 0; i < SubOrder; i++)
    {
        aux = getNode(ListIdNodes[i]);
        if(aux->searchEdge(idMenorCusto[0]) || aux->searchEdge(idMenorCusto[1]))
        {
            if(custo[ListIdNodes[i]][idMenorCusto[0]] < custo[ListIdNodes[i]][idMenorCusto[1]])
            {
                prox[ListIdNodes[i]] = idMenorCusto[0];
            }else
                {
                    prox[ListIdNodes[i]] = idMenorCusto[1];
                }
        }else
            {
                prox[ListIdNodes[i]] = idMenorCusto[1];
            }
    }
    prox[idMenorCusto[1]] = -1;
    prox[idMenorCusto[0]] = -1;
    cont = 1;

    while(cont < SubOrder-1)
    {   
        
        for(int j = 0; j < SubOrder; j++)
        {
            if(prox[ListIdNodes[j]] != -1 && AuxPrim(custo[ListIdNodes[j]][prox[ListIdNodes[j]]],custo,prox))
            {
                aux = Arv->getNode(ListIdNodes[j]);
                aux->insertEdge(prox[ListIdNodes[j]],getNode(ListIdNodes[j])->getEdge(prox[ListIdNodes[j]])->getWeight());
                prox[ListIdNodes[j]] == -1;
                J = ListIdNodes[j];
                break;
            }
            
        }
        for(int i = 0; i < SubOrder; i++)
        {
            if(prox[ListIdNodes[i]] != -1 && (custo[ListIdNodes[i]][prox[ListIdNodes[i]]] > custo[ListIdNodes[i]][J]))
            {
                prox[ListIdNodes[i]] = J;
            }
        }
        cont++;
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
Graph* Graph::Kruskal(int* ListIdNodes, int SubOrder, ofstream& output_file)
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
    float tempW;
    int tempS, tempT;
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
        //Verifica se o no origem e o no de chegada da aresta estao na arvore
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

    //Imprime na tela
    arv->Print_Ad_list();

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


//FUNÇÕES PARTE 2
bool Graph::GulosoVazio(bool* V)
{
    for(int i = 0; i < order; i++)
    {
        if(V[i] == false)
        {
            return false;
        }
    }

    return true;
}


float Graph::Guloso(int k, ofstream& output_file)
{
    int decrescente [order];
    int temp, cont = 0, I = 0, deltaId;
    bool** solucao = new bool*[order];
    bool visitado[order];
    Node* aux;
    Graph* GrafoAux;
    float delta = 0, min = std::numeric_limits<float>::infinity();

    //inicianto matriz solução
    //A matrix solução consiste em N linhas (cada linha representando um vertice) e K colunas (cada coluna representando uma partição do grafo)
    //se o vertice estiver no grupo k na solução ele sera true, caso contrario false
    for(int i = 0; i < order; i ++)
    {
        solucao[i] = new bool[k];
        for(int j = 0; j < k; j++)
        {
            solucao[i][j] = false;
        }
    }

    //Organizando vertices de forma decrescente em uma fila
    //Preenchendo vetor auxiliar
    for(int i = 0; i < order; i++)
    {
        aux = getNode(i);
        visitado[i] = false;
        decrescente[i] = aux->getId();
    }

    for(int i = 0; i < order; i ++)
    {
        for(int j = 0; j < order; j++)
        {
            if(getNode(decrescente[j])->getWeight() < getNode(decrescente[i])->getWeight())
            {
               temp = decrescente[i];
               decrescente[i] = decrescente[j];
               decrescente[j] = temp;
            }
        }
    }

    while(GulosoVazio(visitado) == false)
    {
        if(cont == k-1)
        {
            //esse cont organiza em quais partições será inserido por exemplo,
            //se eu estou no cont = 0, vou inserir o nó na partição k0
            cont == 0;
        }

        //função criterio
        if(visitado[decrescente[I]] == false)
        {
            aux = getNode(decrescente[I]);
            solucao[aux->getId()][cont] = true; //inserindo na solução
            GrafoAux = getVertexInducedDirect(aux->getId(),output_file);

            for(int i = 0; i < order; i++)
            {
                if(GrafoAux->searchNode(i))
                {
                    delta = aux->getWeight() - GrafoAux->getNode(i)->getWeight();
                    cout << aux->getWeight() << " - " << GrafoAux->getNode(i)->getWeight() << " = " << delta << endl;

                    if(delta < min)
                    {
                        deltaId = GrafoAux->getNode(i)->getId();
                        min = delta;
                    }
                }
                
            }

            for(int i = 0; i < order; i++)
            {
                cout << i << "(A):" << GrafoAux->getNode(i)->getWeight()<< endl;
                cout << i << "(G):" << getNode(i)->getWeight()<< endl;
            }


            solucao[deltaId][cont] = true;
            visitado[deltaId] = true;
            I++;
            cont++;
        }
        
    }


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
