/**************************************************************************************************
 * Implementation of the TAD Graph
**************************************************************************************************/

#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include "Node.h"
#include <fstream>
#include <stack>
#include <list>
#include <map>
#include <vector>
#include <tuple>

using namespace std;


class Graph {

    //Atributes
    private:
        int order;
        int number_edges;
        bool directed;
        bool weighted_edge;
        bool weighted_node;
        Node *first_node;
        Node *last_node;
        
        int cluster;
        string clusterType;
        vector<tuple<int, int>> clustersLimits;
        // double clustersCapacity;
        float inferiorLimit;
        float upperLimit;
        float currentLimit;
        float maxBenefit;

        string pathArquivoEntrada;
        string pathArquivoSaida;
        int tipoInstancia;
        int fileType;


    public:
        //Constructor
        Graph(int order, bool directed, bool weighted_edge, bool weighted_node);
        Graph(int order, int cluster, string clusterType, vector<tuple<int, int>> clustersLimits);
        // Graph(int order, int cluster, double clustersCapacity);
        Graph(int inferiorLimit, int upperLimit);
        //Destructor
        ~Graph();

        //Getters
        int getOrder();
        int getNumberEdges();
        bool getDirected();
        bool getWeightedEdge();
        bool getWeightedNode();
        Node *getFirstNode();
        Node *getLastNode();

        //Other methods
        void insertNode(int id);
        void insertNodeAndWeight(int id, int weight);
        void insertEdge(int id, int target_id, float weight);
        void removeNode(int id);
        bool searchNode(int id);
        Node *getNode(int id);
        bool searchEdge(int id, int target_id);

        // Algoritimos Gulosos
        void agmGuloso();
        void agmGulosoRandAdap();
        void algGulosoReativo();

        //axiliar methods
        void printGraph();
        void printGraphDot(ofstream &file);
        void output(string output_file, vector<Graph*> solution, float quality);

        double calculatePenalization(int id1, int id2, Graph *cluster);

    private:
        vector<Graph*> guloso(bool random, double *result, float alfa);

        float qualidadeSolucao(float resultadoObtido);
        Node* returnValidNode(float min, float max);
        float findDistanceBetween2Nodes(int node1, int node2);
        void verifyQuality(float result);
        void imprimeCluster(vector<Graph *> solucao, int option, float resultBeneficio);
        void printNodes();
        void printNodes2();

};

#endif // GRAPH_H_INCLUDED
