/*
* Gavris Alexandru
* Arbori de statistica de ordine
* 
* Am imp,ementat urmatoarele functii:
* -newNode->O(1)
* -buildTree->O(n)
* -osSelect->selecteaza elementul de rang i
* -osDelete->sterge elementul de rang i
* 
* Analiza:
* -build:o(n) confirmata si de grafic
* -select:o(nlogn) ptc select n elemente, o(logn) ar fi doar pt select simplu
* -delete:o(nlogn), aceioasi explicatie ca pt select, dar e mai neeficient ca selectul
* 
* Algoritmii sunt implementati corect si testati in functia demo
*/

#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"
#include <time.h>

#define MAX_SIZE 10000
#define nr_tests 5

Profiler p("Analiza");

typedef struct Node {
	int key;
	int size;
	struct Node* left;
	struct Node* right;
}Node;



Node* newNode(int key) {
	Node* n = (Node*)malloc(sizeof(Node));
	n->size = 1;
	n->key = key;
	n->left = n->right = NULL;
	return n;
}

Node* buildTree(int l, int r, Operation* opComp, Operation* opAtr) {
	opComp->count();
	if (l > r)return NULL;
	int m = (l + r) / 2;
	Node* n = newNode(m);
	opAtr->count();
	n->left = buildTree(l, m - 1, opComp, opAtr);
	opAtr->count();
	n->right = buildTree(m+1, r, opComp, opAtr);
	int szLeft = n->left ? n->left->size : 0;
	int szRight = n->right ? n->right->size : 0;

	opAtr->count();
	n->size = 1 + szLeft + szRight;
	return n;
}

Node* osSelect(Node* root, int i, Operation* opComp, Operation* opAtr) {
	if (root == NULL) return NULL;

	int szLeft = root->left ? root->left->size : 0;
	int rank = szLeft + 1;

	opComp->count();
	if (rank == i) return root;
	else {
		opComp->count();
		if (i<rank) return osSelect(root->left, i, opComp, opAtr);
		else return osSelect(root->right, i-rank, opComp, opAtr);
	}
}

Node* osDelete(Node* root, int i, Operation* opComp, Operation* opAtr) {
	if (root == NULL) return NULL;

	int szLeft = root->left ? root->left->size : 0;
	int rank = szLeft + 1;

	opComp->count();
	if (rank == i) {
		Node* tmp;
		if (root->left == NULL) {
			tmp = root->right;
			free(root);
			return tmp;
		}
		else if (root->right == NULL) {
			tmp = root->left;
			free(root);
			return tmp;
		}
		else {
			Node* suc = root->right;
			opComp->count();
			while (suc->left != NULL) {
				suc = suc->left;
				opComp->count();
			}
			root->key = suc->key;
			opAtr->count();
			root->right = osDelete(root->right, 1, opComp, opAtr);
			opAtr->count();
			}
		
	}
	else {
		opComp->count();
		opAtr->count();
		if (i < rank) root->left = osDelete(root->left, i, opComp, opAtr);
		else root->right = osDelete(root->right, i-rank, opComp, opAtr);
	}
	szLeft = root->left ? root->left->size : 0;
	int szRight = root->right ? root->right->size : 0;

	opAtr->count();
	root->size = 1 + szLeft + szRight;
	

	return root;
}

void pretty_print(Node* root, int level) {
	if (root == NULL) return;
	for (int i = 0; i < level; i++) {
		printf("    ");
	}printf("%d(%d)\n", root->key, root->size);

	pretty_print(root->left, level + 1);
	pretty_print(root->right, level + 1);
}

void demo(Operation* opComp, Operation* opAtr) {
	int n = 11;
	Node* root = buildTree(1, n, opComp, opAtr);

	printf("Arbore initial:\n");
	pretty_print(root, 0);

	for (int j = 0; j < 3; j++) {
		int idx = rand() % n + 1; 
		Node* sel = osSelect(root, idx, opComp, opAtr);
		if (sel!=NULL)
			printf("\nIndexul selectat %d -> key=%d\n", idx, sel->key);
	}

	printf("\nArborele dupa stergeri\n");
	for (int j = 0; j < 3; j++) {
		int idx = rand() % n + 1;
		printf("\n Index de sters %d:\n", idx);
		root = osDelete(root, idx, opComp, opAtr);
		n--;
		pretty_print(root, 0);
	}

}

void perf() {
	p.reset("arbori de statistica de ordine");
	for (int n = 100; n <= MAX_SIZE; n += 100) {
		Operation opComp_build = p.createOperation("build-comp", n);
		Operation opAtr_build = p.createOperation("build-atr", n);
		Operation opComp_select = p.createOperation("select-comp", n);
		Operation opAtr_select = p.createOperation("select-atr", n);
		Operation opComp_delete = p.createOperation("delete-comp", n);
		Operation opAtr_delete = p.createOperation("delete-atr", n);

		for (int i = 0; i < nr_tests; i++) {
			Node* root = buildTree(1, n, &opComp_build, &opAtr_build);
			int n_curent = n;
			for (int j = 0; j < n; j++) {
				if (n_curent > 0) {
					int indx = rand() % n_curent + 1;
					osSelect(root, indx, &opComp_select, &opAtr_select);
					root=osDelete(root, indx, &opComp_delete, &opAtr_delete);
					n_curent--;
				}
			}
		}
	}
	p.divideValues("build-atr", nr_tests);
	p.divideValues("build-comp", nr_tests);
	p.divideValues("select-atr", nr_tests);
	p.divideValues("select-comp", nr_tests);
	p.divideValues("delete-atr", nr_tests);
	p.divideValues("delete-comp", nr_tests);

	p.addSeries("build-total", "build-atr", "build-comp");
	p.addSeries("select-total", "select-atr", "select-comp");
	p.addSeries("delete-total", "delete-atr", "delete-comp");

	p.createGroup("OS_TREE", "build-total", "select-total", "delete-total");
	p.showReport();
}


int main() {
	srand(time(0));
	int n = MAX_SIZE;
	//Operation opComp = p.createOperation("nr_comp", n);
	//Operation opAtr = p.createOperation("nr_atr", n);

	//demo(&opComp, &opAtr);

	perf();



	return 0;
}
