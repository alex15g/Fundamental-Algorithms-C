
/*
* Gavris Alexandru Catalin 
* Arbori Multicai
*
* Am implementat urmatorii algoritmi: 
* -traversare unui arbore binar: inorder, preorder, postorder-recursiv, inorder-morris(iterativ)
* -transformarea T1(parent->multi-way)
* -transformarea T2(multi-way->binary)
* -pretty_print pt: R1, R2 si R3
* 
* In R1 folosim indexare 1-based pentru claritate (copiii sunt 1..n), 
* iar vectorul Pi este 0-based. In R2 si R3 revenim la indexare 0-based in interiorul structurii.
*
* Analiza:
* - R1_to_R2 are complexitate liniara O(n) in timp deoarece 
*     ~parcurgem vectorul de parinti o singura data pt a numara cati copii are fiecare nod [O(n)],
*     ~aloc memorie pt fiecare nod si pt vector copii [O(n+total copii)=O(n)], 
*     ~construim legaturile copiilor[O(n)],
*     =>deci O(n)
* -R1_to_R2 are complexitate liniara O(n) in spatiu deoarece
*     ~vectorul rez are O(n)
*     ~vectorul temp childCount tot O(n)
*     ~nu se folosesc stive recurente aditionale
*     =>deci O(n)
* 
* -R2_to_R3 are tot complexitate liniara in timp O(n) deoarece
*      ~functia viziteaza fiecare nod o singura data si toate operatiile pt un nod sunt O(1)
* -R2_to_R3 are complexitate O(n+h)=O(n) in spatiu deoarece
*      ~fiecare nod R3 e alocat dinamic,O(n) memorie pt noduri
*      ~recursia foloseste stiva, dar adancimea maxima e inaltime arbor R2, O(h), care e ft putin comparativ cu O(n)
* 
* =>deci algoritmii sunt eficienti
* 
* 
* Algoritmul este implementat corect si testat pe exemple mici in functia demo
*/



#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef struct Node {
	int val;
	struct Node* left, * right;
}Node;

typedef struct Node1 {
	int val;
	struct Node1* next;
}Node1;

typedef struct NodeR2 {
	int key;
	int nc;
	struct NodeR2** children;
}NodeR2;

typedef struct NodeR3 {
	int key;
	struct NodeR3* c, *s;
}NodeR3;

void inorder_rec(Node* r) {
	if (r == NULL) return;

	inorder_rec(r->left);
	printf("%d ", r->val);
	inorder_rec(r->right);
}

void preorder_rec(Node* r) {
	if (r == NULL) return;

	printf("%d ", r->val);
	preorder_rec(r->left);
	preorder_rec(r->right);
}

void postorder_rec(Node* r) {
	if (r == NULL) return;

	postorder_rec(r->left);
	postorder_rec(r->right);
	printf("%d ", r->val);
}

void inorder_morris(Node* r) {
	Node* cur = r;
	while (cur != NULL) {
		if (cur->left == NULL) {
			printf("%d ", cur->val);
			cur = cur->right;

		}
		else {
			Node* pred = cur->left;

			while (pred->right != NULL && pred->right != cur)
				pred = pred->right;

			if (pred->right == NULL) {
				pred->right = cur;
				cur = cur->left;
			}
			else {
				pred->right = NULL;
				printf("%d ", cur->val);
				cur = cur->right;
			}
		}
	}
}

void add_child(Node1** children, int parent, int child) {
	Node1* new_n = (Node1*)malloc(sizeof(Node1));
	new_n->val = child;
	new_n->next = NULL;
	if(children[parent] == NULL) children[parent]= new_n;
	else {
		Node1* cur = children[parent];
		while (cur->next!=NULL) cur = cur->next;
		cur->next = new_n;
	}
}

void pretty_print_R1(Node1** children, int node, int level) {
	for (int i = 0; i < level; i++) {
		printf("    ");
	}printf("%d\n", node);

	Node1* cur = children[node];
	while (cur != NULL) {
		pretty_print_R1(children, cur->val, level + 1);
		cur = cur->next;
	}
}

NodeR2* R1_to_R2(int* p, int n) {
	NodeR2** rez = (NodeR2**)malloc(n*sizeof(NodeR2*));
	int* childCount = (int*)calloc(n, sizeof(int));
	int root = 0;

	for (int i = 0; i < n; i++) {
		rez[i]=(NodeR2*) malloc(sizeof(NodeR2));
		rez[i]->key = i + 1;
		rez[i]->children = NULL;
		rez[i]->nc = 0;
	}

	for (int i = 0; i < n; i++) {
		if (p[i] == -1) root = i;
		else childCount[p[i] - 1]++;
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
			rez[p[i] - 1]->children[childCount[p[i] - 1]++] = rez[i];
		}
	}
	free(childCount);
	return rez[root];
}

