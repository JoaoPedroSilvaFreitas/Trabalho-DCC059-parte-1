#ifndef GRAPH_H
#define GRAPH_H

#include "Node.h"
class Graph
{
    public:
    Graph(int Graph_Size);
    ~Graph();
    void insert_Node(int id, float value);
    void addEdges(int id);
    void Print_Graph();
    Node* getFirstNode();
    Node* getLastNode();

    private:
    int Graph_Size;
    int Edge_Number;
    Node* First_Node;
    Node* Last_Node;
};

#endif