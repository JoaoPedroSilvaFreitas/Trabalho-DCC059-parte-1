# Trabalho-DCC059-parte-1
Primeira parte do trabalho de teoria dos grafos 2021.3


-Parametros para iniciar o programa:
Nome_do_programa    Nome_do_arquivo_de_entrada.txt  Nome_do_arquivo_de_saida.txt (direcionado)   (aresta)    (nó)

comando para compilar(windows): cd "path do programa" ; if ($?) { g++ main.cpp Graph.cpp Node.cpp Edge.cpp -o main } ; if ($?) { .\main Nome_do_arquivo_de_entrada.txt Nome_do_arquivo_de_saida.txt (0 ou 1) (0 ou 1) (0 ou 1)}


-Formato arquivo de entrada:

    -Caso Grafo não tenha peso nos nós ou arestas:
        ordem do grafo
        Id(i)   Id(j)

    -Caso Grafo tenha peso nas arestas:
        ordem do grafo
        Id(i)   Id(j)   Peso(ij)

    -Caso Grafo tenha peso nos nós:
        ordem do grafo
        Id(i)   Peso(i)   Id(j)   Peso(j)

    -Caso Grafo tenha peso nos nós e nas arestas
        ordem do grafo
        Id(i)   Peso(i)   Id(j)   Peso(j)   Peso(ij)
