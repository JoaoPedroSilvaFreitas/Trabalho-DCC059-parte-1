#ifndef EDGE_H
#define EDGE_H

class Edge
{
    public:
    Edge(int id_target);
    ~Edge();
    void setNextEdge(Edge* edge);
    int getTargetID();
    Edge* getNextEdge();


    private:
    int id_target;
    Edge* next_edge;

};

#endif