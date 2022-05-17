#!/bin/bash

# Script to execute the code of graphs

g++ *.c* -o execGrupoX

#./execGrupoX <arquivo_entrada> <arquivo_saida> <Opc_Direc> <Opc_Peso_Aresta> <Opc_Peso_Nos>
 
./execGrupoX ./files/inputFile.txt ./files/outputFile.txt 0 0 0
