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
struct Aresta_aux {
    int origem;
    int destino;
    int peso;
};

typedef struct {
    int id_origem;
    Node *k;
    Edge *e;
} EdgeLinkedNode;


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
    double clustersCapacity;
    float inferiorLimit;
    float upperLimit;
    float currentLimit;
    float maxBenefit;

    string pathArquivoEntrada;
    string pathArquivoSaida;
    int tipoInstancia;


public:
    //Constructor
    Graph(int order, bool directed, bool weighted_edge, bool weighted_node);
    Graph(int order, int cluster, string clusterType, vector<tuple<int, int>> clustersLimits);
    Graph(int order, int cluster, double clustersCapacity);
    Graph(int inferiorLimit, int upperLimit);
    Graph(int argc, const char **argv);

    //Destructor
    ~Graph();

    //Getters
    int getOrder();

    int getNumberEdges();

    bool getDirected();

    bool getWeightedEdge();
    vector<tuple<int, int>> define_leitura();
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

    int minimalDistance(bool visitados[], float dist[]);

    //methods phase1
    void topologicalSorting();

    void breadthFirstSearch(ofstream &output_file);

    void depthFirstSearch(ofstream &output_file, int id);

    list<int> getFechoTransitivoDireto(list<int> &fechoDireto, int *id); //fecho transitivo direto
    list<int> getFechoTransitivoIndireto(list<int> &fechoIndireto, int *id);//fecho transitivo indireto
    void imprimirFechoTransitivoDireto(ofstream &output_file, int id);//fecho transitivo direto
    void imprimirFechoTransitivoIndireto(ofstream &output_file, int id);//fecho transitivo indireto
    void agmByKruskal(ofstream &outputFile, Graph *grafo);

    void imprimirAgmByKruskal(ofstream &outputfile, int ordem, int numero_arestas, Aresta_aux arestas_finais[]);

    Graph *getVerticeInduzido(); //obter subgrafo induzido

    Graph *agmPrim();

    float floydMarshall(int idSource, int idTarget);

    float dijkstra(int idSource, int idTarget);

    float localClusteringCoefficient(int idNode);

    float averageClusteringCoefficient();

    void auxDijkstra(float *dist, int *aPercorrer, int *noAnterior, int *map, int atual);

    int mapeamento(int *map, int id);

    //methods phase1
    vector<Graph*> guloso(bool random, float *result, float alfa);
    void agmGuloso();
    void agmGulosoRandAdap();
    void agmGulosoRandReativ();

    float qualidadeSolucao(float resultadoObtido);

    float findDistanceBetween2Nodes(int node1, int node2);


    //axiliar methods
    void printGraph();

    void printGraphDot(ofstream &file);

    void output(string output_file, vector<Graph*> solution, float quality);

    void dfsRec(int id, list<Edge> &arvore, list<Edge> &retorno, int *pai, int tempo, int *tempoDescoberta, int *tempoFinal);

    void minimalPathByFloyd(int id_one, int id_two);

    void minimalSpanningTreeByPrimAlgorithm(Graph *g);


private:
    //Auxiliar methods
    bool searchListAdj(int idNode, int idToFind);

    int countNodeInAdjList(int idNode, int idToFind);

    bool arestaNaLista(Aresta_aux listEdges[], int id, int destino, int size);

    void cleanVisited();
    int type_instance;

    int getNumberInMap(int id, std::map<Node *, int> m);

    EdgeLinkedNode *getLighterEdge(list<Node *> t, list<Node *> v);
};

#endif // GRAPH_H_INCLUDED
