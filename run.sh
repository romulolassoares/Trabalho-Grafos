#!/bin/bash

# Script to execute the code of graphs

g++ *.c* -o execGrupoX

#./execGrupoX <arquivo_entrada> <arquivo_saida> <tipo_instancia>
# Tipo de Instância
# 0 - Arquivos RanReal e Sparse
# 1 - Handover

echo "RanReal240_01.txt"
./execGrupoX instancias/RanReal240_01.txt results/resultados.csv 0
echo "RanReal240_04.txt"
./execGrupoX instancias/RanReal240_04.txt results/resultados.csv 0
echo "RanReal240_07.txt"
./execGrupoX instancias/RanReal240_07.txt results/resultados.csv 0
echo "RanReal480_01.txt"
./execGrupoX instancias/RanReal480_01.txt results/resultados.csv 0
echo "RanReal480_04.txt"
./execGrupoX instancias/RanReal480_04.txt results/resultados.csv 0
echo "RanReal960_01.30.txt"
./execGrupoX instancias/RanReal960_01.30.txt results/resultados.csv 0
echo "Sparse82_02.txt.txt"
./execGrupoX instancias/Sparse82_02.txt results/resultados.csv 0
echo "20_10_270003"
./execGrupoX instancias/20_10_270003 results/resultados.csv 1
echo "30_5_270003"
./execGrupoX instancias/30_5_270003 results/resultados.csv 1