#ifndef STRUCTS_H
#define STRUCTS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#define Numv 30
#define INF INT_MAX


// Nodo de lista de adyacencia
typedef struct Nodo {
    int id;// indice del vértice destino
    int peso;// peso de la arista
    struct Nodo* siguiente;// puntero al siguiente nodo en la lista
} Nodo;//fin nodo

// Grafo
typedef struct {// estructura del grafo
    int numVertices;// número de vértices
    char etiquetas[Numv];// etiquetas de los vértices, que osn las etiquetas tipo 'a', 'b', 'c', etc.
    // cada vértice tiene una etiqueta asociada para facilitar su identificación
    Nodo* lista[Numv];// lista de adyacencia, que es un arreglo de punteros a nodos
} Grafo;//fin grafo

//funciones
Grafo* crearGrafo(int numVertices);// crea un grafo con un número dado de vértices
void agregarArista(Grafo* grafo, int origen, int destino, int peso, int dirigido);// agrega una arista al grafo
int buscarIndice(Grafo* grafo, char etiqueta);// busca el índice de un vértice dado su etiqueta
void dijkstra(Grafo* grafo, int inicio, int fin);// ejecuta el algoritmo de Dijkstra para encontrar el camino más corto
void imprimirCamino(int predecesor[], int fin, Grafo* grafo);// imprime el camino desde el vértice inicial hasta el final
Grafo* cargarGrafoDesdeArchivo(const char* filename, int dirigido);// carga un grafo desde un archivo

#endif