void pretty_print_R2(NodeR2* node, int level) {
	if (node == NULL) return;
	for (int i = 0; i < level; i++) {
		printf("    ");
	}printf("%d\n", node->key);

	
	for(int i=0; i<node->nc; i++){
		pretty_print_R2(node->children[i], level + 1);
	}
}

NodeR3* R2_to_R3(NodeR2* root) {
	if (root == NULL) return NULL;

	NodeR3* r3 = (NodeR3*)malloc(sizeof(NodeR3));
	r3->key = root->key;
	r3->c = r3->s = NULL;

	if (root->nc > 0) {
		r3->c = R2_to_R3(root->children[0]);

		NodeR3* cur = r3->c;

		for (int i = 1; i < root->nc; i++) {
			cur->s = R2_to_R3(root->children[i]);
			cur = cur->s;
		}
	}
	return r3;
}

void pretty_print_R3(NodeR3* node, int level) {
	if (node == NULL) return;
	for (int i = 0; i < level; i++) {
		printf("    ");
	}printf("%d\n", node->key);

	pretty_print_R3(node->c, level + 1);
	pretty_print_R3(node->s, level);
}

void free_R2(NodeR2* node) {
	if (node==NULL) return;
	for (int i = 0; i < node->nc; i++) free_R2(node->children[i]);
	free(node->children);
	free(node);
}

void free_R3(NodeR3* node) {
	if (node == NULL) return;
	free_R3(node->c);
	free_R3(node->s);
	free(node);
}

void demo_arbore_binar_traversari() {
	Node* n1 =(Node*) malloc(sizeof(Node)); 
	n1->val = 1;
	n1->left = n1->right = NULL;

	Node* n3 = (Node*)malloc(sizeof(Node));
	n3->val = 3;
	n3->left = n3->right = NULL;

	Node* n5 = (Node*)malloc(sizeof(Node));
	n5->val = 5;
	n5->left = n5->right = NULL;

	Node* n2 = (Node*)malloc(sizeof(Node));
	n2->val = 2;
	n2->left = n1;
	n2->right = n3;

	Node* n6 = (Node*)malloc(sizeof(Node));
	n6->val = 6;
	n6->left = n5;
	n6->right = NULL;

	Node* root = (Node*)malloc(sizeof(Node));
	root->val = 4;
	root->left = n2;
	root->right = n6;

	printf("INORDER recursiv: ");
	inorder_rec(root);
	printf("\n");

	printf("PREORDER recursiv: ");
	preorder_rec(root);
	printf("\n");

	printf("POSTORDER recursiv: ");
	postorder_rec(root);
	printf("\n");

	printf("INORDER iterativ-Morris: ");
	inorder_morris(root);
	printf("\n");

	free(n1);
	free(n3);
	free(n5);
	free(n2);
	free(n6);
	free(root);
}

void demo() {
	int Pi[] = {2, 7, 5, 2, 7, 7, -1, 5, 2};
	int n = sizeof(Pi) / sizeof(Pi[0]);

	demo_arbore_binar_traversari();

	Node1** children = (Node1**)malloc((n + 1) * sizeof(Node1*));


	for (int i = 0; i <= n; i++) children[i] = NULL;

	int root = -1;
	for (int i = 0; i < n; i++) {
		if (Pi[i] == -1) root = i + 1;
		else add_child(children, Pi[i], i + 1);
	}
	printf("PrettyPrint R1:\n\n");
	pretty_print_R1(children, root, 0);

	NodeR2* rootR2 = R1_to_R2(Pi, n);

	printf("\n\nPrettyPrint R2:\n\n");
	pretty_print_R2(rootR2, 0);

	NodeR3* r3 = R2_to_R3(rootR2);
	printf("\n\nPrettyPrint R3:\n\n");
	pretty_print_R3(r3, 0);


	for (int i = 1; i <= n; i++) {
		Node1* cur = children[i];
		while (cur != NULL) {
			Node1* tmp = cur;
			cur = cur->next;
			free(tmp);
		}
	}
	free(children);
	free_R2(rootR2);
	free_R3(r3);
}

int main() {
	demo();
	return 0;
}
