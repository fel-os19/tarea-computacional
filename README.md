# *Proyecto: Dijkstra en C*
Este programa permite cargar un grafo no dirigido desde un archivo, aplicar una orientación dirigida según distintos criterios, y calcular el camino más corto entre dos vértices usando el algoritmo de Dijkstra.
## Compilación 
gcc camino.c -o camino 
## Uso
 ./camino <nombre_de_archivo> <tipo_de_orientación> <vértice_origen> <vértice_destino>

## Formato del archivo de entrada
Si se quiere usar otros grafos distintos a los que ya tenemos, el archivo debe tener dos líneas:
- *Lista de vértices*:
Ejemplo: V = {a, b, c, d, e}
- *Lista de aristas no dirigidas (pares de letras)*:
Ejemplo: E = {ab, ac, bd, ce}


## Salida esperada
- Camino más corto desde el vértice origen al destino (si existe), junto con su distancia.


