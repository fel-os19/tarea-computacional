#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#define NVMAX 30
#define INFINITO 99999
#define DELIMIT ", }"

// struct para simplificar la lógica del vertice y guardar su estado
typedef struct
{
    char nombre;
    int distancia;
    bool visitado;
    int IndicePadre;
} Vertice;

// esta función tiene lugar en la lectura del archivo
void inicializarVertice(Vertice *vertice, char nombre)
{
    vertice->nombre = nombre;
    vertice->distancia = INFINITO;
    vertice->visitado = false;
    vertice->IndicePadre = -1;
}
// función para asignar memoria dinámica a una matriz de adyacencia
int **asignarMatrizDinamica(int vNum)
{
    int **matriz = malloc(vNum * sizeof(int *));
    if (!matriz)
    {
        perror("malloc");
        return NULL;
    }
    for (int i = 0; i < vNum; i++)
    {
        matriz[i] = calloc(vNum, sizeof(int));
        if (!matriz[i])
        {
            perror("calloc");
            for (int j = 0; j < i; j++)
                free(matriz[j]);
            free(matriz);
            return NULL;
        }
    }
    return matriz;
}
// función utilitaria para no repetir código
void liberarMatriz(int **matriz, int vNum)
{
    for (int i = 0; i < vNum; i++)
        free(matriz[i]);
    free(matriz);
}
// función puramente utilitaría para debug, pero no tiene que ver con el objetivo principal
void imprimirMatriz( int **matriz, const Vertice *vertices, int vNum)
{
    printf("   ");
    for (int i = 0; i < vNum; i++)
        printf("%c ", vertices[i].nombre);
    printf("\n");
    for (int i = 0; i < vNum; i++)
    {
        printf("%c: ", vertices[i].nombre);
        for (int j = 0; j < vNum; j++)
            printf("%d ", matriz[i][j]);
        printf("\n");
    }
}
// algoritmo de dijkstra
void dijkstra(int vNum, int **matriz, Vertice vertices[], int origen, int destino)
{
    vertices[origen].distancia = 0;
    for (int k = 0; k < vNum - 1; k++)
    { // recorre todos los vértices
        int distancia_min = INFINITO;
        int u = -1;
        for (int i = 0; i < vNum; i++)
        {
            if (!vertices[i].visitado && vertices[i].distancia < distancia_min)
            {
                distancia_min = vertices[i].distancia;
                u = i; // u guardará el indice del vertice más cercano
            }
        }

        if (u == -1)
            break; // no hay vertice que lo conecte que no esté visitada
        vertices[u].visitado = true;

        for (int v = 0; v < vNum; v++)
        {
            // vecinos de u, están en las columnas de la fila de u en la matriz de adyacencia
            // pero solo revisamos sus salidas, no entradas, dado que queremos construir un camino
            if (matriz[u][v] == 1 && !vertices[v].visitado)
            {
                int nuevaDist = vertices[u].distancia + 1; // como el peso es 1 pues se lo sumamos y es más simple que implementar pesos
                if (nuevaDist < vertices[v].distancia)
                {
                    vertices[v].distancia = nuevaDist;
                    vertices[v].IndicePadre = u;
                }
            }
        }
    }

    if (vertices[destino].distancia == INFINITO)
    { // si el anterior for termina por break, tendríamos que no hay camino
        printf("No hay camino desde %c a %c\n", vertices[origen].nombre, vertices[destino].nombre);
        return;
    }
    // construcción del camino, se puede separar de la lógica de dijkstra si se quiere
    printf("Camino mas corto de %c a %c: ", vertices[origen].nombre, vertices[destino].nombre);
    int camino[NVMAX]; // le pasamos esa constante para que compile aunque luego será cambiado
    int tam = 0;       // el tamaño del arreglo irá creciendo en tiempo de ejecución
    for (int v = destino; v != -1; v = vertices[v].IndicePadre)
        camino[tam++] = v; // primero se asigna y luego hace el aumento

    for (int i = tam - 1; i >= 0; i--) // dado que empezamos a guardar desde el último se recorre al revéz
        printf("%c ", vertices[camino[i]].nombre);
    printf("\nDistancia: %d\n", vertices[destino].distancia);
}

