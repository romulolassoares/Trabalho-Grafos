#ifndef TRABALHO_GRAFOS_MATRIZ_H
#define TRABALHO_GRAFOS_MATRIZ_H


class Matriz {
private:
    float *vet;
    int nl;
    int nc;
    bool simetrica;

    int detInd(int linha, int coluna);

public:
    Matriz(int linhas, int colunas, int simetrica);

    ~Matriz();

    float get(int linha, int coluna);

    void set(int linha, int coluna, float valor);
};


#endif //TRABALHO_GRAFOS_MATRIZ_H
