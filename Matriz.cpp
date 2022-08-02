#include "Matriz.h"

#include <iostream>

using namespace std;

Matriz::Matriz(int linhas, int colunas, int simetrica) {
    this->nl = linhas;
    this->nc = colunas;
    this->simetrica = simetrica;
    if (simetrica)
        this->vet = new float[nl * (nl + 1) / 2];
    else
        this->vet = new float[nl * nc];
}

Matriz::~Matriz() {
    delete[]vet;
}

int Matriz::detInd(int linha, int coluna) {
    if (linha >= nl || linha < 0 || coluna >= nc || coluna < 0) {
        cout << "Indice invalido" << endl;
        return -1;
    }
    if (this->simetrica) {
        if (linha < coluna) {
            return (((coluna + 1) * coluna) / 2) + linha;
        } else {
            return (((linha + 1) * linha) / 2) + coluna;
        }
    } else {
        return (linha * nl) + coluna;
    }
}

float Matriz::get(int linha, int coluna) {
    int k = detInd(linha, coluna);
    if (k != -1) {
        return this->vet[k];
    } else {
        exit(-1);
    }
}

void Matriz::set(int linha, int coluna, float valor) {
    int k = detInd(linha, coluna);
    if (k != -1) {
        this->vet[k] = valor;
    } else {
        exit(-1);
    }
}