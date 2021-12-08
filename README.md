# Trabalho-DCC059-parte-1
Primeira parte do trabalho de teoria dos grafos 2021.3


-Parametros para iniciar o programa:
Nome_do_programa    Nome_do_arquivo_de_entrada.dot  Nome_do_arquivo_de_saida.dot (direcionado)   (aresta)    (nó)

comando para compilar: cd "path do programa" ; if ($?) { g++ main.cpp Graph.cpp Node.cpp Edge.cpp -o main } ; if ($?) { .\main Grafo.dot GrafoO.dot (0 ou 1) (0 ou 1) (0 ou 1)}


-Formato arquivo Grafo.dot:

    -Caso Grafo não tenha peso nos nós ou arestas:
        order
        Id(i)   Id(j)

    -Caso Grafo tenha peso nas arestas:
        order
        Id(i)   Id(j)   Peso(ij)

    -Caso Grafo tenha peso nos nós:
        order
        Id(i)   Peso(i)   Id(j)   Peso(j)

    -Caso Grafo tenha peso nos nós e nas arestas
        order
        Id(i)   Peso(i)   Id(j)   Peso(j)   Peso(ij)
