#include "graph.h"

//code for Dijkstra
// void add_vertex(Graph *g, int i){
// 	Vertex* vertex = (Vertex*)malloc(sizeof(Vertex));
// 	g->vertices[i] = vertex;
// 	g->vertices[i]->index = i;
// }


// void add_edge(Graph *g, int v_index, int weight, int end, int i){
// 	Vertex *vertex = g->vertices[v_index];
// 	printf("%d", v_index);
// 	int cap = 2;
// 	Edge **edges = malloc(sizeof(Edge*)*cap);

// 	if (log2(i-1) == floor(log2(i-1))){
// 		cap = 2 * cap;
// 		Edge **new_edges = malloc(sizeof(Edge*)*cap);
// 		for (int a = 0; a < cap; a++){
// 			new_edges[a] = edges[a];
// 		}
// 		vertex->edges = new_edges;
// 		vertex->edges[i]->end = end;
// 		vertex->edges[i]->weight = weight;
// 	}
// 	else{
// 	vertex->edges[i]->end = end;
// 	vertex->edges[i]->weight = weight;
// 	}
// }

void add_new_edge(Graph * g, int start, int end, int weight, int i){
	(g -> edges)[i].end = end;
	(g -> edges)[i].start = start;
	(g -> edges)[i].weight = weight;
}

int name_to_index(char * name){
	int index = 0;
	index = (name[0]-'A')*26*26 + (name[1]-'A')*26 + (name[2]-'A');
    return index;
}

char * index_to_name(int index){
	char *name = malloc(sizeof(char)*3);
	name[0] = index / (26*26) + 'A';
	name[1] = (index - (name[0]-'A') * 26 * 26)/26 + 'A';
	name[2] = index - (name[0]-'A') *26 * 26- (name[1]-'A') * 26 + 'A';
	return name;
}
int find_index(Graph *g, int index, int total){
	for (int i = 0; i < total; i++){
		if (g->vertices[i] == index){
			return i;
		}
	}
	fprintf(stderr, "Index not found");
	return total;
}

Graph * generate_graph(char* start, char* end){
	FILE *pfile = fopen("large-airports.txt", "r");
	int total = 0;
	char line[40];
	while (fgets(line, 40, pfile) != NULL){
		total ++;
	}
	rewind(pfile);

	char name[4];
	char **airports;
	airports = malloc(total*sizeof(char*));
	for (int i = 0; i < total; i++){
		airports[i] = malloc(sizeof(char)*4);
	}
	int indices[26*26*26];
	//int * vertices = malloc(sizeof(int)*total);
	Graph *g = malloc(sizeof(Graph));
	g->vertices = malloc(sizeof(int)*total);
	int index = 0;
	//g->vertices = malloc(100*sizeof(Vertex*));
	for (int i = 0; i < 26*26*26; i++){
		indices[i] = 0;
	}
	for (int i = 0; i < total; i++){
		fscanf(pfile, "%s\t%*[^\n]\n", name);
		strcpy(airports[i],name); 
		index = name_to_index(name);
		indices[index] = 1;
		g->vertices[i] = index;
		if (!strcmp(airports[i], start)){
			g->st = i;
		}
		if (!strcmp(airports[i], end)){
			g->e = i;
		}
		//add_vertex(g, i);
	}

	FILE *fp = fopen("large-dists.txt", "r");

	char sta[40];
	char en[40];
	int distance = 0;
	int st = 0, e = 0, num = 0;
	while (fgets(line, 40, fp) != NULL){
		num ++;
	}

	rewind(fp);

	g->edges = malloc(sizeof(Edge)*num);

	for (int i = 0; i < num; i++){
		fscanf(fp, "%s\t%s\t%d\n", sta, en, &distance);
		st = name_to_index(sta);
		e = name_to_index(en);
		add_new_edge(g, st, e, distance, i);
		//printf("%s %s %d \n", index_to_name(st), index_to_name(e), distance);
	}

	return g;
}

void print_path(Graph *g, int *path, int st, int e) {
	int cur = e;
	while (cur != st) {
		printf("%s\t%s\n", index_to_name(g->vertices[cur]), index_to_name(g->vertices[path[cur]]));
		cur = path[cur];
	}
}

