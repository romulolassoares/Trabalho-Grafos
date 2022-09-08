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
#include <string.h>
#include <vector>
#include <algorithm>
#include <map>
#include <chrono>
#include <random>

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

Graph::Graph(int order, int cluster, string clusterType, vector<tuple<int, int>> clustersLimits) {
    // this->order = order;
    this->cluster = cluster;
    this->clusterType = clusterType;
    this->clustersLimits = clustersLimits;
    this->first_node = this->last_node = nullptr;
    this->weighted_edge = 1;
    this->weighted_node = 1;
}

// Graph::Graph(int order, int cluster, double clustersCapacity) {
//     // this->order = order;
//     this->cluster = cluster;
//     this->clustersCapacity = clustersCapacity;
//     this->first_node = this->last_node = nullptr;
//     this->weighted_edge = 1;
//     this->weighted_node = 1;
// }

Graph::Graph(int inferiorLimit, int upperLimit) {
    this->first_node = this->last_node = nullptr;
    this->weighted_edge = 1;
    this->weighted_node = 1;
    this->inferiorLimit = inferiorLimit;
    this->upperLimit = upperLimit;
    this->currentLimit = 0;
}

// Destructor
Graph::~Graph() {
    Node *next_node = this->first_node;
    while (next_node != nullptr) {
        next_node->removeAllEdges();
        Node *aux_node = next_node->getNextNode();
        delete next_node;
        next_node = aux_node;
    }
}

long inline size_arq(fstream &arq) {
    arq.ignore(std::numeric_limits<std::streamsize>::max());
    std::streamsize size= arq.gcount();
    arq.clear();
    arq.seekg(0, std::ios_base::beg);

    return size;
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
    if (this->getFirstNode() == nullptr) {
        this->first_node = new Node(id);
        this->last_node = this->getFirstNode();
    } else {
        if (!this->searchNode(id)) {
            next = this->getFirstNode();
            // Procura o último nó inserido
            while (next != nullptr) {
                aux = next;
                next = next->getNextNode();
            }
            // Inseri o nó na última posição
            aux->setNextNode(new Node(id));
            this->last_node = this->getNode(id);
        }
    }
}

void Graph::insertNodeAndWeight(int id, int weight) {
    Node *next;
    Node *aux = nullptr;

    // Verifica se já existe algum nó
    if (this->getFirstNode() == nullptr) {
        this->first_node = new Node(id);
        // cout << "144 - id:" << this->first_node->getId() << endl;
        this->first_node->setWeight(weight);
        // this->last_node = this->getFirstNode();
        this->order++;
    } else {
        if (!this->searchNode(id)) {
            next = this->getFirstNode();
            // Procura o último nó inserido
            while (next != nullptr) {
                aux = next;
                next = next->getNextNode();
            }
            // Inseri o nó na última posição
            aux->setNextNode(new Node(id));
            this->last_node = this->getNode(id);
            this->last_node->setWeight(weight);
            // cout << "TEST" << endl;
            this->order++;
        }
    }
}

void Graph::insertEdge(int id, int target_id, float weight) {
    // Procura se o nó id existe. Se não existir insere ele no grafo
    if (!this->searchNode(id)) {
        this->insertNode(id);
        // cout << "Inserindo " << id << endl;
    }
    // Procura se o nó target_id existe. Se não existir insere ele no grafo
    if (!this->searchNode(target_id)) {
        this->insertNode(target_id);
        // cout << "Inserindo " << target_id << endl;
    }

    Node *nodeId = this->getNode(id);
    Node *nodeTargetId = this->getNode(target_id);

    if (this->getDirected()) {
        // Cria a aresta => id -> target_id
        nodeId->insertEdge(target_id, weight);

        // Aumenta os graus de saída e de entrada
        nodeId->incrementOutDegree();
        nodeTargetId->incrementInDegree();
    } else {
        // Cria a aresta => id - target_id
        if (!this->searchEdge(id, target_id)) {
            nodeId->insertEdge(target_id, weight);
            nodeTargetId->insertEdge(id, weight);
            // Aumenta os graus de saída e de entrada
            nodeId->incrementOutDegree();
            nodeTargetId->incrementOutDegree();
            nodeId->incrementInDegree();
            nodeTargetId->incrementInDegree();
        }
    }
    this->number_edges++;
}

bool Graph::searchEdge(int id, int target_id) {
    Node *node = this->getNode(id);
    Node *targetNode = this->getNode(target_id);
    Edge *edge = nullptr;

    edge = node->getFirstEdge();
    while (edge != nullptr) {
        if (edge->getTargetId() == target_id) {
            return true;
        }
        edge = edge->getNextEdge();
    }

    return false;
}

