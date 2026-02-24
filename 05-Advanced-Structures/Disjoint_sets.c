/*
* Gavris Alexandru 
* Multimi disjuncte
*
* Implementari:
* -MAKE_SET->creeaza o multime noua
* -FIND_SET->cauta reprezentantul unui set cu path compression
* -UNION->uneste 2 seturi
* -kruskal->cpnstruieste MST folosind cei 3 alg de mai sus
* 
* Analiza complexitatilor:
* -alfa(n)=functia inversa Ackermann
* -MAKE_SET->O(1)
* -FIND_SET->O(alfa(n))
* -UNION->O(alfa(n))
* -Kruskal->O(mlogm+n alfa(n)), m=muchii, n=noduri
* 
* Observatii grafice:
* -nr_opMake creste liniar cu nr de noduri n, cel mai eficient
* -nr_opFind creste aproximativ proportional cu nr de muchii
* -nr_opUnion creste liniar cu nr de noduri, mai eficient decat nr_opFind
* 
* Algoritmii sunt implementati corect si testati in functia demo.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Profiler.h"

Profiler p("Kruskal");
#define MAX_SIZE 10000

typedef struct elem {
	int k;
	int rank;
	struct elem* p;
}elem;

typedef struct {
	int a, b, p;
}Edge;

int part(Edge a[], int l, int r) {
	Edge piv = a[r];
	int j = l - 1;
	for (int i = l; i < r; i++) {
		////op->count();
		if (a[i].p < piv.p) {
			j++;
			//op->count(3);
			Edge aux = a[i];
			a[i] = a[j];
			a[j] = aux;
		}
	}
	//op->count(3);
	Edge aux = a[j + 1];
	a[j + 1] = a[r];
	a[r] = aux;
	return j + 1;
}

void quickSort(Edge a[], int l, int r) {
	if (l < r) {
		int piv = part(a, l, r);
		quickSort(a, l, piv - 1);
		quickSort(a, piv + 1, r);
	}
}


elem* MAKE_SET(int x, Operation* op) {
	elem* node = (elem*)malloc(sizeof(elem));
	node->k = x;
	node->rank = 0;
	node->p = node;
	op->count(3);
	return node;
}

elem* FIND_SET(elem* x, Operation* op) {
	op->count();
	if (x == x->p)return x;
	elem* r = FIND_SET(x->p, op);
	op->count();
	x->p = r;
	return r;
}

void UNION(elem* x, elem* y, Operation* op) {
	elem* rx = FIND_SET(x, op);
	elem* ry = FIND_SET(y, op);

	op->count();
	if (rx == ry) return;//sunt in acelasi set

	op->count();
	if (rx->rank < ry->rank) {
		rx->p = ry;
		op->count();
	}
	else {
		ry->p = rx;
		op->count();

		op->count();
		if (rx->rank == ry->rank) {
			rx->rank++;
			op->count();
		}
	}

}

void PRINT_SETS(elem* sets[], int n, Operation* op) {
	for (int i = 0; i < n; i++) {
		elem* r = FIND_SET(sets[i], op);
		printf("elementul %d e reprezentat de: %d\n", sets[i]->k, r->k);
	}
}

int kruskal(int n, Edge edges[],int  m, Edge mst[], Operation* opMake, Operation* opFind, Operation* opUnion) {
	elem** nodes = (elem**)malloc(n * sizeof(elem*));
	for (int i = 0; i < n; i++)
		nodes[i] = MAKE_SET(i, opMake);

	quickSort(edges, 0, m-1);

	int count = 0;
	for (int i = 0; i < m && count < n - 1; i++) {
		int u = edges[i].a;
		int v = edges[i].b;
		elem* setU = FIND_SET(nodes[u], opFind);
		elem* setV = FIND_SET(nodes[v], opFind);
		if (setU != setV) {
			mst[count] = edges[i];
			UNION(setU, setV, opUnion);
			count++;
		}
	}
	return count;
}

void demo(Operation *op) {
	elem* s[10];
	for (int i = 0; i < 10; i++) {
		s[i] = MAKE_SET(i + 1, op);
	}
	PRINT_SETS(s, 10, op);

	printf("\n");

	UNION(s[0], s[1], op);
	UNION(s[2], s[3], op);
	UNION(s[2], s[1], op);
	UNION(s[4], s[1], op);
	PRINT_SETS(s, 10, op);
}

void demo_Kruskal(Operation* opMake, Operation* opFind, Operation* opUnion) {
	int n = 5, m = 9;

	Edge edges[9] = { {0,1,2}, {0,2,4}, {0,3,1}, {0,4,3},{1,2,5}, {1,3,2}, {1,4,3},{2,3,4}, {3,4,1} };
	printf("\nmuchii initiale:\n");
	for (int i = 0; i < m; i++) {
		printf("(%d-%d) -> %d\n", edges[i].a + 1, edges[i].b + 1, edges[i].p);
	}

	Edge mst[4];
	int cnt = kruskal(n, edges, m, mst, opMake, opFind, opUnion);
	printf("\nmuchii alese in MST:\n");
	for (int i = 0; i < cnt; i++) {
		printf("(%d-%d) -> %d\n", mst[i].a + 1, mst[i].b + 1, mst[i].p);
	}
}

void generareGrafConex(int n, Edge* edges) {
	int m = n * 4;
	int idx = 0;

	//lant pt a fi conex
	for (int i = 0; i < n - 1; i++) {
		edges[idx].a = i;
		edges[idx].b = i + 1;
		edges[idx].p = rand() % 1000 + 1;
		idx++;
	}

	while (idx < m) {
		int u = rand() % n;
		int v = rand() % n;
		int dupl = 0;
		if (u != v) {
			for (int j = 0; j < idx; j++) {
				if ((edges[j].a == u && edges[j].b == v) || (edges[j].a == v && edges[j].b == u)) {
					dupl = 1;
					break;
				}
			}
		}
		if (dupl == 0) {
			edges[idx].a = u;
			edges[idx].b = v;
			edges[idx].p = rand() % 1000 + 1;
			idx++;
		}
	}

}
void perf() {
	for (int n = 100; n <= MAX_SIZE; n += 100) {
		int m = n * 4;
		Edge* edges = (Edge*)malloc(m * sizeof(Edge));
		Edge* mst = (Edge*)malloc((n-1) * sizeof(Edge));
		generareGrafConex(n, edges);

		Operation opMake = p.createOperation("nr_opMake", n);
		Operation opFind = p.createOperation("nr_opFind", n);
		Operation opUnion = p.createOperation("nr_opUnion", n);

		kruskal(n, edges, m, mst, &opMake, &opFind, &opUnion);
	}

	p.createGroup("MAKE-FIND-UNION", "nr_opMake", "nr_opFind", "nr_opUnion");
	p.showReport();
}

int main() {
	srand(time(NULL));
	int n = MAX_SIZE;
	/*Operation op = p.createOperation("nr_op", n);
	Operation opMake = p.createOperation("nr_op", n);
	Operation opFind = p.createOperation("nr_op", n);
	Operation opUnion = p.createOperation("nr_op", n);
	demo(&op);
	demo_Kruskal(&opMake, &opFind, &opUnion);
	*/

	perf();
	return 0;
}
