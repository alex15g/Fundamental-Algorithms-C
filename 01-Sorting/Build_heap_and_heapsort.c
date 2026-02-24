/*
*Gavris Alexandru Catalin 
*Am implementat 4 algoritmi: Bubble Sort iterativ+recursiv, constructia Heap(Bottom-up+Top-Down), Heap Sort.
*Am efectuat analiza in cele 2 cazuri cerute pt heaps: average, worst (sir sortat crescator)
*De asemenea, pt algoritmii bubble, am analizat complexitatea atat in timp cat si in spatiu (am contorizat comparatiile, atribuirile si suma lor si am creat graficele comparative)
*Cei 4 algoritmi sunt implementati corecti si testati pe exemple mici in functiile demo
*
* INTERPRETARE GRAFICE:
* BubbleSort iterativ e usor mai eficient dpdv al timpului datorita lipsei apelurilor recursive, complexitatea fiind O(n^2) in cazul average
* Pt heap construction: Bottom-Up are complexitate O(n), iar Top-down O(nlogn)
* Pt HeapSort avem complexitatea O(nlogn) in toate cazurile
*/
#include <stdio.h>
#include <string.h>
#include "Profiler.h"

#define MAX_SIZE 6000
#define STEP_SIZE 100
#define NR_TESTS 5
#define nr_test 100
#define max_size_heap 10000

Profiler p("HEAP");
Profiler p_time("SORT_TIMER");

void bubblesort_iterativ(int v[], int n, Operation* opComp, Operation* opAtr) {
	int schimbat;
	for (int i = 0; i < n - 1; i++) {
		schimbat = 0;
		for (int j = 0; j < n - i - 1; j++) {
			opComp->count();
			if (v[j] > v[j + 1]) {
				opAtr->count(3);
				int aux = v[j];
				v[j] = v[j + 1];
				v[j + 1] = aux;
				schimbat = 1;
			}
		}
		if (schimbat == 0) {
			return;
		}
	}
}

void bubblesort_recursiv(int v[], int n, Operation * opComp, Operation * opAtr) {
	if (n == 1) return;
	int schimbat = 0;
	for (int j = 0; j < n - 1; j++) {
		opComp->count();
		if (v[j] > v[j + 1]) {
			opAtr->count(3);
			int aux = v[j];
			v[j] = v[j + 1];
			v[j + 1] = aux;
			schimbat = 1;
		}
	}
	if (schimbat == 0) {
		return;
	}
	bubblesort_recursiv(v, n - 1, opComp, opAtr);
}




void heapify(int v[], int n, int i, Operation *op) {
	int largest = i;
	int left = 2 * i+1;
	int right = 2 * i + 2;
	
	if (left<n ) {
		if (v[left] > v[largest]) {
			op->count();
			largest = left;
		}
	}
	
	if (right<n ) {
		if (v[right] > v[largest]) {
			op->count();
			largest = right;
		}
	}
	if (largest != i) {
		op->count(3);
		int aux = v[i];
		v[i] = v[largest];
		v[largest] = aux;
		heapify(v, n, largest, op);
	}
}

void buildHeapBottomUp(int v[], int n, Operation * op) {
	int i;
	for ( i = n / 2 - 1; i >=0; i--) heapify(v, n, i, op);
}

void insertTopDown(int heap[], int* size, int val, Operation* op) {
	int i = (*size)++;
	heap[i] = val;
	op->count();
	while (i > 0) {
		int parent = (i - 1) / 2;
		op->count();
		if (heap[parent] < heap[i]) {
			op->count(3);
			int aux = heap[i];
			heap[i] = heap[parent];
			heap[parent] = aux;
			i = parent;
		}
		else break;
	}
}
void buildHeapTopDown(int v[], int n, Operation* op) {
	int heap[max_size_heap];
	int size = 0;
	for (int i = 0; i < n; i++) insertTopDown(heap, &size, v[i], op);
	for (int i = 0; i < n; i++) {
		//op->count();
		v[i] = heap[i];
	}
}

void heapsort(int v[], int n, Operation* op) {
	buildHeapBottomUp(v, n, op);
	for (int i = n-1; i >= 1; i--) {
		op->count(3);
		int aux = v[0];
		v[0] = v[i];
		v[i] = aux;

		heapify(v, i, 0, op);
	}
}

void demo_sort(Operation* opCompr, Operation* opAtrr, Operation* opCompi, Operation* opAtri) {
	int v[] = { 7, 9, 5, 9, 28, 3, 66, 10, 0, 99 };
	int b[] = { 2, 10, 5, 9, 39, 3, 66, 10, 120, 99 };
	int c[] = { 7, 9, 35, 19, 28, 38, 5, 10, 0, 400 };
	int d[] = { 11, 9, 35, 19, 28, 38, 5, 109, 88, 400 };


	int n = sizeof(v) / sizeof(int);
	bubblesort_iterativ(v, n, opCompi, opAtri);
	bubblesort_recursiv(b, n, opCompr, opAtrr);

	printf("sirul ordonat crescator dupa bubble sort iterativ este:");
	for (int i = 0; i < n; i++) printf("%d ", v[i]);
	printf("\n");

	printf("sirul ordonat crescator dupa bubble sort recursiv este:");
	for (int i = 0; i < n; i++) printf("%d ", b[i]);
	printf("\n");
}

