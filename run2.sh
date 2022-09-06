#!/bin/bash

# Script to execute the code of graphs

g++ *.c* -o execGrupoX

#./execGrupoX <arquivo_entrada> <arquivo_saida> <tipo_instancia>
# Tipo de Instância
# 0 - Arquivos RanReal e Sparse
# 1 - Handover

echo "RanReal240_01.txt"
./execGrupoX instancias/RanReal240_01.txt results/resultados.csv 0
echo "************************************************************"
echo "Instancia -> RanReal240_04.txt"
./execGrupoX instancias/RanReal240_04.txt results/resultados.csv 0
echo "************************************************************"
echo "Instancia -> RanReal240_07.txt"
./execGrupoX instancias/RanReal240_07.txt results/resultados.csv 0
echo "************************************************************"
echo "Instancia -> RanReal480_01.txt"
./execGrupoX instancias/RanReal480_01.txt results/resultados.csv 0
echo "************************************************************"
echo "Instancia -> RanReal480_04.txt"
./execGrupoX instancias/RanReal480_04.txt results/resultados.csv 0
echo "************************************************************"
echo "Instancia -> RanReal960_01.txt"
./execGrupoX instancias/RanReal960_01.30.txt results/resultados.csv 0
echo "************************************************************"
echo "Instancia -> Sparse82_02.txt"
./execGrupoX instancias/Sparse82_02.txt results/resultados.csv 0
echo "************************************************************"
echo "Instancia -> 20_5_270001"
