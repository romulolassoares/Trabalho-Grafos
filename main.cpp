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

int menu() {

    int selecao;
    cout << endl;
    cout << "   ------  MENU ------" << endl;
    cout << "[1] Algoritmo construtivo guloso" << endl;
    cout << "[2] Algoritmo construtivo guloso randomizado e adaptativo" << endl;
    cout << "[3] Algoritmo construtivo guloso randomizado reativo" << endl;
    cout << "[4] Sair" << endl;

    cin >> selecao;

    return selecao;

}

void selecionar(ofstream &output_file,int selecao, Graph *graph) {
    switch (selecao) {
        case 1: {
            graph->agmGuloso();
            break;
        }
        case 2: {
            // graph->agmGulosoRandAdap();
            break;
        }
        case 3: {
            graph->algGulosoReativo();
            break;
        }
        default: {
            cout << "Exit!!!" << endl;
        }
    }
}

int mainMenu(ofstream &output_file,Graph *graph) {
    int selecao = menu();
    while (selecao != 4) {

        if (output_file.is_open())
            selecionar(output_file,selecao, graph);
        else
            cout << "Unable to open the output_file" << endl;
        output_file << endl;
        selecao = menu();
    }
    return 0;
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
    ofstream output_file;
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



    // mainMenu(output_file, graph);
    cout << "------- GULOSO -------" << endl;
    graph->agmGuloso();
    cout << "------- GULOSO RANDOMIZADO ADAPTATIVO -------" << endl;
    graph->agmGulosoRandAdap(stof(argv[4]));
    cout << "------- GULOSO RANDOMIZADO REATIVO -------" << endl;
    graph->algGulosoReativo();
    //Fechando arquivo de entrada
    input_file.close();
    //Fechando arquivo de saída
    output_file.close();

    return 0;
}