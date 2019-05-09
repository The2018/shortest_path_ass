#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <ctype.h>

typedef struct{
	int start;
	int end;
	int weight;
}Edge;

// typedef struct{
// 	Edge** edges;
// 	int index;
// }Vertex;

// typedef struct{
// 	Vertex** vertices;
// }Graph;

typedef struct{
	Edge * edges;
	int * vertices;
	int st;
	int e;
}Graph;

// void add_vertex(Graph *g, int i);
// void add_edge(Graph *g, int v_index, int weight, int end, int i);
Graph * generate_graph();