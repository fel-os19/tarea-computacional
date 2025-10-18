#include "structs.h"

void dijkstra(Grafo* grafo, int inicio, int fin) {
    int n = grafo->numVertices;
    int distancia[n];
    int visitado[n];
    int predecesor[n];

    for (int i = 0; i < n; i++) {
        distancia[i] = INF;
        visitado[i] = 0;
        predecesor[i] = -1;
    }

    distancia[inicio] = 0;

    for (int c = 0; c < n - 1; c++) {
        int min = INF, u = -1;
        for (int i = 0; i < n; i++)
            if (!visitado[i] && distancia[i] < min)
                min = distancia[i], u = i;

        if (u == -1) break;
        visitado[u] = 1;

        Nodo* temp = grafo->lista[u];
        while (temp) {
            int v = temp->id;
            int peso = temp->peso;
            if (!visitado[v] && distancia[u] + peso < distancia[v]) {
                distancia[v] = distancia[u] + peso;
                predecesor[v] = u;
            }
            temp = temp->siguiente;
        }
    }

    if (distancia[fin] == INF) {
        printf("No existe camino entre %c y %c\n", grafo->etiquetas[inicio], grafo->etiquetas[fin]);
    } else {
        printf("Distancia mínima entre %c y %c: %d\n", grafo->etiquetas[inicio], grafo->etiquetas[fin], distancia[fin]);
        printf("Camino: ");
        imprimirCamino(predecesor, fin, grafo);
        printf("\n");
    }
}

void imprimirCamino(int predecesor[], int fin, Grafo* grafo) {
    if (predecesor[fin] == -1) {
        printf("%c", grafo->etiquetas[fin]);
        return;
    }
    imprimirCamino(predecesor, predecesor[fin], grafo);
    printf(" -> %c", grafo->etiquetas[fin]);
}
