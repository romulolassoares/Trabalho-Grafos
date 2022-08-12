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
    vector<int> weights;
    char w;
    int elementA, elementB;
    float distance;
    vector<tuple<int, int, float>> elements;

    //Criando objeto grafo

    //Pegando a ordem do grafo
    input_file >> order;
    input_file >> cluster;
    input_file >> clusterType;
    
    // Graph *graph = new Graph(order, 0, 1, 1);

    // cout << order << " " << cluster << " " << clusterType << endl;

    // Get cluster limits
    for (int i = 0; i < cluster; i++) {
        input_file >> lower >> upper;
        tuple<int, int> t(lower, upper);
        // cout << get<0>(t) << ";" << get<1>(t) << " - ";
        clustersLimits.push_back(t);
    }

    cout << endl;

    input_file >> w;

    Graph *graph = new Graph(order, cluster, clusterType, clustersLimits);

    // Get nodes weights
    int aux;
    for (int i = 0; i < order; i++) {
        input_file >> aux;
        // cout << "weights[" << i << "]" << aux << endl;
        graph->insertNodeAndWeight(i, aux);
        weights.push_back(aux);
    }

    // Get elements and distance
    while (input_file >> elementA >> elementB >> distance) {
        tuple<int, int, float> t(elementA, elementB, distance);
        // cout << get<0>(t) << ";" << get<1>(t) << ";" << get<2>(t) << " - ";
        graph->insertEdge(elementA, elementB, distance);
        elements.push_back(t);
    }


    // graph->agmGuloso();
    // float result = 0;
    // vector<Graph*> sol = graph->guloso(0, &result, 0);

    return graph;
}

Graph *leituraHandover(ifstream &input_file) {

    //Variáveis para auxiliar na criação dos nós no Grafo
    int idNodeSource;
    int idNodeTarget;
    int order; // Number of elements
    int cluster; // Number of clusters
    string clusterType; // Type of cluster
    double clustersCapacity; // Array of clusters limits
    vector<tuple<int, int>> clustersLimits; // Array of clusters limits
    int lower;
    int upper;
    vector<int> weights;
    char w;
    int elementA, elementB;
    float distance;
    // vector<tuple<int, int, float>> elements;
    vector<vector<int>> elements;


    //Criando objeto grafo

    //Pegando a ordem do grafo
    input_file >> order;
    input_file >> cluster;

    // Get cluster limits
    input_file >> upper;
    for (int i = 0; i < cluster; i++) {
        tuple<int, int> t(0, upper);
        // cout << get<0>(t) << ";" << get<1>(t) << " - ";
        clustersLimits.push_back(t);
    }

    // cout << order << " " << cluster << " " << clusterType << " " << clustersCapacity << endl;

    cout << endl;

    Graph *graph = new Graph(order, cluster, clusterType, clustersLimits);

    // Get nodes weights
    double aux;
    for (int i = 0; i < order; i++) {
        input_file >> aux;
        // cout << "weights[" << i << "]: " << aux << endl;
        graph->insertNodeAndWeight(i, aux);
        weights.push_back(aux);
    }

    int y;
    input_file >> y;

    string linha;
    for(int i = 0; i < order; i++) {
        vector<int> line;
        for(int j = 0; j < order; j++) {
            getline(input_file, linha, ' ');
            line.push_back(stof(linha));
            graph->insertEdge(i, j, line.at(j));
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
            graph->agmGulosoRandAdap();
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

    int selecao = 1;

    while (selecao != 0) {
        selecao = menu();

        if (output_file.is_open())
            selecionar(output_file,selecao, graph);

        else
            cout << "Unable to open the output_file" << endl;


        output_file << endl;

    }

    return 0;
}


int main(int argc, char const *argv[]) {

    int fileType;
    srand(time(nullptr));
    //Verificação se todos os parâmetros do programa foram entrados
    // if (argc != 6) {

    //     cout
    //             << "ERROR: Expecting: ./<program_name> <input_file> <output_file> <instance>"
    //             << endl;
    //     return 1;

    // }

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
        // cout << graph << endl;
        // graph->printGraph();
        graph->printGraphDot(output_file);

    } else
        cout << "Unable to open " << argv[1];

    mainMenu(output_file, graph);

    //Fechando arquivo de entrada
    input_file.close();

    //Fechando arquivo de saída
    output_file.close();

    return 0;
}





