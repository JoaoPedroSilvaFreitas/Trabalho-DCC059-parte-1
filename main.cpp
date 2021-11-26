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
    cout << "A- Subgrafo induzido por conjunto de vertices(Direto)" << endl;
    cout << "B- Subgrafo induzido por conjunto de vertices(Indireto)" << endl;
    cout << "C- Caminho Minimo entre dois vertices - Dijkstra" << endl;
    cout << "D- Caminho Minimo entre dois vertices - Floyd" << endl;
    cout << "E- Arvore Geradora Minima de Prim" << endl;
    cout << "F- Arvore Geradora Minima de Kruskal" << endl;
    cout << "G- Imprimir caminhamento em largura" << endl;
    cout << "H- Imprimir ordenacao topologica" << endl;
    cout << "I- Imprime lista de adjacencia" << endl;
    cout << "0- Sair" << endl;
    cout << "--------------------------------------------------------------------------" << endl;
    cin >> opt;
    return opt;
}


void AuxMainMenu(Graph* grafo)
{
    char opt = '1';
        while(opt != '0')
        {
            opt = MainMenu(grafo);
            //Subgrafo induzido por conjunto de vertices TALVEZ EU TENHA QUE MUDAR TUDO ISSO
            if(opt == 'a' || opt == 'A')
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

                //print pra ver se deu certo
                //função print tem erros
                sub_grafo->Print_Ad_list();
            }
            if(opt == 'b' || opt == 'B')
            {

            }
            if(opt == 'c' || opt == 'C')
            {
                int idSource, idTarget;
                cout << "Menor distancia de:";
                cin >> idSource;
                cout << "Ate:";
                cin >> idTarget;
                grafo->dijkstra(idSource,idTarget);
            }
            
            if(opt == 'i' || opt == 'I')
            {
                grafo->Print_Ad_list();
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

    //Declara duas strings que recebem nome do programa e o nome do arquivo de entrada
    string program_name(argv[0]);
    string input_file_name(argv[1]);

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
        cout << "Unable to open " << argv[1];
    } 

    //ADICIONAR MENU AQUI
    AuxMainMenu(grafo);

   //Fechando arquivos de entrada e saída
    input_file.close();
    output_file.close();
}
