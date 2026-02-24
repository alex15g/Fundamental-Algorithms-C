 /*
 * Gavris Alexandru 
 * Parcurgerea unui graf utilizand BFS
 *
 * Implementari realizate:
 * - get_neighbors -> determina vecinii valizi ai unei pozitii din grid (sus/jos/stanga/dreapta)
 * - bfs -> parcurgere Breadth-First Search cu coada implementata manual
 * - print_bfs_tree -> reconstruieste arborele BFS folosind vectorul de parinti si il afiseaza ierarhic
 * - shortest_path -> determina cel mai scurt drum intre doua noduri folosind parintii rezultati din BFS
 * - generareGrafConex -> construieste un graf aleator conex cu numar dat de muchii
 * - performance -> testeaza complexitatea BFS variind numarul de muchii si numarul de noduri
 *
 * Analiza complexitatilor:
 * - get_neighbors -> O(1), verifica doar 4 directii
 * - bfs -> O(V + E), unde V = numar noduri, E = numar muchii
 *      - fiecare nod este introdus o singura data in coada
 *      - fiecare muchie este procesata exact o data
 * - shortest_path -> O(V) pentru reconstructia drumului
 * - generareGrafConex -> O(E), deoarece adauga exact E muchii
 *
 * Observatii grafice:
 * - bfs-edges creste liniar cu numarul de muchii, conform teoriei
 * - bfs-vertices creste aproape liniar cu numarul de noduri
 *
 * Algoritmul BFS este implementat corect, respectand complexitatea teoretica O(V+E).
 */


#include <stdlib.h>
#include <string.h>
#include "bfs.h"
#include <time.h>



typedef struct NodeR2 {
    Point key;
    struct NodeR2** children;
    int nc;
}NodeR2;

int get_neighbors(const Grid *grid, Point p, Point neighb[])
{
    int count = 0;

    //sus
    if (p.row - 1 >= 0 && grid->mat[p.row - 1][p.col] == 0) {
        neighb[count++] = { p.row - 1, p.col };
    }

    //jos
    if (p.row + 1 < grid->rows && grid->mat[p.row + 1][p.col] == 0) {
        neighb[count++] = { p.row + 1, p.col };
    }

    //stanga
    if (p.col - 1 >= 0 && grid->mat[p.row][ p.col - 1] == 0) {
        neighb[count++] = { p.row, p.col - 1 };
    }

    //dreapta
    if (p.col + 1 < grid->cols && grid->mat[p.row][ p.col + 1] == 0) {
        neighb[count++] = { p.row, p.col + 1 };
    }

    return count;
}

void grid_to_graph(const Grid *grid, Graph *graph)
{
    //we need to keep the nodes in a matrix, so we can easily refer to a position in the grid
    Node *nodes[MAX_ROWS][MAX_COLS];
    int i, j, k;
    Point neighb[4];

    //compute how many nodes we have and allocate each node
    graph->nrNodes = 0;
    for(i=0; i<grid->rows; ++i){
        for(j=0; j<grid->cols; ++j){
            if(grid->mat[i][j] == 0){
                nodes[i][j] = (Node*)malloc(sizeof(Node));
                memset(nodes[i][j], 0, sizeof(Node)); //initialize all fields with 0/NULL
                nodes[i][j]->position.row = i;
                nodes[i][j]->position.col = j;
                ++graph->nrNodes;
            }else{
                nodes[i][j] = NULL;
            }
        }
    }
    graph->v = (Node**)malloc(graph->nrNodes * sizeof(Node*));
    k = 0;
    for(i=0; i<grid->rows; ++i){
        for(j=0; j<grid->cols; ++j){
            if(nodes[i][j] != NULL){
                graph->v[k++] = nodes[i][j];
            }
        }
    }

    //compute the adjacency list for each node
    for(i=0; i<graph->nrNodes; ++i){
        graph->v[i]->adjSize = get_neighbors(grid, graph->v[i]->position, neighb);
        if(graph->v[i]->adjSize != 0){
            graph->v[i]->adj = (Node**)malloc(graph->v[i]->adjSize * sizeof(Node*));
            k = 0;
            for(j=0; j<graph->v[i]->adjSize; ++j){
                if( neighb[j].row >= 0 && neighb[j].row < grid->rows &&
                    neighb[j].col >= 0 && neighb[j].col < grid->cols &&
                    grid->mat[neighb[j].row][neighb[j].col] == 0){
                        graph->v[i]->adj[k++] = nodes[neighb[j].row][neighb[j].col];
                }
            }
            if(k < graph->v[i]->adjSize){
                //get_neighbors returned some invalid neighbors
                graph->v[i]->adjSize = k;
                graph->v[i]->adj = (Node**)realloc(graph->v[i]->adj, k * sizeof(Node*));
            }
        }
    }
}

