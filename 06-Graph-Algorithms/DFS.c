/*
 * Gavris Alexandru 
 * Parcurgerea unui graf utilizand DFS
 *
 * Implementari realizate:
 * - dfs_visit / dfs -> parcurgere Depth-First Search a unui graf reprezentat prin liste de adiacenta
 * - sortare_topologica -> genereaza lista de noduri sortate topologic (daca graful nu contine ciclu)
 * - tarjan / strongConnect -> determina componentele puternic conexe  intr-un graf orientat
 *
 * Analiza complexitatilor:
 * - dfs / dfs_visit -> O(V + E), unde V = nr noduri, E = nr muchii
 * - sortare_topologica -> O(V + E), deoarece se bazeaza pe DFS
 * - tarjan / strongConnect -> O(V + E), fiecare nod si fiecare muchie sunt procesate o singura data
 *
 * Observatii grafice:
 * - muchii_fixe_dfs creste liniar cu numarul de noduri
 * - varduri_fixe_dfs creste  liniar cu numarul de muchii
 *
 * Algoritmurile: DFS, Tarjan si sortarea topologica sunt implementate corect datorita testelor din functia demo.
 */


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Profiler.h"

Profiler p("DFS");
#define MAX_SIZE 100


typedef enum {
	WHITE, GRAY, BLACK
}color;

typedef struct Node {
	int v;
	struct Node* next;
	int ti, tf;
	//int parent;
	struct Node** adj;
	int adjSize;
	color c;

	int index;
	int lowLink;
	int onStack;
}Node;

typedef struct {
	int n;
	Node* nodes;
}Graf;

typedef struct List {
	Node* x;
	struct List* next;
}List;

typedef struct Stack {
	Node* v[MAX_SIZE];
	int first;
};

int Time = 0;
boolean cycle = FALSE;
List* topo = NULL;
int index;
Stack s;

void initStiva(Stack* s) {
	s->first = -1;
}

void push(Stack* s, Node* x) {
	s->v[++(s->first)] = x;
}

Node* pop(Stack* s) {
	return s->v[(s->first)--];
}

int eGoala(Stack* s) {
	return s->first == -1;
}

void dfs_visit(Node* x, Operation* op) {
	x->ti = ++Time;
	x->c = GRAY;
	op->count(2);

	for (int i = 0; i < x->adjSize; i++) {
		Node* y = x->adj[i];
		op->count(2);
		if (y->c == WHITE) {
			dfs_visit(y, op);
		}
		else if (y->c == GRAY) cycle = TRUE;
	}
	x->c = BLACK;
	x->tf = ++Time;
	op->count(2);

	List* node = (List*)malloc(sizeof(List));
	node->x = x;
	node->next = topo;
	topo = node;
}

void dfs(Graf* g, Operation* op) {
	Time = 0;
	cycle = FALSE;
	for (int i = 0; i < g->n; i++) {
		g->nodes[i].c = WHITE;
		op->count(2);
	}
	
	for (int i = 0; i < g->n; i++) {
		op->count();
		if (g->nodes[i].c == WHITE) dfs_visit(&g->nodes[i], op);

	}
}

List* sortare_topologica(Graf* g, Operation* op) {
	topo = NULL;
	dfs(g, op);

	if (cycle) {
		printf("Nu se poate sorta topologic ca avem ciclu");
		return NULL;
	}
	
	return topo;
}

void printGraf(Graf* g) {
	printf("Graful initial e:\n");
	for (int i = 0; i < g->n; i++) {
		printf("%d: ", g->nodes[i].v);
		for (int j = 0; j < g->nodes[i].adjSize; j++) {
			printf("%d ", g->nodes[i].adj[j]->v);
		}
		printf("\n");
	}
	printf("\n");
}

void printDFS(Graf* g) {
	printf("Graful rezultat e:\n");
	for (int i = 0; i < g->n; i++) {
		printf("%d: ti=%d, tf=%d \n", g->nodes[i].v, g->nodes[i].ti, g->nodes[i].tf);
	}
	printf("\n");
}

void printSortare(List* l) {
	printf("sortarea topologica este:\n");
	for (List* p = l; p != NULL; p = p->next) {
		printf("%d ", p->x->v);
	}
	printf("\n\n");
}

void adaugaMuchie(Graf* g, int u, int v) {
	Node* nodu = &g->nodes[u];
	nodu->adjSize++;
	nodu->adj = (Node**)realloc(nodu->adj, nodu->adjSize * sizeof(Node*));
	nodu->adj[nodu->adjSize - 1] = &g->nodes[v];
}


void strongConnect(Node* v) {
	v->index = index;
	v->lowLink = index;
	index++;
	push(&s, v);
	v->onStack = 1;

	for (int i = 0; i < v->adjSize; i++) {
		Node* w = v->adj[i];
		if (w->index == -1) {
			strongConnect(w);
			if (w->lowLink < v->lowLink)v->lowLink = w->lowLink;
		}
		else if (w->onStack) {
			if (w->index < v->lowLink)v->lowLink = w->index;
		}
	}
	if (v->lowLink == v->index) {
		printf("Componenta puternic conexa: ");
		Node* w;
		do {
			w = pop(&s);
			w->onStack = 0;
			printf("%d ", w->v);
		} while (w != v);
		printf("\n");
	}
}