int bellman(char* st, char* e){
	FILE *pfile = fopen("large-airports.txt", "r");
	int total = 0;
	char line[40];
	while (fgets(line, 40, pfile) != NULL){
		total ++;
	}

	FILE *fp = fopen("large-dists.txt", "r");
	int num = 0;
	while (fgets(line, 40, fp) != NULL){
		num ++;
	}

	Graph *g;
	g = generate_graph(st, e);
	int change = 1;
	int * scores = malloc(sizeof(int)*total);
	for (int i = 0; i < total; i++){
		scores[i] = INT_MAX;
	}


	int *path = malloc(sizeof(int)*total);
	path[g->st] = g->st;

	// printf("%s %d\n",st, g->st);
	// printf("%s %d\n",e, g->e);

	scores[g->st] = 0;
	while (change != 0){
		change = 0;
		for (int i = 0; i < num; i++){
			int end_index = find_index(g,(g->edges)[i].end,total);
			int start_index = find_index(g,(g->edges)[i].start,total);
			// printf("%d\n", start_index);
			// printf("%d\n", end_index);
			// printf("%d\n", scores[end_index]);
			// printf("%d\n", (g->edges)[i].weight);
				if (scores[start_index] != INT_MAX) {
					if (scores[end_index] != INT_MAX) {
						if (scores[end_index] + (g->edges)[i].weight < scores[start_index]) {
							scores[start_index] = scores[end_index] + (g->edges)[i].weight;
							path[start_index] = end_index;
							change++;
						}
						if (scores[start_index] + (g->edges)[i].weight < scores[end_index]) {
							scores[end_index] = scores[start_index] + (g->edges)[i].weight;
							path[end_index] = start_index;
							change++;
						}
					}
					else {
						scores[end_index] = scores[start_index] + (g->edges)[i].weight;
						path[end_index] = start_index;
						change++;
					}
				}
				else {
					if (scores[end_index] != INT_MAX) {
						scores[start_index] = scores[end_index] + (g->edges)[i].weight;
						path[start_index] = end_index;
						change++;
					}
					else {
						continue;
					}
				}
		}
	}
	// for (int i=0; i<total; i++) {
	// 	printf("%d ", path[i]);
	// }
	print_path(g, path, g->st, g->e);
	return scores[g->e];
}

// void print_vertex(Graph *g){
// 	for(int i = 0; i < 100; i++) {
// 		printf("Vertex %d:\n", i);
// 		for(int j = 0; j < 50; j++) {
// 			printf("%d, %d\n", g->vertices[i]->edge[j]->end, g->vertices[i]->edge[j]->weight);
// 		}
// 	}
// }


void print_array(int size, char arr[size][40]){
	for (int i=0; i<size; i++){
		printf("%s ",arr[i]);	
	}
	printf("\n");
}

int name_check(char *start, char *end){
	if (strlen(start) != 3 || strlen(end) != 3){
		return 0;
	} for(int i = 0; i < 3; i++){
        if(!isalpha(start[i]) || !isalpha(end[i])) {
        	return 0;
        }
    } return 1;
}

void airports(){
	FILE *pfile = fopen("large-airports.txt", "r");
	if(pfile == NULL)  {
		printf("Error opening file");
	}
	int total = 0;
	char line[40];
	while (fgets(line, 40, pfile) != NULL){
		printf("%s",line);
		total ++;
	}
	rewind(pfile);

	char token[40];
	char airport[total][40];

	for (int i = 0; i < total; i++){
		fscanf(pfile, "%s\t%*[^\n]\n", token);
		strcpy(airport[i],token); 
	}
	//print_array(total, airport);

	fclose(pfile);
}

// void distance(Graph *g, vertex v1, vertex v2){
// 	FILE *fp = fopen("small-dists.txt", "r");

// }

int main() {
	printf("enter command>\n");
	//char buf[10];
	//scanf("%s", mode);
	char *start, *end, *mode;
	char text[20]; 
	while(fgets(text, 20, stdin)) {
        mode = strtok(text, " \n");
        start = strtok(NULL," \n");
        end = start != NULL ? strtok(NULL," \n") : NULL;

		if (!strcmp(mode, "help")){
			printf("help: prints out a help message on how to use your system.\n" 
		"quit: quits the program.\n" 
		"airports: prints all airports serviced by FTA.\n" 
		"distance <airport1> <airport2>: prints the shortest path from one airport to another, \n"
		"using only routes flown by the airline. Also prints the total length of the shortest path.\n");
		}

		else if (!strcmp(mode, "quit")){
			break;
		}
		
		else if (!strcmp(mode, "distance")){
			if (start && end && name_check(start, end)){
			int a = bellman(start, end);
			printf("%d\n", a);
			}
		}
		else if (!strcmp(mode, "airports")){
			airports();
		}
		printf("enter command>\n");
	}

	return 0;
}