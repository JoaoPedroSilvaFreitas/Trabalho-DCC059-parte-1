#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include "Graph.h"
#include "Node.h"
using namespace std;

Graph* Cria_Grafo(ifstream& input_file, int directed, int weightedEdge, int weightedNode)
{

    //Variáveis para auxiliar na criação de arestas no Grafo
    int idNodeSource;
    int idNodeTarget;
    int order;

    //Pegando a ordem do grafo
    input_file >> order;

    //Criando objeto grafo
    Graph* graph = new Graph(order, directed, weightedEdge, weightedNode);

    if(!graph->getWeightedEdge() && !graph->getWeightedNode())//Caso o grafo não tenha peso nos nós e arestas
    {
        while(input_file >> idNodeSource >> idNodeTarget) 
        {
            graph->insertEdge(idNodeSource, idNodeTarget, 1);
        }

    }else if(graph->getWeightedEdge() && !graph->getWeightedNode() )//Caso o grafo tenha peso nas arestas
    {

        float edgeWeight;

        while(input_file >> idNodeSource >> idNodeTarget >> edgeWeight) 
        {

            graph->insertEdge(idNodeSource, idNodeTarget, edgeWeight);

        }

    }else if(graph->getWeightedNode() && !graph->getWeightedEdge())//Caso o grafo tenha peso nos nós
    {

        float nodeSourceWeight, nodeTargetWeight;

        while(input_file >> idNodeSource >> nodeSourceWeight >> idNodeTarget >> nodeTargetWeight) 
        {

            graph->insertEdge(idNodeSource, idNodeTarget, 1);
            graph->getNode(idNodeSource)->setWeight(nodeSourceWeight);
            graph->getNode(idNodeTarget)->setWeight(nodeTargetWeight);

        }

    }else if(graph->getWeightedNode() && graph->getWeightedEdge())//Caso o grafo tenha peso nos nós e arestas
    {

        float nodeSourceWeight, nodeTargetWeight, edgeWeight;

        while(input_file >> idNodeSource >> nodeSourceWeight >> idNodeTarget >> nodeTargetWeight >> edgeWeight) 
        {

            graph->insertEdge(idNodeSource, idNodeTarget, edgeWeight);
            graph->getNode(idNodeSource)->setWeight(nodeSourceWeight);
            graph->getNode(idNodeTarget)->setWeight(nodeTargetWeight);

        }
    }

    return graph;
}

char MainMenu(Graph* grafo)
{
    char opt = '1';
    cout << "--------------------------------Main Menu---------------------------------" << endl;
    cout << "A- Subgrafo vertice-induzido pelo fecho transitivo direto(DIRECIONADO)" << endl;
    cout << "B- Subgrafo vertice-induzido pelo fecho transitivo indireto(DIRECIONADO)" << endl;
    cout << "C- Caminho Minimo entre dois vertices - Dijkstra" << endl;
    cout << "D- Caminho Minimo entre dois vertices - Floyd" << endl;
    cout << "E- Arvore Geradora Minima de Prim" << endl;
    cout << "F- Arvore Geradora Minima de Kruskal" << endl;
    cout << "G- Imprimir caminhamento em largura" << endl;
    cout << "H- Imprimir ordenacao topologica" << endl;
    cout << "----------------------------Funcoes adicionais----------------------------" << endl;
    cout << "I- Imprime lista original" << endl;
    cout << "J- Subgrafo induzido por conjunto de vertices" << endl;
    cout << "0- Sair" << endl;
    cout << "--------------------------------------------------------------------------" << endl;
    cin >> opt;
    return opt;
}

