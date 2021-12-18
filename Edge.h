#ifndef EDGE_H_INCLUDED
#define EDGE_H_INCLUDED

using namespace std;

class Edge
{
    public:
        // Constructor e Destructor
        Edge(int target_id);
        ~Edge();

        // Getters
        int getTargetId();
        Edge* getNextEdge();
        float getWeight();


        // Setters
        void setNextEdge(Edge* edge);
        void setWeight(float weight); 

    private:
        int target_id;
        Edge* next_edge;
        float weight;
        int id;
}; 

#endif // EDGE_H_INCLUDED