void Graph::removeNode(int id) {}

bool Graph::searchNode(int id) {
    Node *node = this->getFirstNode();
    while (node != nullptr) {
        if (node->getId() == id)
            return true;
        node = node->getNextNode();
    }
    return false;
}

Node *Graph::getNode(int id) {
    Node *node = this->getFirstNode();

    while (node != nullptr) {
        if (node->getId() == id)
            return node;
        node = node->getNextNode();
    }
    return nullptr;
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
        } else {
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
    } else {
        if (this->getWeightedEdge()) {
            while (node != nullptr) {
                edge = node->getFirstEdge();
                cout << node->getId() << " - ";

                while (edge != nullptr) {
                    cout << edge->getTargetId() << " -(" << edge->getWeight() << ")- ";
                    edge = edge->getNextEdge();
                }

                cout << "null" << endl;
                node = node->getNextNode();
            }
        } else {
            while (node != nullptr) {
                edge = node->getFirstEdge();
                cout << node->getId() << " -> ";

                while (edge != nullptr) {
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

        vector<bool> visited;

        while (node != nullptr) {
            visited.push_back(false);
            node = node->getNextNode();
        }
        node = this->getFirstNode();
        // Verifica se é ou não direcionado
        if (this->getDirected()) {
            file << "digraph { \n";
        } else {
            file << "strict graph { \n";
        }

        // Verifica se o nó tem peso
        if (this->getWeightedNode() == 1) {
            while (node != nullptr) {
                file << "   " << node->getId() << " [weight = ";
                file << node->getWeight() << "] \n";
                node = node->getNextNode();
            }
        }

        node = this->getFirstNode();

        while (node != nullptr) {
            if(!visited.at(node->getId())) {
                visited.at(node->getId()) = true;
                edge = node->getFirstEdge();
                while (edge != nullptr) {
                    if(!visited.at(edge->getTargetId())) {
                        file << "   " << node->getId();
                        if (this->getDirected()) {
                            file << " -> ";
                        } else {
                            file << " -- ";
                        }
                        file << edge->getTargetId();

                        if (this->getWeightedEdge()) {
                            file << " [label=" << edge->getWeight();
                            file << ",weight=" << edge->getWeight() << "]";
                        }
                        file << "\n";
                    }
                    edge = edge->getNextEdge();
                }
            }
            node = node->getNextNode();
        }

        file << "} \n";
    } else {
        cout << "Falha ao abrir o arquivo";
    }
}

vector<Graph*> Graph::guloso(bool random, double *result, float alfa) {
    vector<Graph*> solution;
    *result = 0;
    vector<bool> visitedNodes;
    int countVisitedNodes = 0;
    vector<vector<bool>> visitedEdges;
    // Preenche os veteros de nós visitados e e arestas visitadas
    visitedEdges.resize(this->getOrder());
    for(int i = 0; i < this->getOrder(); i++) {
        visitedEdges.at(i).resize(this->getOrder());
        visitedNodes.push_back(false);
    }

    float resultBenefit = 0;

    // Inicializa os clusters
    for(int i = 0; i < this->cluster; i++) {
        tuple<int, int> limits = this->clustersLimits.at(i);
        Graph *cluster = new Graph(get<0>(limits), get<1>(limits));
        solution.push_back(cluster);
    }

    // Sorteia o nó inicial de cada cluster
    for(int i = 0; i < this->cluster; i++) {
        Node *node;

        if(random) {
            node = this->returnValidNode(0.0f, alfa * (this->getOrder()- 1));
            // position = (int)(rand() % (int)(alfa*this->getOrder()));
        } else {
            node = this->getNode(i);
        }
        int position = node->getId();       

        if(node == nullptr || visitedNodes.at(position) == true) {
            i--;
            continue;
        }

        visitedNodes.at(position) = true;
        countVisitedNodes++;

        // solution.at(i)->insertNodeAndWeight(node->getId(), node->getWeight());
        solution.at(i)->insertNodeAndWeight(position, node->getWeight());
        solution.at(i)->currentLimit += node->getWeight();
    }

    priority_queue<tuple<float, int, int>> candidates;
    
    // Valida o limite inferior
    for(int i = 0; i < this->cluster; i++) {
        Graph *clusterGraph = solution.at(i);
        int auxId = clusterGraph->getFirstNode()->getId();
        
        while(clusterGraph->currentLimit < clusterGraph->inferiorLimit || candidates.empty()) {
            
            for(int j = 0; j < this->getOrder(); j++) {
                if(!visitedNodes.at(j)) {
                    float auxDistance = findDistanceBetween2Nodes(auxId, j);
                    tuple<float, int, int> candidate(auxDistance, auxId, j);
                    double p = VerificaQualidade(auxId, j, clusterGraph);
                    if(p > 100) {
                        candidates.push(candidate);
                    }
                }
            }
            
            tuple<float, int, int> candidate = candidates.top();
            float distance = get<0>(candidate);
            tuple<int, int> twoNodes(get<1>(candidate), get<2>(candidate));
            candidates.pop();

            Node *graphNode1 = clusterGraph->getNode(get<0>(twoNodes));
            Node *graphNode2 = this->getNode(get<1>(twoNodes));

            if(graphNode1 == nullptr) {
                graphNode1 = clusterGraph->getNode(get<1>(twoNodes));
                graphNode2 = this->getNode(get<0>(twoNodes));
            }

            if(
                (clusterGraph->currentLimit + graphNode2->getWeight() <= clusterGraph->upperLimit && 
                visitedNodes.at(graphNode2->getId()) == false)
            ) {
                clusterGraph->insertNodeAndWeight(graphNode2->getId(), graphNode2->getWeight());
                clusterGraph->maxBenefit += distance;
                // resultBenefit += distance;

                visitedEdges.at(graphNode2->getId()).at(graphNode1->getId()) = true;
                visitedEdges.at(graphNode1->getId()).at(graphNode2->getId()) = true;

                Node *clusterNode = clusterGraph->getFirstNode();
                while(clusterNode != nullptr) {
                    if(
                        visitedEdges.at(graphNode2->getId()).at(clusterNode->getId()) == false && 
                        visitedEdges.at(clusterNode->getId()).at(graphNode2->getId()) == false
                    ) {
                        float auxDistance = findDistanceBetween2Nodes(graphNode2->getId(), clusterNode->getId());
                        clusterGraph->maxBenefit += auxDistance;
                        resultBenefit += auxDistance;
                    }
                    clusterNode = clusterNode->getNextNode();
                }
                resultBenefit += distance;

                clusterGraph->currentLimit += graphNode2->getWeight();
                visitedNodes.at(graphNode2->getId()) = true;
                countVisitedNodes++;
                auxId = graphNode2->getId();
            }
        }
    }

    // Atualiza nós que não estão no cluster
    for(int i = 0; i < this->getOrder(); i++) {
        for(int j = 0; j < this->getOrder(); j++) {
            if(visitedNodes.at(i) == false || visitedNodes.at(j) == false) {
                float auxDistance = findDistanceBetween2Nodes(i, j);
                tuple<float, int, int> candidate(auxDistance, i, j);
                candidates.push(candidate);
            }
        }
    }

    // Valida o limite superior
    while(countVisitedNodes < this->getOrder() && !candidates.empty()) {
        tuple<float, int, int> candidate = candidates.top();
        float distance = get<0>(candidate);
        tuple<int, int> twoNodes(get<1>(candidate), get<2>(candidate));
        candidates.pop();

        if(
            !(visitedNodes.at(get<0>(twoNodes)) == true && visitedNodes.at(get<1>(twoNodes)) == true) && 
            !(visitedNodes.at(get<0>(twoNodes)) == false && visitedNodes.at(get<1>(twoNodes)) == false)
        ) {
            for(int i = 0; i < this->cluster; i++) {
                Graph *cluster = solution.at(i);

                Node *graphNode1 = cluster->getNode(get<0>(twoNodes));
                Node *graphNode2 = this->getNode(get<1>(twoNodes));

                if(graphNode1 == nullptr) {
                    graphNode1 = cluster->getNode(get<1>(twoNodes));
                    graphNode2 = this->getNode(get<0>(twoNodes));
                }

                if(
                    cluster->currentLimit + graphNode2->getWeight() <= cluster->upperLimit &&
                    visitedNodes.at(graphNode2->getId()) == false
                ) {
                    cluster->insertNodeAndWeight(graphNode2->getId(), graphNode2->getWeight());
                    cluster->maxBenefit += distance;
                    resultBenefit += distance;

                    visitedEdges.at(get<0>(twoNodes)).at(get<1>(twoNodes)) = true;
                    visitedEdges.at(get<1>(twoNodes)).at(get<0>(twoNodes)) = true;

                    Node *clusterNode = cluster->getFirstNode();
                    while(clusterNode != nullptr) {
                        if(
                            visitedEdges.at(graphNode2->getId()).at(clusterNode->getId()) == false && 
                            visitedEdges.at(clusterNode->getId()).at(graphNode2->getId()) == false
                        ) {
                            float auxDistance = findDistanceBetween2Nodes(graphNode2->getId(), clusterNode->getId());
                            cluster->maxBenefit += auxDistance;
                            resultBenefit += auxDistance;
                        }
                        clusterNode = clusterNode->getNextNode();
                    }
                    cluster->currentLimit += graphNode2->getWeight();
                    visitedNodes.at(graphNode2->getId()) = true;
                    countVisitedNodes++;
                }
            }
        }
    }

    *result = resultBenefit;
    return solution;
}

// GULOSOS
void Graph::agmGuloso() {
    cout << std::setprecision(2) << std::fixed;
    auto start = chrono::steady_clock::now();

    double result = 0;
    vector<Graph*> sol = guloso(0, &result, 0);

    auto end = chrono::steady_clock::now();
    cout << "Demorou  "
            << chrono::duration_cast<chrono::milliseconds>(end - start).count()
            << " ms para executar." << endl;
    // cout << "Qualidade Solucao: " << qualidadeSolucao(result) << "%" << endl;

    cout << "Qualidade Obtida: " << result << endl;
    if (result > 0) {
        cout << "Conseguiu alguma solucao viavel" << endl;
    } else {
        cout << "Nao conseguiu nenhuma solucao viavel" << endl;
    }

    // imprimeCluster(sol, 2, result);

    // imprimeCluster(sol, 2, result);
    // output("AlgoritmoGuloso.txt", sol, qualidadeSolucao(result));
}

void Graph::agmGulosoRandAdap(float x){
    auto start = chrono::steady_clock::now();

    float melhor = 0;
    double resultado = 0;
    int criterio_parada= 10;
    float cof_randomizacao = x;


    vector<Graph *> solution, best_solution;

    cout << "Coeficiente de randomizacao: " << cof_randomizacao << endl;

    for (int i = 0; i < criterio_parada; i++) {
        solution = guloso(1, &resultado,  cof_randomizacao);
        // cout << "i:" << i << endl;
        if (resultado > melhor) {
            melhor = resultado;
            best_solution = solution;
        }
    }

    cout << std::setprecision(2) << std::fixed;
    auto end = chrono::steady_clock::now();
    cout << "Demorou  "
            << chrono::duration_cast<chrono::milliseconds>(end - start).count()
            << " ms para executar." << endl;

    cout << "Beneficio da melhor solucao: " << melhor <<endl;
    if (melhor > 0) {
        cout << "O guloso randomizado obteve alguma solucao viavel" << endl;
    } else {
        cout << "O guloso randomizado reativo nao obteve nenhuma solucao viavel" << endl;
    }

    //output("AlgoritmoGulosoRandomizadoAdaptativo.txt", melhorSol, qualidadeSolucao(maior));
}

float RandomAlfa(vector<float> &alfas) {
    //usando a biblioteca random para gerar números mais aleatórios
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0,9);
    return alfas[dis(gen)];
}

void Graph::algGulosoReativo() {
    auto start = chrono::steady_clock::now();

    vector<float> alfas{0.5f, 0.55f, 0.6f, 0.65f, 0.7f, 0.75f, 0.8f, 0.85f, 0.9f, 0.95f};
    vector<Graph *> solution, melhorSol;
    float MelhorSolution = 0;
    float melhorBeneficio= 0;
    int criterio_parada = 10;
    double result = 0;

    for (int i = 1; i <= criterio_parada; i++) {
        float cof_randomizado = RandomAlfa(alfas);
        
        result = 0;
        cout << "Coeficiente de Randomização: " << cof_randomizado << endl;
        solution = guloso(1, &result, cof_randomizado);

        if (result > melhorBeneficio) {
            melhorBeneficio = result;
            melhorSol = solution;
            MelhorSolution = melhorBeneficio;
        }
        
        for (auto &i : solution) {
            delete i;
        }
    }

    cout << std::setprecision(2) << std::fixed;
    auto end = chrono::steady_clock::now();
    cout << "Demorou  "
            << chrono::duration_cast<chrono::milliseconds>(end - start).count()
            << " ms para executar." << endl;
            
    cout << "Beneficio da Melhor Solucao: " << MelhorSolution << endl;
    if (MelhorSolution > 0) {
        cout << "Conseguiu alguma solucao viavel" << endl;
    } else {
        cout << "Nao conseguiu nenhuma solucao viavel" << endl;
    }
    result = 0;
    //output("AlgoritmoGulosoRandomizadoReativo.txt", melhorSol, qualidadeSolucao(auxSolBest));
}

float Graph::findDistanceBetween2Nodes(int node1, int node2) {
    // START - Get Distance between two nodes
    Node *nodeAux1 = this->getNode(node1);
    Node *nodeAux2 = this->getNode(node2);
    Edge *edgeAux;
    float auxDistance = 0;
    if(nodeAux1 != nullptr) {
        edgeAux = nodeAux1->getFirstEdge();
        while (edgeAux != nullptr) {
            if(edgeAux->getTargetId() == nodeAux2->getId())
                auxDistance = edgeAux->getWeight();
            edgeAux = edgeAux->getNextEdge();
        }
    }
    // END - Get Distance between two nodes
    // cout << "Distância (findDistanceBetween2Nodes): " << auxDistance << endl;
    return auxDistance;
}

Node* Graph::returnValidNode(float min, float max) {
    float random = ((float)rand()) / (float)RAND_MAX;
    float diff = max - min;
    float r = random * diff;
    int idRandom = min + r;
    // find the node with at idRandom
    list<int> candidates;
    for (int i = 0; i < this->getOrder(); i++) {
        candidates.push_back(i);
    }

    int i = 0;
    for (auto it = candidates.begin(); it != candidates.end(); ++it)
    {
        if (i == idRandom)
        {
            auto node = this->getNode(*it);
            candidates.erase(it);
            return node;
        }
        i++;
    }

    return nullptr;
}

void Graph::imprimeCluster(vector<Graph *> solucao, int option, float resultBeneficio)
{
    float totalBeneficio = 0.0;

    for (int i = 0; i < this->cluster; i++)
    {
        Graph *cluster = solucao[i];

        if (option == 2)
        {
            cout << "************* CLUSTER " << i + 1 << " *************" << endl;
            cout << "Beneficio " << cluster->maxBenefit << endl;
            totalBeneficio += cluster->maxBenefit;
        }

        if (option == 1)
        {
            cluster->printNodes();
        }
        else if (option == 2)
        {
            cluster->printNodes2();
        }

        cout << endl;
    }

    if (option == 2)
    {
        cout << std::setprecision(2) << std::fixed;
    }
    cout << "\n\nBeneficio final: " << totalBeneficio << endl;
}

void Graph::printNodes2()
{
    Node *node = this->first_node;
    int cont = 0;

    cout << "Limite | " << this->inferiorLimit << " <= " << this->currentLimit << " <= " << this->upperLimit << ""
         << endl;

    while (node != nullptr)
    {
        cout << node->getId() << ",";
        node = node->getNextNode();
        cont++;
    }
}

void Graph::printNodes()
{
    Node *node = this->first_node;
    int cont = 0;

    while (node != nullptr)
    {
        cout << node->getId() << ",";
        node = node->getNextNode();
        cont++;
    }
}

void Graph::output(string output_file, vector<Graph*> solution, float quality){
    fstream output_File(output_file, ios::in);

    output_File << "Solução da Clusterização: " << endl;

    for (int i = 0; i < this->cluster; i++) {
        Graph *cluster = solution[i];
        output_File << "=============== CLUSTER =============== " << endl;

        output_File << "Beneficio do Cluster: " << cluster->maxBenefit << endl;

        Node *node = cluster->getFirstNode();

        output_File << "Limite: " << cluster->inferiorLimit << " <->"/*<< cluster->getLimit << "<->"*/ << cluster->upperLimit << endl;

        output_File << "Nós do Cluster: " << endl;
        while (node != nullptr) {
            output_File << node->getId() << ";";
            node = node->getNextNode();
        }

        output_File << endl;
    }
    output_File << "Qualidade da solução: " << quality << "%" << endl;

    cout << "O arquivo " << output_file << " foi gerado com sucesso.";
}

double Graph::VerificaQualidade(int id1, int id2, Graph *cluster) {
    Node *node1 = this->getNode(id1);
    Node *node2 = this->getNode(id2);

    double totalWeight = 0;

    if(node1 == nullptr) {
        totalWeight = node2->getWeight();
    } else if(node2 == nullptr) {
        totalWeight = node1->getWeight();
    } else {
        totalWeight = node1->getWeight() + node2->getWeight();
    }

    double aux = totalWeight + cluster->currentLimit;
    double x, y;

    if(cluster->inferiorLimit == 0) {
        x = 2 * aux / cluster->upperLimit;
    } else {
        x = aux / cluster->inferiorLimit;
    }

    y = aux / cluster->upperLimit;

    float A = 1/4;
    float B = 10;
    double value = (A * x) + (this->getOrder() * B * y);

    return value;
}

