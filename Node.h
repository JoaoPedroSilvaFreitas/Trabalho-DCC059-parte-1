#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED
#include "Edge.h"

using namespace std;

class Node
{
    public:
        // Constructor & Destructor
        Node(int id);
        ~Node();


        // Getters
        Edge* getFirstEdge();
        Edge* getLastEdge();
        int getId();
        int getInDegree();
        int getOutDegree();
        float getWeight();
        Node* getNextNode();


        // Setters
        void setNextNode(Node* node);
        void setWeight(float weight);


        // Other methods
        bool searchEdge(int target_id);
        void insertEdge(int target_id, float weight);
        void removeAllEdges();
        void incrementOutDegree();
        void decrementOutDegree();
        void incrementInDegree();
        void decrementInDegree();
        bool hasEdgeBetween(int target_id);


        //Minhas funções
        void Print_Edges();
        int getNumberEdges();


    private:
        Edge* first_edge;
        Edge* last_edge;
        int id;
        int number_edges;
        unsigned int in_degree;
        unsigned int out_degree;
        float weight;
        Node* next_node;
};

#endif // NODE_H_INCLUDED
