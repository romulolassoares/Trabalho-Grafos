#include "Graph.h"
#include "Node.h"
#include "Edge.h"
#include <iostream>
#include <fstream>
#include <stack>
#include <queue>
#include <list>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <float.h>
#include <iomanip>

#define INFINITO 10000000

using namespace std;

/**************************************************************************************************
 * Defining the Graph's methods
 **************************************************************************************************/

// Constructor
Graph::Graph(int order, bool directed, bool weighted_edge, bool weighted_node)
{
    this->order = order;
    this->directed = directed;
    this->weighted_edge = weighted_edge;
    this->weighted_node = weighted_node;
    this->first_node = this->last_node = nullptr;
    this->number_edges = 0;
}

// Destructor
Graph::~Graph()
{
    Node *next_node = this->first_node;
    while (next_node != nullptr)
    {
        next_node->removeAllEdges();
        Node *aux_node = next_node->getNextNode();
        delete next_node;
        next_node = aux_node;
    }
}

// Getters
int Graph::getOrder()
{
    return this->order;
}

int Graph::getNumberEdges()
{
    return this->number_edges;
}

// Function that verifies if the graph is directed
bool Graph::getDirected() {
    return this->directed;
}

// Function that verifies if the graph is weighted at the edges
bool Graph::getWeightedEdge() {
    return this->weighted_edge;
}

// Function that verifies if the graph is weighted at the nodes
bool Graph::getWeightedNode() {
    return this->weighted_node;
}

Node *Graph::getFirstNode()
{
    return this->first_node;
}

Node *Graph::getLastNode()
{
    return this->last_node;
}

// Other methods
/*
    The outdegree attribute of nodes is used as a counter for the number of edges in the graph.
    This allows the correct updating of the numbers of edges in the graph being directed or not.
*/
void Graph::insertNode(int id)
{
    Node *next;
    Node *aux = nullptr;

    // Verifica se já existe algum nó
    if (this->getFirstNode() == nullptr) {
        this->first_node = new Node(id);
        this->last_node = this->getFirstNode();
    }
    else {
        if (!this->searchNode(id))
        {
            next = this->getFirstNode();
            // Procura o último nó inserido
            while (next != nullptr)
            {
                aux = next;
                next = next->getNextNode();
            }
            // Inseri o nó na última posição
            aux->setNextNode(new Node(id));
            this->last_node = this->getNode(id);
        }
    }
}

void Graph::insertEdge(int id, int target_id, float weight) {
    // Procura se o nó id existe. Se não existir insere ele no grafo
    if (!this->searchNode(id)) {
        this->insertNode(id);
        cout << "Inserindo " << id << endl;
    }
    // Procura se o nó target_id existe. Se não existir insere ele no grafo
    if (!this->searchNode(target_id)){
        this->insertNode(target_id);
        cout << "Inserindo " << target_id << endl;
    }

    Node *nodeId = this->getNode(id);
    Node *nodeTargetId = this->getNode(target_id);

    if (this->getDirected()) {
        // Cria a aresta => id -> target_id
        nodeId->insertEdge(target_id, weight);

        // Aumenta os graus de saída e de entrada
        nodeId->incrementOutDegree();
        nodeTargetId->incrementInDegree();
    }
    else {
        // Cria a aresta => id - target_id
        if(!this->searchEdge(id, target_id)) {
            nodeId->insertEdge(target_id, weight);
            nodeTargetId->insertEdge(id, weight);
            // Aumenta os graus de saída e de entrada
            nodeId->incrementOutDegree();
            nodeTargetId->incrementOutDegree();
            nodeId->incrementInDegree();
            nodeTargetId->incrementInDegree();
        }
    }
}

void Graph::removeNode(int id) {}

bool Graph::searchNode(int id)
{
    Node *node = this->getFirstNode();
    while (node != nullptr) {
        if (node->getId() == id)
            return true;
        node = node->getNextNode();
    }
    return false;
}

Node *Graph::getNode(int id)
{
    Node *node = this->getFirstNode();

    while (node != nullptr)
    {
        if (node->getId() == id)
            return node;
        node = node->getNextNode();
    }
    return nullptr;
}

