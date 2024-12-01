#!/bin/bash

# Diretório onde os arquivos .cpp estão localizados
DIR="./tests"

# Loop por todos os arquivos .cpp no diretório
for file in $DIR/*.cpp; do
    # Verifica se o arquivo existe
    if [ -f "$file" ]; then
        # Extrai o nome base do arquivo (sem a extensão .cpp)
        output="${file%.cpp}"

        # Compila o arquivo .cpp
        # echo "Compilando $file..."
        g++ "$file" ./tests/Tester.h -o "$output"

        # Verifica se a compilação foi bem-sucedida
        if [ $? -eq 0 ]; then
            # Executa o binário gerado
            # echo "Rodando $output..."
            ./$output
            rm "$output"
        else
            echo "Erro na compilação de $file"
        fi
    fi
done
