#ifndef EDGE_H_INCLUDED
#define EDGE_H_INCLUDED

using namespace std;

class Edge
{
    public:
        // Constructor e Destructor
        Edge(int target_id);
        Edge(int source_id, int target_id, float weight);
        ~Edge();

        // Getters
        int getTargetId();
        Edge* getNextEdge();
        float getWeight();
        int getSourceId();


        // Setters
        void setNextEdge(Edge* edge);
        void setWeight(float weight); 

    private:
        int target_id;
        Edge* next_edge;
        float weight;
        int source_id;
}; 

#endif // EDGE_H_INCLUDED