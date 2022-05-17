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

using namespace std;

/**************************************************************************************************
 * Defining the Graph's methods
 **************************************************************************************************/

// Constructor
Graph::Graph(int order, bool directed, bool weighted_edge, bool weighted_node) {
    this->order = order;
    this->directed = directed;
    this->weighted_edge = weighted_edge;
    this->weighted_node = weighted_node;
    this->first_node = this->last_node = nullptr;
    this->number_edges = 0;
}

// Destructor
Graph::~Graph() {
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
int Graph::getOrder() {
    return this->order;
}

int Graph::getNumberEdges() {
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

Node *Graph::getFirstNode() {
    return this->first_node;
}

Node *Graph::getLastNode() {
    return this->last_node;
}

// Other methods
/*
    The outdegree attribute of nodes is used as a counter for the number of edges in the graph.
    This allows the correct updating of the numbers of edges in the graph being directed or not.
*/
void Graph::insertNode(int id) {
    Node *next;
    Node *aux = nullptr;

    // Verifica se já existe algum nó
    if (this->getFirstNode() == nullptr)
    {
        this->first_node = new Node(id);
        this->last_node = this->getFirstNode();
    }
    else
    {
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
    if (!this->searchNode(id))
        this->insertNode(id);
    // Procura se o nó target_id existe. Se não existir insere ele no grafo
    if (!this->searchNode(target_id))
        this->insertNode(target_id);

    Node *nodeId = this->getNode(id);
    Node *nodeTargetId = this->getNode(target_id);

    if (this->getDirected())
    {
        // Cria a aresta => id -> target_id
        nodeId->insertEdge(target_id, weight);

        // Aumenta os graus de saída e de entrada
        nodeId->incrementOutDegree();
        nodeTargetId->incrementInDegree();
    }
    else
    {
        // Cria a aresta => id - target_id
        nodeId->insertEdge(target_id, weight);
        nodeTargetId->insertEdge(id, weight);

        // // Aumenta os graus de saída e de entrada
        // nodeId->incrementOutDegree();
        // nodeTargetId->incrementOutDegree();
        // nodeId->incrementInDegree();
        // nodeTargetId->incrementInDegree();
    }
}

void Graph::removeNode(int id) {}

bool Graph::searchNode(int id) {
    Node *node = this->getFirstNode();
    while (node != nullptr)
    {
        if (node->getId() == id)
            return true;
        node = node->getNextNode();
    }
    return false;
}

Node *Graph::getNode(int id) {
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

// float Graph::floydMarshall(int idSource, int idTarget) {}

// float Graph::dijkstra(int idSource, int idTarget) {}

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