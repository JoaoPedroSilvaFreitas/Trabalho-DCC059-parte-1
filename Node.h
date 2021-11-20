#ifndef NODE_H
#define NODE_H

#include "Edge.h"
class Node
{
    public:
    Node(int id, float value);
    ~Node();
    void printEdges();

    //Get
    Node* getNext();
    int getID();
    float getVALUE();
    void getEdges();

    //Set
    void setNextNode(Node *node);
    void setEdge(int id_target);
    

    private:
    int id;
    float value;
    Node* Next_Node;
    Edge* First_Edge;
    Edge* Last_Edge;
};

#endif