// básicamente solo busca en qué posición del arreglo está, pero
// se puede usar ascii en su lugar, si asumimos una lista ordenada de vértices
int buscarIndice(Vertice vertices[], int vNum, char nombre)
{
    for (int i = 0; i < vNum; i++)
        if (vertices[i].nombre == nombre)
            return i;
    return -1;
}
// cuatro orientaciones posibles
void aplicarOrientacion(int vNum, int **matriz, int tipo)
{
    for (int i = 0; i < vNum; i++)
    {
        for (int j = i + 1; j < vNum; j++)
        {
            // j = i+1 para recorrer solo la triangular superior,
            // dado que recibe la matriz del grafo no dirigido y no es necesario recorrer la inferior
            // y no recorre la diagonal principal por la misma razón
            if (matriz[i][j] == 1 || matriz[j][i] == 1)
            {
                switch (tipo)
                {
                case 1: // a - b = a -> b
                    matriz[i][j] = 1;
                    matriz[j][i] = 0;
                    break;
                case 2: // a - b = a <- b
                    matriz[i][j] = 0;
                    matriz[j][i] = 1;
                    break;
                case 3: // a - b = a <-> b, osea tecnicamente se mantiene la no dirigida
                    matriz[i][j] = 1;
                    matriz[j][i] = 1;
                    break;
                case 4:
                    if (rand() % 2)
                    { // orientación aleatoria
                        matriz[i][j] = 1;
                        matriz[j][i] = 0;
                    }
                    else
                    {
                        matriz[i][j] = 0;
                        matriz[j][i] = 1;
                    }
                    break;
                }
            }
        }
    }
}
// función para leer el archivo y cargar vértices y matriz
int leerArchivo(int *vNum, int *vCapacidad, char *nombretxt, Vertice **vertices, int ***matriz)
{
    FILE *archivo = fopen(nombretxt, "r");
    if (!archivo)
    {
        perror("fopen");
        return -1;
    }

    char linea[256];
    if (fgets(linea, sizeof(linea), archivo) != NULL)
    {                                 // lee la primera línea
        char *p = strchr(linea, '{'); // primera aparición de "{"
        if (p)
        {
            p++; // después de "{", sumamos uno al puntero para apuntar al siguiente caracter
            char *token = strtok(p, DELIMIT);
            while (token != NULL)
            {
                if (strlen(token) > 0 && isalpha(token[0]))
                {
                    if (*vNum >= *vCapacidad)
                    { // esto es por si el tamaño que tenemos no es suficiente
                        // ajustamos dinámicamente reasignando memoria
                        (*vCapacidad) *= 2;
                        Vertice *temp = realloc(*vertices, *vCapacidad * sizeof(Vertice));
                        if (!temp)
                        {
                            perror("realloc");
                            fclose(archivo);
                            return -1;
                        }
                        *vertices = temp;
                    }
                    // técnicamente esto es de la lógica de dijkstra
                    //  pero dado que estamos inicializando el nombre del vértice aprovechamos a hacer lo demás
                    inicializarVertice(&(*vertices)[*vNum], token[0]);
                    (*vNum)++;
                }
                token = strtok(NULL, DELIMIT);
            }
        }
    }
    // esto básicamente al puntero nulo de matriz en el main le asignamos memoria
    // se hizo por simplicidad de manejar punteros por referencia en lugar de matriz estática
    *matriz = asignarMatrizDinamica(*vNum);
    if (!*matriz)
    {
        fclose(archivo);
        return -1;
    }

    if (fgets(linea, sizeof(linea), archivo) != NULL)
    { // lee la segunda línea
        char *p = strchr(linea, '{');
        if (p)
        {
            p++;
            char *token = strtok(p, DELIMIT);
            while (token)
            {
                if (strlen(token) >= 2)
                {
                    char origen = token[0];
                    char destino = token[1];
                    int i = buscarIndice(*vertices, *vNum, origen);
                    int j = buscarIndice(*vertices, *vNum, destino);
                    if (i != -1 && j != -1)
                    {
                        (*matriz)[i][j] = 1;
                        (*matriz)[j][i] = 1;
                    }
                }
                token = strtok(NULL, DELIMIT);
            }
        }
    }

    fclose(archivo);
    return 1;
}
// función para validar la entrada del programa
int validarEntrada(int argc, char *argv[])
{
    if (argc != 5)
    {
        fprintf(stderr, "Uso: %s <archivo.txt> <tipo_orientacion (1-4)> <origen> <destino>\n", argv[0]);
        return -1;
    }
    int tipo = atoi(argv[2]);
    if (tipo < 1 || tipo > 4)
    {
        fprintf(stderr, "Tipo de orientacion invalido (1-4)\n");
        return -1;
    }
    if (strlen(argv[3]) != 1 || !isalpha(argv[3][0]) || strlen(argv[4]) != 1 || !isalpha(argv[4][0]))
    {
        fprintf(stderr, "Vertices deben ser letras\n");
        return -1;
    }
    return 1;
}

int main(int argc, char *argv[])
{
    if (!validarEntrada(argc, argv))
        return -1;

    int vNum = 0;
    int vCapacidad = NVMAX;
    Vertice *vertices = malloc(vCapacidad * sizeof(Vertice));
    if (!vertices)
    {
        perror("malloc");
        return -1;
    }

    int **matriz = NULL;
    // dentro de la lectura del archivo se inicializará la matriz
    if (leerArchivo(&vNum, &vCapacidad, argv[1], &vertices, &matriz) != 1)
    {
        free(vertices);
        return -1;
    }

    printf("Vertices: %d\n ", vNum);
    for (int i = 0; i < vNum; i++)
        printf("%c ", vertices[i].nombre);
    printf("\n\nMatriz de adyacencia:\n");
    imprimirMatriz(matriz, vertices, vNum);

    aplicarOrientacion(vNum, matriz, atoi(argv[2]));
    printf("\nOrientacion aplicada:\n");
    imprimirMatriz(matriz, vertices, vNum);

    int origen = buscarIndice(vertices, vNum, argv[3][0]);
    int destino = buscarIndice(vertices, vNum, argv[4][0]);
    if (origen == -1 || destino == -1)
    {
        fprintf(stderr, "Vertices invalidos\n");
        liberarMatriz(matriz, vNum);
        free(vertices);
        return -1;
    }

    dijkstra(vNum, matriz, vertices, origen, destino);

    liberarMatriz(matriz, vNum);
    free(vertices);
    return 0;
}