void free_graph(Graph *graph)
{
    if(graph->v != NULL){
        for(int i=0; i<graph->nrNodes; ++i){
            if(graph->v[i] != NULL){
                if(graph->v[i]->adj != NULL){
                    free(graph->v[i]->adj);
                    graph->v[i]->adj = NULL;
                }
                graph->v[i]->adjSize = 0;
                free(graph->v[i]);
                graph->v[i] = NULL;
            }
        }
        free(graph->v);
        graph->v = NULL;
    }
    graph->nrNodes = 0;
}

void bfs(Graph *graph, Node *s, Operation *op)
{
    for (int i = 0; i < graph->nrNodes; i++) {
        if (op != NULL) op->count(3);
        graph->v[i]->color = COLOR_WHITE;
        graph->v[i]->dist = -1;
        graph->v[i]->parent = NULL;
    }

    if(op!=NULL)op->count(3);
    s->color = COLOR_GRAY;
    s->dist = 0;
    s->parent = NULL;

    Node** coada=(Node**)malloc(graph->nrNodes*sizeof(Node*));
    int first = 0;
    int last = 0;

    coada[last++] = s;

    while (first != last) {
        Node* cur = coada[first++];

        if (cur->adj != NULL) {
            for (int i = 0; i < cur->adjSize; i++) {
                Node* v = cur->adj[i];
                if (op != NULL) op->count();
                if (v->color == COLOR_WHITE) {
                    if (op != NULL) op->count(3);
                    v->color = COLOR_GRAY;
                    v->dist = cur->dist + 1;
                    v->parent = cur;

                    coada[last++] = v;
                }
            }
        }
        cur->color = COLOR_BLACK;
    }
    free(coada);
}


void pretty_print_R2(NodeR2* node, int level) {
    if (node == NULL) return;
    for (int i = 0; i < level; i++) {
        printf("    ");
    }printf("(%d, %d)\n", node->key.row, node->key.col);


    for (int i = 0; i < node->nc; i++) {
        pretty_print_R2(node->children[i], level + 1);
    }
}

NodeR2* R1_to_R2(int* p, Point* reprezent, int n) {
    NodeR2** rez = (NodeR2**)malloc(n * sizeof(NodeR2*));
    int* childCount = (int*)calloc(n, sizeof(int));
    int root = 0;

    for (int i = 0; i < n; i++) {
        rez[i] = (NodeR2*)malloc(sizeof(NodeR2));
        rez[i]->key = reprezent[i];
        rez[i]->children = NULL;
        rez[i]->nc = 0;
    }

    for (int i = 0; i < n; i++) {
        if (p[i] == -1) root = i;
        else childCount[p[i]]++;
    }

    for (int i = 0; i < n; i++) {
        if (childCount[i] > 0) {
            rez[i]->children = (NodeR2**)malloc(childCount[i] * sizeof(NodeR2*));
            rez[i]->nc = childCount[i];
            childCount[i] = 0;//resetare
        }
    }

    for (int i = 0; i < n; i++) {
        if (p[i] != -1) {
            rez[p[i]]->children[childCount[p[i]]++] = rez[i];
        }
    }
    free(childCount);
    return rez[root];
}

