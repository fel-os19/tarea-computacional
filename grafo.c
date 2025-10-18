#include "structs.h"

Grafo* crearGrafo(int numVertices) {
    Grafo* g = (Grafo*)malloc(sizeof(Grafo));
    g->numVertices = numVertices;
    for (int i = 0; i < numVertices; i++) {
        g->lista[i] = NULL;
        g->etiquetas[i] = 'a' + i;
    }
    return g;
}

void agregarArista(Grafo* grafo, int origen, int destino, int peso, int dirigido) {
    Nodo* nuevo = (Nodo*)malloc(sizeof(Nodo));
    nuevo->id = destino;
    nuevo->peso = peso;
    nuevo->siguiente = grafo->lista[origen];
    grafo->lista[origen] = nuevo;

    // Si no es dirigido, agregar la arista inversa
    if (!dirigido) {
        Nodo* rev = (Nodo*)malloc(sizeof(Nodo));
        rev->id = origen;
        rev->peso = peso;
        rev->siguiente = grafo->lista[destino];
        grafo->lista[destino] = rev;
    }
}

int buscarIndice(Grafo* grafo, char etiqueta) {
    for (int i = 0; i < grafo->numVertices; i++) {
        if (grafo->etiquetas[i] == etiqueta)
            return i;
    }
    return -1;
}

// Cargar grafo desde archivo (formato: "a b" por línea)
Grafo* cargarGrafoDesdeArchivo(const char* filename, int dirigido) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        printf("Error al abrir archivo: %s\n", filename);
        exit(1);
    }

    // Determinar cantidad de vértices según nombre del archivo
    int n = 0;
    if (strstr(filename, "arbol")) n = 12;
    else if (strstr(filename, "planar")) n = 9;
    else if (strstr(filename, "euleriano")) n = 10;

    Grafo* g = crearGrafo(n);

    char u, v;
    while (fscanf(f, " %c %c", &u, &v) == 2) {
        int origen = buscarIndice(g, u);
        int destino = buscarIndice(g, v);
        if (origen == -1 || destino == -1) continue;
        agregarArista(g, origen, destino, 1, dirigido);
    }

    fclose(f);
    return g;
}
