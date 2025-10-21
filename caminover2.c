#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define NVMAX 30
#define INFINITO 99999
#define DELIMIT ", }"

// estructura para simplificar la logica del vertice y guardar su estado
typedef struct {
    char nombre;
    int distancia;
    bool visitado;
    int IndicePadre;
} Vertice;

// funcion que inicializa un vertice
void inicializarVertice(Vertice *vertice, char nombre) {
    vertice->nombre = nombre;
    vertice->distancia = INFINITO;
    vertice->visitado = false;
    vertice->IndicePadre = -1;
}
// funcion para asignar memoria dinamica a una matriz de adyacencia
int **asignarMatrizDinamica(int vNum) {
    int **matriz = malloc(vNum * sizeof(int *));
    if (!matriz) {
        perror("malloc");
        return NULL;
    }
    for (int i = 0; i < vNum; i++) {
        matriz[i] = calloc(vNum, sizeof(int));
        if (!matriz[i]) {
            perror("calloc");
            for (int j = 0; j < i; j++)
                free(matriz[j]);
            free(matriz);
            return NULL;
        }
    }
    return matriz;
}

// funcion utilitaria para liberar memoria
void liberarMatriz(int **matriz, int vNum) {
    for (int i = 0; i < vNum; i++)
        free(matriz[i]);
    free(matriz);
}

// funcion puramente utilitaria para debug
void imprimirMatriz(int **matriz, const Vertice *vertices, int vNum) {
    printf("   ");
    for (int i = 0; i < vNum; i++)
        printf("%c ", vertices[i].nombre);
    printf("\n");
    for (int i = 0; i < vNum; i++) {
        printf("%c: ", vertices[i].nombre);
        for (int j = 0; j < vNum; j++)
            printf("%d ", matriz[i][j]);
        printf("\n");
    }
}
// implementacion del algoritmo de dijkstra 
void dijkstra(int vNum, int **matriz, Vertice vertices[], int origen, int destino) {
    vertices[origen].distancia = 0;
    for (int k = 0; k < vNum - 1; k++) {
        int distancia_min = INFINITO;
        int u = -1;
        for (int i = 0; i < vNum; i++) {
            if (!vertices[i].visitado && vertices[i].distancia < distancia_min) {
                distancia_min = vertices[i].distancia;
                u = i;
            }
        }

        if (u == -1)
            break;
        vertices[u].visitado = true;

        for (int v = 0; v < vNum; v++) {
            if (matriz[u][v] == 1 && !vertices[v].visitado) {
                int nuevaDist = vertices[u].distancia + 1;
                if (nuevaDist < vertices[v].distancia) {
                    vertices[v].distancia = nuevaDist;
                    vertices[v].IndicePadre = u;
                }
            }
        }
    }

    if (vertices[destino].distancia == INFINITO) {
        printf("No hay camino desde %c a %c\n", vertices[origen].nombre, vertices[destino].nombre);
        return;
    }

    printf("Camino mas corto de %c a %c: ", vertices[origen].nombre, vertices[destino].nombre);
    int camino[NVMAX];
    int tam = 0;
    for (int v = destino; v != -1; v = vertices[v].IndicePadre)
        camino[tam++] = v;

    for (int i = tam - 1; i >= 0; i--)
        printf("%c ", vertices[camino[i]].nombre);
    printf("\nDistancia: %d\n", vertices[destino].distancia);
}

int buscarIndice(const Vertice vertices[], int vNum, char nombre) {
    for (int i = 0; i < vNum; i++)
        if (vertices[i].nombre == nombre)
            return i;
    return -1;
}

// funcion para aplicar la orientacion segun el tipo
void aplicarOrientacion(int vNum, int **matriz, int tipo) {
    for (int i = 0; i < vNum; i++) {
        for (int j = i + 1; j < vNum; j++) {
            if (matriz[i][j] == 1 || matriz[j][i] == 1) {
                switch (tipo) {
                case 1:
                    matriz[i][j] = 1;
                    matriz[j][i] = 0;
                    break;
                case 2:
                    matriz[i][j] = 0;
                    matriz[j][i] = 1;
                    break;
                case 3:
                    matriz[i][j] = 1;
                    matriz[j][i] = 1;
                    break;
                case 4:
                    if (rand() % 2) {
                        matriz[i][j] = 1;
                        matriz[j][i] = 0;
                    } else {
                        matriz[i][j] = 0;
                        matriz[j][i] = 1;
                    }
                    break;
                }
            }
        }
    }
}

// funcion para leer el archivo y cargar vertices y matriz
int leerArchivo(int *vNum, int *vCapacidad, const char *nombretxt, Vertice **vertices, int ***matriz) {
    FILE *archivo = fopen(nombretxt, "r");
    if (!archivo) {
        perror("fopen");
        return -1;
    }

    char linea[256];
    if (fgets(linea, sizeof(linea), archivo) != NULL) {
        char *p = strchr(linea, '{');
        if (p) {
            p++;
            char *token = strtok(p, DELIMIT);
            while (token != NULL) {
                if (strlen(token) > 0 && isalpha(token[0])) {
                    if (*vNum >= *vCapacidad) {
                        (*vCapacidad) *= 2;
                        Vertice *temp = realloc(*vertices, *vCapacidad * sizeof(Vertice));
                        if (!temp) {
                            perror("realloc");
                            fclose(archivo);
                            return -1;
                        }
                        *vertices = temp;
                    }
                    inicializarVertice(&(*vertices)[*vNum], token[0]);
                    (*vNum)++;
                }
                token = strtok(NULL, DELIMIT);
            }
        }
    }

    *matriz = asignarMatrizDinamica(*vNum);
    if (!*matriz) {
        fclose(archivo);
        return -1;
    }

    if (fgets(linea, sizeof(linea), archivo) != NULL) {
        char *p = strchr(linea, '{');
        if (p) {
            p++;
            char *token = strtok(p, DELIMIT);
            while (token) {
                if (strlen(token) >= 2) {
                    char origen = token[0];
                    char destino = token[1];
                    int i = buscarIndice(*vertices, *vNum, origen);
                    int j = buscarIndice(*vertices, *vNum, destino);
                    if (i != -1 && j != -1) {
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
// funcion para validar la entrada del programa
int validarEntrada(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Uso: %s <archivo.txt> <tipo_orientacion (1-4)> <origen> <destino>\n", argv[0]);
        return -1;
    }
    int tipo = atoi(argv[2]);
    if (tipo < 1 || tipo > 4) {
        fprintf(stderr, "Tipo de orientacion invalido (1-4)\n");
        return -1;
    }
    if (strlen(argv[3]) != 1 || !isalpha(argv[3][0]) || strlen(argv[4]) != 1 || !isalpha(argv[4][0])) {
        fprintf(stderr, "Vertices deben ser letras\n");
        return -1;
    }
    return 1;
}
// programa principal
int main(int argc, char *argv[]) {
    if (!validarEntrada(argc, argv))
        return -1;

    int vNum = 0;
    int vCapacidad = NVMAX;
    Vertice *vertices = malloc(vCapacidad * sizeof(Vertice));
    if (!vertices) {
        perror("malloc");
        return -1;
    }

    int **matriz = NULL;
    if (leerArchivo(&vNum, &vCapacidad, argv[1], &vertices, &matriz) != 1) {
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
    if (origen == -1 || destino == -1) {
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