void print_bfs_tree(Graph *graph)
{
    //first, we will represent the BFS tree as a parent array
    int n = 0; //the number of nodes
    int *p = NULL; //the parent array
    Point *repr = NULL; //the representation for each element in p

    //some of the nodes in graph->v may not have been reached by BFS
    //p and repr will contain only the reachable nodes
    int *transf = (int*)malloc(graph->nrNodes * sizeof(int));
    for(int i=0; i<graph->nrNodes; ++i){
        if(graph->v[i]->color == COLOR_BLACK){
            transf[i] = n;
            ++n;
        }else{
            transf[i] = -1;
        }
    }
    if(n == 0){
        //no BFS tree
        free(transf);
        return;
    }

    int err = 0;
    p = (int*)malloc(n * sizeof(int));
    repr = (Point*)malloc(n * sizeof(Node));
    for(int i=0; i<graph->nrNodes && !err; ++i){
        if(graph->v[i]->color == COLOR_BLACK){
            if(transf[i] < 0 || transf[i] >= n){
                err = 1;
            }else{
                repr[transf[i]] = graph->v[i]->position;
                if(graph->v[i]->parent == NULL){
                    p[transf[i]] = -1;
                }else{
                    err = 1;
                    for(int j=0; j<graph->nrNodes; ++j){
                        if(graph->v[i]->parent == graph->v[j]){
                            if(transf[j] >= 0 && transf[j] < n){
                                p[transf[i]] = transf[j];
                                err = 0;
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
    free(transf);
    transf = NULL;

    if(!err){
        NodeR2* root = R1_to_R2(p, repr, n);
        pretty_print_R2(root, 0);
    }

    if(p != NULL){
        free(p);
        p = NULL;
    }
    if(repr != NULL){
        free(repr);
        repr = NULL;
    }
}

int shortest_path(Graph *graph, Node *start, Node *end, Node *path[])
{
    bfs(graph, start, NULL);
    if (end->color != COLOR_BLACK) return -1;

    int lung = 0;
    Node* cur = end;

    while (cur != NULL) {
        path[lung++] = cur;
        cur = cur->parent;
    }

    //inversare drum
    for (int i = 0; i < lung / 2; i++) {
        Node* tmp = path[i];
        path[i] = path[lung - 1 - i];
        path[lung - 1 - i] = tmp;
    }
    return lung;
}

void generareGrafConex(int m, Graph* g){

    int n = g->nrNodes;
    int muchieCur = 0;

    //lant pt a fi conex
    for (int i = 0; i < n - 1; i++) {
        g->v[i]->adj[g->v[i]->adjSize++] = g->v[i+1];
        g->v[i+1]->adj[g->v[i+1]->adjSize++] = g->v[i];
        muchieCur++;
    }

    while (muchieCur < m) {
        int u = rand() % n;
        int v = rand() % n;
        int dupl = 0;
        if (u != v) {
            for (int j = 0; j < g->v[u]->adjSize; j++) {
                if (g->v[u]->adj[j]==g->v[v]) {
                    dupl = 1;
                    break;
                }
            }
        }
        if (dupl == 0 && u != v) {
            g->v[u]->adj[g->v[u]->adjSize++] = g->v[v];
            g->v[v]->adj[g->v[v]->adjSize++] = g->v[u];
            muchieCur++;
        }
    }
}

void performance()
{
    srand(time(NULL));
    int n, i;
    Profiler p("bfs");

    // vary the number of edges
    for(n=1000; n<=4500; n+=100){
        Operation op = p.createOperation("bfs-edges", n);
        Graph graph;
        graph.nrNodes = 100;
        //initialize the nodes of the graph
        graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        for(i=0; i<graph.nrNodes; ++i){
            graph.v[i] = (Node*)malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
            graph.v[i]->adj = (Node**)malloc(n * sizeof(Node*));
            graph.v[i]->adjSize = 0;
        }
        // TODO: generate n random edges
        // make sure the generated graph is connected
        generareGrafConex(n, &graph);


        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }

    // vary the number of vertices
    for(n=100; n<=200; n+=10){
        Operation op = p.createOperation("bfs-vertices", n);
        Graph graph;
        graph.nrNodes = n;
        //initialize the nodes of the graph
        graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        for(i=0; i<graph.nrNodes; ++i){
            graph.v[i] = (Node*)malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
            graph.v[i]->adj = (Node**)malloc(4500 * sizeof(Node*));
            graph.v[i]->adjSize = 0;
        }
        // TODO: generate 4500 random edges
        // make sure the generated graph is connected
        generareGrafConex(4500, &graph);

        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }

    p.showReport();
}
