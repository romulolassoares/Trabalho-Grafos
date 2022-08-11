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

    // cout << order << " " << cluster << " " << clusterType << endl;
    graph->agmGuloso(clustersLimits);
    return graph;
    // return graph;
}

Graph *leituraHandover(ifstream &input_file) {

    //Variáveis para auxiliar na criação dos nós no Grafo
    int idNodeSource;
    int idNodeTarget;
    int order; // Number of elements
    int cluster; // Number of clusters
    string clusterType; // Type of cluster
    double clustersCapacity; // Array of clusters limits

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
    

    input_file >> clustersCapacity;

    // cout << order << " " << cluster << " " << clusterType << " " << clustersCapacity << endl;

    cout << endl;

    Graph *graph = new Graph(order, cluster, clustersCapacity);

    // Get nodes weights
    double aux;
    for (int i = 0; i < order; i++) {
        input_file >> aux;
        // cout << "weights[" << i << "]: " << aux << endl;
        graph->insertNodeAndWeight(i, aux);
        weights.push_back(aux);
    }

    for(int i = 0; i < order; i++) {
        vector<int> line;
        for(int j = 0; j < order; j++) {
            int aux;
            input_file >> aux;
            line.push_back(aux);
        }
        elements.push_back(line);
    }
    for(int i = 0; i < order; i++) {
        vector<int> line = elements.at(i);
        for(int j = 0; j < order; j++) {
            if(line.at(j) != 0) {
                graph->insertEdge(i, j, line.at(j));
            }
        }
    }
    // // Get elements and distance
    // while (input_file >> elementA >> elementB >> distance) {
    //     tuple<int, int, float> t(elementA, elementB, distance);
    //     // cout << get<0>(t) << ";" << get<1>(t) << ";" << get<2>(t) << " - ";
    //     graph->insertEdge(elementA, elementB, distance);
    //     elements.push_back(t);
    // }

    // cout << order << " " << cluster << " " << clusterType << endl;

    return graph;
    // return graph;
}

Graph *leituraInstancia(ifstream &input_file, int directed, int weightedEdge, int weightedNode) {

    //Variáveis para auxiliar na criação dos nós no Grafo
    int idNodeSource;
    int idNodeTarget;
    int order;
    int numEdges;

    //Pegando a ordem do grafo
    input_file >> order;

    //Criando objeto grafo
    Graph *graph = new Graph(order, directed, weightedEdge, weightedNode);

    //Leitura de arquivo
    while (input_file >> idNodeSource >> idNodeTarget) {

        graph->insertEdge(idNodeSource, idNodeTarget, 0);

    }

    return graph;
}

int menu() {

    int selecao;
    cout << endl;
    cout << "   ------  MENU ------" << endl;
    cout << "[1] Fecho transitivo direto de um vertice" << endl;
    cout << "[2] Fecho transitivo indireto de um vertice" << endl;
    cout << "[3] Coeficiente de agrupamento local de um vertice" << endl;
    cout << "[4] Coeficiente de agrupamento medio do grafo" << endl;
    cout << "[5] Caminho Minimo entre dois vertices - Dijkstra " << endl;
    cout << "[6] Caminho Minimo entre dois vertices - Floyd" << endl;
    cout << "[7] Arvore Geradora Minima - Prim" << endl;
    cout << "[8] Arvore Geradora Minima - Kruskal " << endl;
    cout << "[9] Caminhamento em profundidade " << endl;
    cout << "[10] Algoritmo construtivo guloso " << endl;
    cout << "[11] Algoritmo construtivo guloso randomizado e adaptativo " << endl;
    cout << "[12] Algoritmo construtivo guloso randomizado reativo " << endl;
    cout << "[0] Sair" << endl;

    cin >> selecao;

    return selecao;

}

void selecionar(ofstream &output_file,int selecao, Graph *graph,vector<tuple<int, int>> limite_dos_clusters ) {

    switch (selecao) {

        //Fecho transitivo direto de um vértice ;
        case 1: {
            int id;
            cout << "Digite o ID do vertice para seu fecho transitivo direto: ";
            cin >> id;
            graph->imprimirFechoTransitivoDireto(output_file, id);
            break;
        }
            //Fecho transitivo indireto de um vértice;
        case 2: {
            int id;
            cout << "Digite o ID do vertice para seu fecho transitivo indireto: ";
            cin >> id;
            graph->imprimirFechoTransitivoIndireto(output_file, id);
            break;
        }

            //Coeficiente de agrupamento local de um vértice;
        case 3: {
            int id;
            cout << "Digite o ID do vertice: ";
            cin >> id;
            cout << "Coeficiente do nó " << id << ": " << graph->localClusteringCoefficient(id) << endl;
            break;
        }
            //Coeficiente de agrupamento médio do grafo;
        case 4: {
            cout << "Coeficiente Medio de agrupamento do grafo";
            cout << " = " << graph->averageClusteringCoefficient() << endl;
            break;
        }
            // Djkstra
            // Caminho Mínimo entre dois vértices - Dijkstra ;
        case 5: {
            int id1, id2;
            cout << "Digite o vértcie de inicio: ";
            cin >> id1;
            cout << "Digite o vértcie final: ";
            cin >> id2;
            float distancia = graph->dijkstra(id1, id2);
            cout << "A distancia entre " << id1 << " e " << id2 << " é de: " << distancia << endl;
            break;
        }


        case 6: {
            int id_one, id_two;
            cout << "Digite o id do vertice de origem: ";
            cin >> id_one;
            cout << "Digite o id do vertice de destino: ";
            cin >> id_two;
            graph->minimalPathByFloyd(id_one, id_two);
            break;
        }
            //Árvore Geradora Mínima - Prim;
        case 7: {
            cout << "Árvore Geradora Mínima - Prim" << endl;
            graph->minimalSpanningTreeByPrimAlgorithm(graph->getVerticeInduzido());
            break;
        }
            //Árvore Geradora Mínima - Kruskal;
        case 8: {
            graph->agmByKruskal(output_file, graph->getVerticeInduzido());
            break;
        }
            //Caminhamento em profundidade
        case 9: {
            // output_file.close();
            int id;
            cout << "Digite o vértice de inicio: ";
            cin >> id;
            graph->depthFirstSearch(output_file, id);
        }
        case 10: {
            graph->agmGuloso(limite_dos_clusters);
        }
        case 11: {
            graph->agmGulosoRandAdap(limite_dos_clusters);
        }
        case 12: {
            graph->agmGulosoRandReativ(limite_dos_clusters);
        }
        default: {
            cout << "Exit!!!" << endl;
        }

    }
}

int mainMenu(ofstream &output_file,Graph *graph,vector<tuple<int, int>> limite_dos_clusters) {

    int selecao = 1;

    while (selecao != 0) {
        // system("clear");
        selecao = menu();

        if (output_file.is_open())
            selecionar(output_file,selecao, graph, limite_dos_clusters);

        else
            cout << "Unable to open the output_file" << endl;


        output_file << endl;

    }

    return 0;
}


int main(int argc, char const *argv[]) {

    int fileType;
    srand(time(nullptr));
    Graph *grap = new Graph(argc, argv);  //como auto ele nao reconhece
    vector<tuple<int, int>> limite_dos_clusters = grap->define_leitura();
    ofstream outputfile("files/outputFile5.dot", ios:: out);

    mainMenu(outputfile, grap, limite_dos_clusters);

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

    // mainMenu(output_file, graph);

    //Fechando arquivo de entrada
    input_file.close();

    //Fechando arquivo de saída
    output_file.close();

    return 0;
}

