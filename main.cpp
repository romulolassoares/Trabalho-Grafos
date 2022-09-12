#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <utility>
#include <tuple>
#include <iomanip>
#include <stdlib.h>
#include <chrono>
#include <vector>

#include "Graph.h"
#include "Node.h"

using namespace std;

Graph *leituraRR(ifstream &input_file) {

    //Variáveis para auxiliar na criação dos nós no Grafo
    int idNodeSource;
    int idNodeTarget;
    int order; // Number of elements
    int cluster; // Number of clusters
    string clusterType; // Type of cluster
    vector<tuple<int, int>> clustersLimits; // Array of clusters limits
    int lower;
    int upper;
    char w;
    int elementA, elementB;
    float distance;
    vector<tuple<int, int, float>> elements;

    input_file >> order;
    input_file >> cluster;
    input_file >> clusterType;

    // Get cluster limits
    for (int i = 0; i < cluster; i++) {
        input_file >> lower >> upper;
        tuple<int, int> t(lower, upper);
        clustersLimits.push_back(t);
    }

    input_file >> w;

    Graph *graph = new Graph(order, cluster, clusterType, clustersLimits);

    // Get nodes weights
    int aux;
    for (int i = 0; i < order; i++) {
        input_file >> aux;
        graph->insertNodeAndWeight(i, aux);
    }

    // Get elements and distance
    while (input_file >> elementA >> elementB >> distance) {
        tuple<int, int, float> t(elementA, elementB, distance);
        graph->insertEdge(elementA, elementB, distance);
        elements.push_back(t);
    }

    return graph;
}

Graph *leituraHandover(ifstream &input_file) {
    //Variáveis para auxiliar na criação dos nós no Grafo
    int order; // Number of elements
    int cluster; // Number of clusters
    string clusterType = ""; // Type of cluster
    vector<tuple<int, int>> clustersLimits; // Array of clusters limits
    double upper;
    vector<vector<int>> elements;
    int edgeWeight;

    input_file >> order;
    input_file >> cluster;
    input_file >> upper;

    for (int i = 0; i < cluster; i++) {
        tuple<int, int> t(0, upper);
        clustersLimits.push_back(t);
    }

    Graph *graph = new Graph(order, cluster, clusterType, clustersLimits);

    // Get nodes weights
    double aux;
    for (int i = 0; i < order; i++) {
        input_file >> aux;
        graph->insertNodeAndWeight(i, aux);
    }

    string linha;
    for(int i = 0; i < order; i++) {
        vector<int> line;
        for(int j = 0; j < order; j++) {
            input_file >> edgeWeight;
            graph->insertEdge(i, j, edgeWeight);
        }
        elements.push_back(line);
    } 

    return graph;
}

int main(int argc, char const *argv[]) {
    int fileType;
    srand(time(nullptr));

    string program_name(argv[0]);
    string input_file_name(argv[1]);

    string instance;
    if (input_file_name.find("v") <= input_file_name.size()) {
        string instance = input_file_name.substr(input_file_name.find("v"));
        cout << "Running " << program_name << " with instance " << instance << " ... " << endl;
    }

    //Abrindo arquivo de entrada
    ifstream input_file;
    fstream output_file;
    input_file.open(argv[1], ios::in);
    output_file.open(argv[2], ios::out | ios::trunc);
    fileType = atoi(argv[3]);

    Graph *graph;
    if (input_file.is_open()) {
        string fileName = argv[1];
        auto start = chrono::steady_clock::now();
        if(fileType == 0) {
            graph = leituraRR(input_file);
        } else if(fileType == 1) {
            graph = leituraHandover(input_file);
        } else {
            cout << "Opção errada" << endl;
        }
        auto end = chrono::steady_clock::now();
        cout << "Demorou  "
             << chrono::duration_cast<chrono::milliseconds>(end - start).count()
             << " ms para ler o arquivo de entrada." << endl;
        // graph->printGraphDot(output_file);

    } else
        cout << "Unable to open " << argv[1];


    output_file << "------- GULOSO -------" << endl;
    graph->agmGuloso(output_file);
    output_file << "------- GULOSO RANDOMIZADO ADAPTATIVO -------" << endl;
    graph->agmGulosoRandAdap(stof(argv[4]), output_file);
    output_file << "------- GULOSO RANDOMIZADO REATIVO -------" << endl;
    graph->algGulosoReativo(output_file);

    //Fechando arquivo de entrada
    input_file.close();
    //Fechando arquivo de saída
    output_file.close();

    return 0;
}