void AuxMainMenu(Graph* grafo, ofstream& output_file, string output_file_name)
{
    char opt = '1';
        while(opt != '0')
        {
            opt = MainMenu(grafo);
            //Subgrafo vertice induzido fecho transitivo direto
            if(opt == 'a' || opt == 'A')
            {
                if(grafo->getDirected())
                {
                    Graph* grafoDireto;
                    int idSource;
                    do{
                        cout << "Vertice:";
                        cin >> idSource;
                    }while(idSource > grafo->getOrder()-1 || idSource < 0);
                    grafoDireto = grafo->getVertexInducedDirect(idSource, output_file);
                    grafoDireto->Print_Ad_list();

                    //Salvando grafo no arquivo de saída
                    if(output_file.is_open())
                    {
                        output_file << "GrafoVerticeInduzidoDireto {" << endl;
                        grafoDireto->Print_Graph_OF(output_file);
                        output_file << "}" << endl;
                    }
                    else
                        {
                            cerr << "erro ao abrir " << endl;
                            exit(1);
                        }                    


                }else
                cout << "ERRO: Grafo nao direcionado!" << endl;
            }

            //Subgrafo vertice induzido fecho transitivo indireto
            if(opt == 'b' || opt == 'B')
            {
                if(grafo->getDirected())
                {
                    Graph* grafoIndireto;
                    int idSource;
                    do{
                        cout << "Vertice:";
                        cin >> idSource;
                    }while(idSource > grafo->getOrder()-1 || idSource < 0);
                    grafoIndireto = grafo->getVertexInducedIndirect(idSource, output_file);
                    grafoIndireto->Print_Ad_list();

                    //Salvando grafo no arquivo de saída
                    if(output_file.is_open())
                    {
                        output_file << "GrafoVerticeInduzidoIndireto {" << endl;
                        grafoIndireto->Print_Graph_OF(output_file);
                        output_file << "}" << endl;
                    }
                    else
                        {
                            cerr << "erro ao abrir " << endl;
                            exit(1);
                        }
                }else
                cout << "ERRO: Grafo nao direcionado!" << endl;
            }

            //Algotirmo de Dijkstra
            if(opt == 'c' || opt == 'C')
            {
                int idSource, idTarget;
                float MenorCaminho;
                cout << "Menor distancia entre:" << endl;
                do{
                    cout << "Origem:";
                    cin >> idSource;
                }while(idSource > grafo->getOrder()-1 || idSource < 0);
                do{
                    cout << "Destino:";
                    cin >> idTarget;
                }while(idTarget > grafo->getOrder()-1 || idTarget < 0);
                MenorCaminho = grafo->dijkstra(idSource,idTarget, output_file);
                if(MenorCaminho == NULL)
                {
                    cout << "ERRO:Grafo possui ciclos negativos!" << endl;
                }else
                    {
                        cout << "Menor distancia entre " << idSource << " e " << idTarget << ": " << MenorCaminho << endl;
                    }
            }

            //Algoritmo de Floyd
            if(opt == 'd' || opt == 'D')
            {
                int idSource, idTarget;
                float MenorCaminho;
                cout << "Menor distancia entre:" << endl;
                do{
                    cout << "Origem:";
                    cin >> idSource;
                }while(idSource > grafo->getOrder()-1 || idSource < 0);
                do{
                    cout << "Destino:";
                    cin >> idTarget;
                }while(idTarget > grafo->getOrder()-1 || idTarget < 0);
                MenorCaminho = grafo->floydWarshall(idSource,idTarget, output_file);
                if(MenorCaminho == NULL)
                {
                    cout << "ERRO:Grafo possui ciclos negativos!" << endl;
                }else
                    {
                        cout << "Menor distancia entre " << idSource << " e " << idTarget << ": " << MenorCaminho << endl;
                    }
            }

            //Algoritmo de Prim
            if(opt == 'e' || opt == 'E')
            {
                int SubOrder;
                int id;
                cout << "Digite o tamanho da lista de vertices: ";
                cin >> SubOrder;
                while(SubOrder > grafo->getOrder()-1 || SubOrder < 0)
                {
                    cout << "Digite o tamanho da lista de vertices: ";
                    cin >> SubOrder;
                }
                int* listIdNodes = new int[SubOrder];
                cout << "Digite os Ids para a arvore geradora minima:" << endl;
                for(int i = 0; i < SubOrder; i++)
                {
                    cin >> id;
                    while(id > grafo->getOrder()-1 || id < 0)
                    {
                        cout << "Vertice invalido!" << endl;
                        cin >> id;
                    }
                    listIdNodes[i] = id;
                }
                grafo->Prim(listIdNodes,SubOrder, output_file);
            }

            //Algoritmo de Kruskal
            if(opt == 'f' || opt == 'F')
            {
                int SubOrder;
                int id;
                cout << "Digite o tamanho da lista de vertices: ";
                cin >> SubOrder;
                while(SubOrder > grafo->getOrder()-1 || SubOrder < 0)
                {
                    cout << "Digite o tamanho da lista de vertices: ";
                    cin >> SubOrder;
                }
                int* listIdNodes = new int[SubOrder];
                cout << "Digite os Ids para a arvore geradora minima:" << endl;
                for(int i = 0; i < SubOrder; i++)
                {
                    cin >> id;
                    while(id > grafo->getOrder()-1 || id < 0)
                    {
                        cout << "Vertice invalido!" << endl;
                        cin >> id;
                    }
                    listIdNodes[i] = id;
                }
                grafo->Kruskal(listIdNodes,SubOrder, output_file);
            }

            //Caminhamento em largura
            if(opt == 'g' || opt == 'G')
            {
                Graph* ArvLargura;
                int idSource;
                do{
                    cout << "Vertice:";
                    cin >> idSource;
                }while(idSource > grafo->getOrder()-1 || idSource < 0);
                grafo->breadthFirstSearch(idSource, output_file);
            }

            //Ordenação topológica
            if(opt == 'h' || opt == 'H')
            {
                grafo->TopologicalSorting(output_file);
            }

            //Imprime Lista de adjacência original
            if(opt == 'i' || opt == 'I')
            {
                grafo->Print_Ad_list();
            }

            //Subgrafo induzido por conjunto de vertices
            if(opt == 'j' || opt == 'J')
            {
                int OrderSubGraph;
                Graph* sub_grafo;
                cout << "Ordem do subgrafo vertice induzido:";
                cin >> OrderSubGraph;
                int* listIdNodes = new int[OrderSubGraph];
                cout << "Conjunto de Id dos nos:" << endl;
                for(int i = 0; i < OrderSubGraph; i++)
                {
                    cin >> listIdNodes[i];
                }

                sub_grafo = grafo->getVertexInduced(listIdNodes, OrderSubGraph);
                sub_grafo->Print_Ad_list();
            }
        }
}

