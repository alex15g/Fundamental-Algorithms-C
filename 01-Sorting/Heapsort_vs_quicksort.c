/*
*Gavris Alexandru Catalin 
* 
*Am implementat urmatorii algoritmi: 
*1.QuickSort clasic(pivot ultimul element), 
*2.QuickSort cu pivot mijlociu, 
*3.QuickSort hibridizat cu InsertionSort
*4.HeapSort(bottomUp)
*5.Randomized select(dtermina alk-lea element)
*
*Am efectuat analiza pentru urmatoarele cazuri:
* -QuickSort:best case (cu pivot mijlociu+sir crescator), worst case(cu pivot ultimul elemnt + sir sortat) si average case(cu pivot ultimul elemnt)
* -Comparatie QuickSort si HeapSort
* -Comparatie QuickSort si QuickSort hibridizat
* 
*Complexitati:
* -QuickSort: average+best->O(nlogn), worst->O(n^2)
* -QuickSort hibridizat: la fel,dar performanta e imbunatatita datorita insertionSort
* -HeapSort: O(nlogn)
*
*INTERPRETARE GRAFICE:
* -QuickSort hibridizat devine mai rapid la dimensiuni mari(timp redus)
* -HeapSort mai lent decat QuickSort in medie
* -Pragul optim pt hibridizare e intre 20-40 elemente
* 
* Cei 5 algoritmi sunt implementati corecti si testati pe exemple mici in functia demo
*/
#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 5
#define PRAG 30
#define nr_test 100

Profiler p("Analiza");
Profiler p_time("Quick_TIMER");

int part(int a[], int l, int r, Operation* opComp, Operation* opAtr) {
	int piv = a[r];
	int j = l - 1;
	for (int i = l; i < r; i++) {
		opComp->count();
		if (a[i] < piv) {
			j++;
			opAtr->count(3);
			int aux = a[i];
			a[i] = a[j];
			a[j] = aux;
		}
	}
	opAtr->count(3);
	int aux = a[j + 1];
	a[j + 1] = a[r];
	a[r] = aux;
	return j+1;
}

void quickSort(int a[], int l, int r, Operation* opComp, Operation* opAtr) {
	if (l < r) {
		int piv = part(a, l, r, opComp, opAtr);
		quickSort(a, l, piv - 1, opComp, opAtr);
		quickSort(a, piv + 1, r, opComp, opAtr);
	}
}

int partMiddlePiv(int a[], int l, int r, Operation* opComp, Operation* opAtr) {
	int mid = (l + r) / 2;
	int tmp = a[mid];
	a[mid] = a[r];
	a[r] = tmp;
	opAtr->count(3);
	
	int piv = a[r];
	int j = l - 1;
	for (int i = l; i < r; i++) {
		opComp->count();
		if (a[i] < piv) {
			j++;
			opAtr->count(3);
			int aux = a[i];
			a[i] = a[j];
			a[j] = aux;
		}
	}
	opAtr->count(3);
	int aux = a[j + 1];
	a[j + 1] = a[r];
	a[r] = aux;
	return j + 1;
}

void quickSortMiddlePiv(int a[], int l, int r, Operation* opComp, Operation* opAtr) {
	if (l < r) {
		int piv = partMiddlePiv(a, l, r, opComp, opAtr);
		quickSortMiddlePiv(a, l, piv - 1, opComp, opAtr);
		quickSortMiddlePiv(a, piv + 1, r, opComp, opAtr);
	}
}
void insertionsort(int v[], int n, Operation* opComp, Operation* opAtr) {
	for (int i = 0; i < n; i++) {
		opAtr->count();
		int key = v[i];
		int j = i - 1;
		while (j >= 0) {
			opComp->count();
			if (v[j] > key) {
				v[j + 1] = v[j];
				opAtr->count();
				j--;
			}
			else break;
		}
		opAtr->count();
		v[j + 1] = key;
	}
}

void quickSortHibrid(int a[], int l, int r, int prag, Operation* opComp, Operation* opAtr) {
	if (l < r) {
		if (r - l + 1 < prag) insertionsort(a + l, r - l + 1, opComp, opAtr);

		else {
			int piv = part(a, l, r, opComp, opAtr);
			quickSortHibrid(a, l, piv - 1,prag, opComp, opAtr);
			quickSortHibrid(a, piv + 1, r,prag, opComp, opAtr);
		}
	}

}