// Function that prints a set of edges belongs breadth tree
//  void Graph::breadthFirstSearch(ofstream &output_file) {}
void Graph::depthFirstSearch(ofstream &output_file, int id)
{
    list<Edge> arvore, retorno;

    int *tempoDescobertaVertice = new int[this->getOrder()];
    int *tempoFinalDescobertaVertice = new int[this->getOrder()];

    int tempo = 0;

    int *pai = new int[this->getOrder()];

    for (int i = 0; i < this->getOrder(); i++)
    {
        tempoDescobertaVertice[i] = 0;
        tempoFinalDescobertaVertice[i] = 0;
        pai[i] = -1;
    }

    dfsRec(id, arvore, retorno, pai, tempo, tempoDescobertaVertice, tempoFinalDescobertaVertice);

    list<Edge>::iterator it;
    cout << "Arestas da árvore: " << endl;
    for (it = arvore.begin(); it != arvore.end(); it++)
    {
        cout << "[" << (*it).getOrigem() << "---" << (*it).getTargetId() << "]" << endl;
    }
    cout << "Arestas de Retorno: " << endl;
    for(it = retorno.begin(); it != retorno.end(); it++){
        cout << "[" << (*it).getOrigem() << "---" << (*it).getTargetId() << "]" << endl;
    }

    // Deletando vetores
    delete[] tempoDescobertaVertice;
    delete[] tempoFinalDescobertaVertice;
    delete[] pai;
}

void Graph::dfsRec(int id, list<Edge> &arvore, list<Edge> &retorno, int *pai, int tempo, int *tempoDescoberta, int *tempoFinal)
{
    tempo++;
    tempoDescoberta[id] = tempo;
    Node *no = getNode(id);

    for (Edge *adj = no->getFirstEdge(); adj != nullptr; adj = adj->getNextEdge())
    {
        Edge aresta = Edge(adj->getTargetId());
        aresta.setOrigem(id);

        if (tempoDescoberta[adj->getTargetId()] == 0)
        {
            arvore.push_back(aresta);

            pai[adj->getTargetId()] = id;

            dfsRec(adj->getTargetId(), arvore, retorno, pai, tempo, tempoDescoberta, tempoFinal);
        }
        else
        {
            if ((tempoFinal[adj->getTargetId()] == 0) && (pai[id] != adj->getTargetId())){
                retorno.push_back(aresta);
            }
        }
    }
    tempoFinal[id] = tempo;
}

// float Graph::floydMarshall(int idSource, int idTarget) {}

float Graph::dijkstra(int idSource, int idTarget)
{
    int distancia[order];

    int visitados[order];
    priority_queue<pair<int, int>,
                   vector<pair<int, int>>, greater<pair<int, int>>>
        pq;

    for (int i = 0; i < order; i++)
    {
        distancia[i] = 1;
        visitados[i] = false;
    }
    distancia[idSource] = 0;
    int custo_aresta = 0;
    // insere na fila
    pq.push(make_pair(distancia[idSource], idSource));
    while (!pq.empty())
    {
        pair<int, int> p = pq.top(); // tirando o pair do topo
        int u = p.second;            // obtém o vértice do pair
        pq.pop();                    // remove da fila
        if (visitados[u] == false)
        {
            // marca como visitado
            visitados[u] = true;
            list<pair<int, int>>::iterator it;
            {
                int v = it->first;
                custo_aresta = it->second;
                if (distancia[v] > (distancia[u] + custo_aresta))
                {
                    distancia[v] = distancia[u] + custo_aresta;
                    pq.push(make_pair(distancia[v], v));
                }
            }
        }
    }
    return distancia[idTarget];
}

// function that prints a topological sorting
//  void topologicalSorting() {}

// void breadthFirstSearch(ofstream& output_file) {}

// Graph* getVertexInduced(int* listIdNodes) {}

// Graph* agmKuskal() {}

// Graph* agmPrim() {}