int main(int argc, const char*argv[])
{
    //Verifica se foram passados todos parametros
    if (argc != 6) 
    {
        cout << "ERROR: Expecting: ./<program_name> <input_file> <output_file> <directed> <weighted_edge> <weighted_node> " << endl;
        return 1;
    }

    //Declara tres strings que recebem nome do programa, nome dos arquivos de entrada e saida
    string program_name(argv[0]);
    string input_file_name(argv[1]);
    string output_file_name(argv[2]);

    //"Declara" arquivo de entrada e saida
    ifstream input_file;
    ofstream output_file;

    //Abre o aquivo de entrada e o arquivo de saida
    input_file.open(argv[1], ios::in);
    output_file.open(argv[2], ios::out | ios::trunc);

    //Declara um ponteiro de grafo
    Graph* grafo;

    //Verifica se o arquivo de entrada foi aberto com sucesso
    if(input_file.is_open())
    {
        //Lê o arquivo de entrada (atoi converte string em inteiro)
        grafo = Cria_Grafo(input_file, atoi(argv[3]), atoi(argv[4]), atoi(argv[5]));
    }
    else
    {
        cout << "ERRO: Nao foi possivel abrir arquivo " << argv[1];
    } 

    //ADICIONAR MENU AQUI
    AuxMainMenu(grafo,output_file,output_file_name);

   //Fechando arquivos de entrada e saída
    input_file.close();
    output_file.close();
}