void demo_heap(Operation* opBottomUp, Operation* opTopDown, Operation* opHeapSort) {
	int c[] = { 7, 9, 35, 19, 28, 38, 5, 10, 0, 400 };
	int d[] = { 11, 9, 35, 19, 28, 38, 5, 109, 88, 400 };
	int v[] = { 7, 9, 5, 9, 28, 3, 66, 10, 0, 99 };
	int n = sizeof(c) / sizeof(int);

	buildHeapBottomUp(c, n, opBottomUp);
	printf("heap construit bottomup:");
	for (int i = 0; i < n; i++) printf("%d ", c[i]);
	printf("\n");

	buildHeapTopDown(d, n, opTopDown);
	printf("heap construit topdown:");
	for (int i = 0; i < n; i++) printf("%d ", d[i]);
	printf("\n");

	heapsort(v, n, opHeapSort);
	printf("sirul sortat dupa heapsort:");
	for (int i = 0; i < n; i++) printf("%d ", v[i]);
	printf("\n");
}

void perf_average_heap() {
	p.reset("average_heap");
	int v[MAX_SIZE];
	int n;
	
	for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
		Operation opBottomUp = p.createOperation("BottomUp-heap", n);
		Operation opTopDown = p.createOperation("TopDown-heap", n);
		for (int j = 0; j < NR_TESTS; j++) {
			FillRandomArray(v, n);
			int cop[MAX_SIZE];
			for (int i = 0; i < n; i++) {
				cop[i] = v[i];
			}
			
			buildHeapBottomUp(v, n, &opBottomUp);
			buildHeapTopDown(cop, n, &opTopDown);
		}
	}

	p.divideValues("BottomUp-heap", NR_TESTS);
	p.divideValues("TopDown-heap", NR_TESTS);



	p.createGroup("BottomUpVsTopDown", "TopDown-heap", "BottomUp-heap");

	p.showReport();
}

void perf_worst_heap() {
	p.reset("worst_heap");
	int v[MAX_SIZE];
	int cop[MAX_SIZE];
	int n;

	for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
		Operation opBottomUp = p.createOperation("BottomUp-heap", n);
		Operation opTopDown = p.createOperation("TopDown-heap", n); 
		for (int j = 0; j < NR_TESTS; j++) {
			FillRandomArray(v, n, 10, 10000, false, 1);
			for (int i = 0; i < n; i++) {
				cop[i] = v[i];
			}
		
			buildHeapBottomUp(v, n, &opBottomUp);
			buildHeapTopDown(cop, n, &opTopDown);
		}
	}

	p.divideValues("BottomUp-heap", NR_TESTS);
	p.divideValues("TopDown-heap", NR_TESTS);



	p.createGroup("BottomUpVsTopDown", "TopDown-heap", "BottomUp-heap");

	p.showReport();
}



void perf_sort() {
	p.reset("sortari");
	int v[MAX_SIZE], cop[MAX_SIZE];
	int n;
	for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
		Operation opCompi = p.createOperation("BubbleSortIterativ-comp", n);
		Operation opAtri = p.createOperation("BubbleSortIterativ-atr", n);
		Operation opCompr = p.createOperation("BubbleSortRecursiv-comp", n);
		Operation opAtrr = p.createOperation("BubbleSortRecursiv-atr", n);
			FillRandomArray(v, n, 10, 10000, false, 0);
			for (int i = 0; i < n; i++) {
				cop[i] = v[i];
			}
				p_time.startTimer("bubble_sort_iter", n);
				for (int test = 1; test <=nr_test; test++) {
					memcpy(cop, v, n*sizeof(int));
					bubblesort_iterativ(cop, n, &opCompi, &opAtri);
				}
				p_time.stopTimer("bubble_sort_iter", n);

				p_time.startTimer("bubble_sort_recurs", n);
				for (int test = 1; test <= nr_test; test++) {
					memcpy(cop, v, n*sizeof(int));
					bubblesort_recursiv(cop, n, &opCompr, &opAtrr);
				}
				p_time.stopTimer("bubble_sort_recurs", n);
			}
		p.divideValues("BubbleSortIterativ-comp", nr_test);
		p.divideValues("BubbleSortIterativ-atr", nr_test);
		p.addSeries("bubbleIterativ", "BubbleSortIterativ-comp", "BubbleSortIterativ-atr");

		p.divideValues("BubbleSortRecursiv-comp", nr_test);
		p.divideValues("BubbleSortRecursiv-atr", nr_test);
		p.addSeries("bubbleRecursiv", "BubbleSortRecursiv-comp", "BubbleSortRecursiv-atr");

		p.createGroup("total", "bubbleIterativ", "bubbleRecursiv");
		p.showReport();
		p_time.showReport();
}


int main() {
	int n = MAX_SIZE;
	//Operation opCompr = p.createOperation("bbsortr-comp", n);
	//Operation opAtrr = p.createOperation("bbsortr-atr", n);
	//Operation opCompi = p.createOperation("bbsorti-comp", n);
	//Operation opAtri = p.createOperation("bbsorti-atr", n);
	//Operation opBottomUp=p.createOperation("BottomUp-heap", n);
	//Operation opTopDown=p.createOperation("TopDown-heap", n);
	//Operation opHeapSort = p.createOperation("HeapSort", n);
	//demo_sort(&opCompr, &opAtrr, &opCompi, &opAtri);
	//demo_heap(&opBottomUp, &opTopDown, &opHeapSort);
	//perf_average_heap();
	//perf_worst_heap();
	perf_sort();
	return 0;
}