void heapify(int v[], int n, int i, Operation* op) {
	int largest = i;
	int left = 2 * i + 1;
	int right = 2 * i + 2;

	if (left < n) {
		if (v[left] > v[largest]) {
			op->count();
			largest = left;
		}
	}

	if (right < n) {
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

void buildHeapBottomUp(int v[], int n, Operation* op) {
	int i;
	for (i = n / 2 - 1; i >= 0; i--) heapify(v, n, i, op);
}

void heapsort(int v[], int n, Operation* op) {
	buildHeapBottomUp(v, n, op);
	for (int i = n - 1; i >= 1; i--) {
		op->count(3);
		int aux = v[0];
		v[0] = v[i];
		v[i] = aux;

		heapify(v, i, 0, op);
	}
}

int randomPart(int a[], int l, int r, Operation* opComp, Operation* opAtr) {
	int pivind = l + rand() % (r - l + 1); 
	opAtr->count(3);
	int aux = a[pivind];
	a[pivind] = a[r];
	a[r] = aux;

	return part(a, l, r, opComp, opAtr);
}

int randomizedSelect(int a[], int l, int r, int k, Operation* opComp, Operation* opAtr) {
	if (l == r) return a[l]; 

	int piv = randomPart(a, l, r, opComp, opAtr);
	int rank = piv - l + 1;  

	if (rank == k) return a[piv];       
	else if (k < rank)                 
		return randomizedSelect(a, l, piv - 1, k, opComp, opAtr);
	else                                
		return randomizedSelect(a, piv + 1, r, k - rank, opComp, opAtr);
}

void demo(Operation* opComp, Operation* opAtr) {
	int a[] = { 11, 9, 35, 19, 28, 38, 5, 109, 88, 5, 400 };
	int b[] = { 2, 10, 5, 9, 39, 3, 66, 10, 120, 99, 3 };
	int c[] = { 7, 9, 35, 19, 28, 38, 5, 10, 0, 400, 55 };
	int k = 3;

	int n = sizeof(a) / sizeof(int);
	quickSort(a, 0, n - 1, opComp, opAtr);
	quickSortHibrid(b, 0, n - 1, 30, opComp, opAtr);
	int rez=randomizedSelect(c, 0, n - 1, k, opComp, opAtr);

	printf("sirul ordonat crescator dupa quickSort e: ");
	for (int i = 0; i < n; i++)printf("%d ", a[i]);
	printf("\n");

	printf("sirul ordonat crescator dupa quickSortHibrid e: ");
	for (int i = 0; i < n; i++)printf("%d ", b[i]);
	printf("\n");

	printf("Al %d-lea element este: %d", k, rez);
	printf("\n");
}

void perf_average_qs_vs_hs() {
	p.reset("qS_vs_hS");
	int v[MAX_SIZE];
	int n;

	for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
		Operation opq = p.createOperation("qs-total", n);
		Operation op = p.createOperation("hs-total", n);
		for (int j = 0; j < NR_TESTS; j++) {
			FillRandomArray(v, n);
			int cop[MAX_SIZE];
			for (int i = 0; i < n; i++) {
				cop[i] = v[i];
			}

			quickSort(v, 0, n-1, &opq, &opq);

			heapsort(cop, n, &op);
		}
	}
	p.divideValues("qs-total", NR_TESTS);
	p.divideValues("hs-total", NR_TESTS);


	p.createGroup("QS-vs-HS", "qs-total", "hs-total");
	p.showReport();

}

void perf_best_qS() {
	p.reset("best_qS");
	int v[MAX_SIZE];
	int n;
	for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
		Operation opComp = p.createOperation("qs-comp", n);
		Operation opAtr = p.createOperation("qs-atr", n);
		for (int j = 0; j < NR_TESTS; j++) {
			FillRandomArray(v, n, 10, 10000, false, 1);

			quickSortMiddlePiv(v, 0, n - 1, &opComp, &opAtr);
		}
	}
	p.divideValues("qs-comp", NR_TESTS);
	p.divideValues("qs-atr", NR_TESTS);

	p.addSeries("qs-total", "qs-comp", "qs-atr");

	p.showReport();

}

void perf_worst_qS() {
	p.reset("worst_qS");
	int v[MAX_SIZE];
	int n;
	for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
		Operation opComp = p.createOperation("qs-comp", n);
		Operation opAtr = p.createOperation("qs-atr", n);
		for (int j = 0; j < NR_TESTS; j++) {
			FillRandomArray(v, n, 10, 10000, false, 1);

			quickSort(v, 0, n - 1, &opComp, &opAtr);
		}
	}
	p.divideValues("qs-comp", NR_TESTS);
	p.divideValues("qs-atr", NR_TESTS);

	p.addSeries("qs-total", "qs-comp", "qs-atr");

	p.showReport();
}