void tarjan(Graf* g) {
	for (int i = 0; i < g->n; i++) {
		g->nodes[i].index = -1;
		g->nodes[i].lowLink = -1;
		g->nodes[i].onStack = 0;
	}
	index = 0;
	initStiva(&s);

	for (int i = 0; i < g->n; i++) {
		if (g->nodes[i].index == -1)strongConnect(&g->nodes[i]);
	}
}

void demo(Operation* op) {
	printf("Demo DFS:\n");
	int n = 8;
	Graf g;
	g.n = n;
	g.nodes = (Node*)calloc(n, sizeof(Node));

	for (int i = 0; i < n; i++) {
		g.nodes[i].v = i;
		g.nodes[i].adj = NULL;
		g.nodes[i].adjSize = 0;
		g.nodes[i].c = WHITE;
		g.nodes[i].tf = 0;
		g.nodes[i].ti = 0;
		g.nodes[i].index = -1;
		g.nodes[i].lowLink = -1;
		g.nodes[i].onStack = 0;
	}

	adaugaMuchie(&g, 0, 1);
	adaugaMuchie(&g, 0, 2);
	adaugaMuchie(&g, 1, 3);
	adaugaMuchie(&g, 2, 4);
	adaugaMuchie(&g, 4, 5);
	adaugaMuchie(&g, 6, 7);
	adaugaMuchie(&g, 7, 4);

	printGraf(&g);

	List* l = sortare_topologica(&g, op);

	printSortare(l);

	printDFS(&g);

	printf("Demo tarjan:\n");
	Graf gt;
	gt.n = n;
	gt.nodes = (Node*)calloc(n, sizeof(Node));

	for (int i = 0; i < n; i++) {
		gt.nodes[i].v = i;
		gt.nodes[i].adj = NULL;
		gt.nodes[i].adjSize = 0;
		gt.nodes[i].c = WHITE;
		gt.nodes[i].tf = 0;
		gt.nodes[i].ti = 0;
		gt.nodes[i].index = -1;
		gt.nodes[i].lowLink = -1;
		gt.nodes[i].onStack = 0;
	}

	adaugaMuchie(&gt, 0, 1);
	adaugaMuchie(&gt, 1, 2);
	adaugaMuchie(&gt, 2, 0);
	adaugaMuchie(&gt, 3, 1);
	adaugaMuchie(&gt, 3, 2);
	adaugaMuchie(&gt, 3, 4);
	adaugaMuchie(&gt, 4, 5);
	adaugaMuchie(&gt, 5, 6);
	adaugaMuchie(&gt, 6, 4);
	adaugaMuchie(&gt, 6, 7);

	printGraf(&gt);

	printf("Componente puternic conexe: \n");
	tarjan(&gt);
}

void generareGraf(Graf* g, int m) {
	int n = g->n;
	int muchieCur = 0;

	while (muchieCur < m) {
		int uI = rand() % n;
		int vI = rand() % n;

		int dupl = 0;
		if (uI != vI) {
			Node* u = &g->nodes[uI];
			Node* v = &g->nodes[vI];


			for (int j = 0; j < u->adjSize; j++) {
				if (u->adj[j] == v) {
					dupl = 1;
					break;
				}
			}

			if (dupl == 0) {
				u->adjSize++;
				u->adj = (Node**)realloc(u->adj, u->adjSize * sizeof(Node*));
				u->adj[u->adjSize - 1] = v;


				muchieCur++;
			}
		}
	}
}

void perf() {
	for (int e = 1000; e <= 4500; e += 100) {
		Operation opVfix = p.createOperation("varfuri_fixe_dfs", e);
		Graf g;
		g.n = 100;
		g.nodes = (Node*)calloc(g.n, sizeof(Node));

		for (int i = 0; i < g.n; i++) {
			g.nodes[i].v = i;
			g.nodes[i].adj = NULL;
			g.nodes[i].adjSize = 0;
			g.nodes[i].c = WHITE;
			g.nodes[i].tf = 0;
			g.nodes[i].ti = 0;
		}
		generareGraf(&g, e);

		dfs(&g, &opVfix);
	}

	for (int v = 100; v <= 200; v += 10) {
		Operation opEfix = p.createOperation("muchii_fixe_dfs", v);
		Graf g;
		g.n = v;
		g.nodes = (Node*)calloc(g.n, sizeof(Node));

		for (int i = 0; i < g.n; i++) {
			g.nodes[i].v = i;
			g.nodes[i].adj = NULL;
			g.nodes[i].adjSize = 0;
			g.nodes[i].c = WHITE;
			g.nodes[i].tf = 0;
			g.nodes[i].ti = 0;
		}
		generareGraf(&g, 4500);

		dfs(&g, &opEfix);
	}

	//p.createGroup("Analiza DFS", "varfuri_fixe", "muchii_fixe");
	p.showReport();
}

int main() {
	srand(time(NULL));
	int n = MAX_SIZE;
	Operation op = p.createOperation("nr_op", n);
	demo(&op);

	//perf();
	return 0;
}