int Graph::localClusteringCoefficient(int idNode) {
    Node *node = this->getNode(idNode);
    Node *node2 = nullptr;
    Edge *edge = nullptr;
    Edge *edge2 = nullptr;
    int ccValue = 0;

    if (node != nullptr) {
        edge = node->getFirstEdge();
        int idEdge;
        // Percorre a lista de adjacencia do nó idNode
        while (edge != nullptr) {
            idEdge = edge->getTargetId();
            node2 = this->getNode(idEdge);
            // Acessa a lista de adj da lista de adj de idNode
            if (node2 != nullptr) {
                edge2 = node2->getFirstEdge();
                while(edge2 != nullptr) {
                    ccValue += this->countNodeInAdjList(edge2->getTargetId(), idNode);
                    edge2 = edge2->getNextEdge();
                }
            }
            edge = edge->getNextEdge();
        }
    }
    cout << "Local Clustering Coefficient of " << idNode << " :" << ccValue << endl;
    return ccValue;
}

// Função para contar quantas vezes um nó aparece na lista de adj de um determinado nó
int Graph::countNodeInAdjList(int idNode, int idToFind) {
    Node *node = this->getNode(idNode);
    Edge *edge = nullptr;
    int value = 0;

    if (node != nullptr) {
        edge = node->getFirstEdge();
        while (edge != nullptr) {
            if (edge->getTargetId() == idToFind)
                value++;
            edge = edge->getNextEdge();
        }
    }
    return value;
}

void Graph::printGraph() {
    Node *node = this->getFirstNode();
    Edge *edge = nullptr;

    cout << "Lista de adjacência" << endl;

    if (this->getDirected()) {
        if (this->getWeightedEdge()) {
            while (node != nullptr) {
                edge = node->getFirstEdge();
                cout << node->getId() << " -> ";

                while (edge != nullptr) {
                    cout << edge->getTargetId() << " -(" << edge->getWeight() << ")-> ";
                    edge = edge->getNextEdge();
                }

                cout << "null" << endl;
                node = node->getNextNode();
            }
        }
        else {
            while (node != nullptr) {
                edge = node->getFirstEdge();
                cout << node->getId() << " -> ";

                while (edge != nullptr) {
                    cout << edge->getTargetId() << " -> ";
                    edge = edge->getNextEdge();
                }

                cout << "null" << endl;
                node = node->getNextNode();
            }
        }
    }
    else
    {
        if (this->getWeightedEdge())
        {
            while (node != nullptr)
            {
                edge = node->getFirstEdge();
                cout << node->getId() << " - ";

                while (edge != nullptr)
                {
                    cout << edge->getTargetId() << " -(" << edge->getWeight() << ")- ";
                    edge = edge->getNextEdge();
                }

                cout << "null" << endl;
                node = node->getNextNode();
            }
        }
        else
        {
            while (node != nullptr)
            {
                edge = node->getFirstEdge();
                cout << node->getId() << " -> ";

                while (edge != nullptr)
                {
                    cout << edge->getTargetId() << " - ";
                    edge = edge->getNextEdge();
                }

                cout << "null" << endl;
                node = node->getNextNode();
            }
        }
    }
}

void Graph::printGraphDot(ofstream &file) {
    if (file.is_open()) {
        cout << "Salvando o grafo" << endl;

        Node *node = this->getFirstNode();
        Edge *edge;

        // Verifica se é ou não direcionado
        if (this->getDirected()) {
            file << "digraph { \n";
        }
        else {
            file << "strict graph { \n";
        }

        // Verifica se o nó tem peso
        if (this->getWeightedNode()) {
            while (node != nullptr)
            {
                file << "   " << node->getId() << " [weight = ";
                file << node->getWeight() << "] \n";
                node = node->getNextNode();
            }
        }

        node = this->getFirstNode();

        while (node != nullptr) {
            edge = node->getFirstEdge();
            while (edge != nullptr)
            {
                file << "   " << node->getId();
                if (this->getDirected()) {
                    file << "->";
                }
                else {
                    file << "--";
                }
                file << edge->getTargetId();

                if (this->getWeightedEdge()) {
                    file << " [label=" << edge->getWeight();
                    file << ",weight=" << edge->getWeight() << "]";
                }
                file << "\n";
                edge = edge->getNextEdge();
            }
            node = node->getNextNode();
        }

        file << "} \n";
    }
    else {
        cout << "Falha ao abrir o arquivo";
    }
}