void perf_average_qS() {
	p.reset("average_qS");
	int v[MAX_SIZE];
	int n;
	for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
		Operation opComp = p.createOperation("qs-comp", n);
		Operation opAtr = p.createOperation("qs-atr", n);
		for (int j = 0; j < NR_TESTS; j++) {
			FillRandomArray(v, n);

			quickSort(v, 0, n - 1, &opComp, &opAtr);
		}
	}
	p.divideValues("qs-comp", NR_TESTS);
	p.divideValues("qs-atr", NR_TESTS);

	p.addSeries("qs-total", "qs-comp", "qs-atr");

	p.showReport();

}

void perf_quickS_vs_quickShibr() {
	p.reset("quicksort-vs-quickSortHibridizat");
	int v[MAX_SIZE], cop[MAX_SIZE];
	int n;
	for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
		Operation opComp = p.createOperation("QuickSort-comp", n);
		Operation opAtr = p.createOperation("QuickSort-atr", n);
		Operation opComph = p.createOperation("QuickSortHibridizat-comp", n);
		Operation opAtrh = p.createOperation("QuickSortHibridizat-atr", n);
		FillRandomArray(v, n, 10, 50000, false, 0);
		for (int i = 0; i < n; i++) {
			cop[i] = v[i];
		}
		p_time.startTimer("quickSort", n);
		for (int test = 1; test <= nr_test; test++) {
			memcpy(cop, v, n * sizeof(int));
			quickSort(cop, 0, n-1, &opComp, &opAtr);
		}
		p_time.stopTimer("quickSort", n);

		p_time.startTimer("quickSortHibridizat", n);
		for (int test = 1; test <= nr_test; test++) {
			memcpy(cop, v, n * sizeof(int));
			quickSortHibrid(cop, 0, n-1, 30, &opComph, &opAtrh);
		}
		p_time.stopTimer("quickSortHibridizat", n);
	}
	p.divideValues("QuickSort-comp", nr_test);
	p.divideValues("QuickSort-atr", nr_test);
	p.addSeries("QuickSort", "QuickSort-comp", "QuickSort-atr");

	p.divideValues("QuickSortHibridizat-comp", nr_test);
	p.divideValues("QuickSortHibridizat-atr", nr_test);
	p.addSeries("QuickSortHibridizat", "QuickSortHibridizat-comp", "QuickSortHibridizat-atr");

	p.createGroup("total", "QuickSort", "QuickSortHibridizat");
	p_time.createGroup("timer", "quickSort", "quickSortHibridizat");
	p.showReport();
	p_time.showReport();
}

void perf_prag_optim_combinat() {
	p.reset("determinare_prag_optim");
	int v[MAX_SIZE], cop[MAX_SIZE];
	int n;
	for (n = 5; n <= 50; n += 5) {
		Operation opComph = p.createOperation("QuickSortHibridizat-comp", n);
		Operation opAtrh = p.createOperation("QuickSortHibridizat-atr", n);
		FillRandomArray(v, 10000, 10, 50000, false, 0);
		for (int i = 0; i < 10000; i++) {
			cop[i] = v[i];
		}

		p_time.startTimer("quickSortHibridizat", n);
		for (int test = 1; test <= nr_test; test++) {
			memcpy(cop, v, 10000 * sizeof(int));
			quickSortHibrid(cop, 0, 10000 - 1, n, &opComph, &opAtrh);
		}
		p_time.stopTimer("quickSortHibridizat", n);
	}
	p.divideValues("QuickSortHibridizat-comp", nr_test);
	p.divideValues("QuickSortHibridizat-atr", nr_test);
	p.addSeries("QuickSortHibridizat", "QuickSortHibridizat-comp", "QuickSortHibridizat-atr");

	p.showReport();
	p_time.showReport();
}

int main() {
	int n = MAX_SIZE;
	//Operation opComp = p.createOperation("qs-comp", n);
	//Operation opAtr = p.createOperation("qs-atr", n);
	//demo(&opComp, &opAtr);
	//perf_average_qS();
	//perf_worst_qS();
	//perf_best_qS();
	//perf_average_qs_vs_hs();
	//perf_quickS_vs_quickShibr();
	perf_prag_optim_combinat();
	return 0;
}
