#include "structs.h"

int main() {
    char tipo[50];
    char inicioChar, finChar;
    int dirigido = 0;

    printf("Ingrese tipo de grafo (arbol / planar / euleriano): ");
    scanf("%s", tipo);

    printf("Desea orientacion dirigida (1 = si, 0 = no): ");
    scanf("%d", &dirigido);

    char filename[100];
    sprintf(filename, "grafo_%s.txt", tipo);

    Grafo* g = cargarGrafoDesdeArchivo(filename, dirigido);

    printf("Ingrese vértice inicial y final (ej: a h): ");
    scanf(" %c %c", &inicioChar, &finChar);

    int inicio = buscarIndice(g, inicioChar);
    int fin = buscarIndice(g, finChar);

    if (inicio == -1 || fin == -1) {
        printf("Error: vértices inválidos.\n");
        return 1;
    }

    dijkstra(g, inicio, fin);
    return 0;
}
