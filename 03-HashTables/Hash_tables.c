/*
* Gavris Alexandru Catalin
* Cautare in tabele de dispersie
*
* Am implementat urmatorii algoritmi: inserare, cautare si stergere
* intr-o tabela de dispersie folosind adresarea deschisa cu verificare patratica.
*
* Etapele implementarii:
* 1. Generarea a k liste sortate aleatoare de dimensiuni diferite, dar cu nr total fix de elemente n.
* 2. Adaptarea operatiilor de min-heap pentru a lucra direct pe vectorul de pointeri catre liste.
* 3. Interclasarea efectiva a listelor prin extragerea repetata a minimului din heap si actualizarea acestuia.
*
* Analiza:
* - Efortul mediu al cautarilor gasite creste odata cu alpha(factorul de umplere), deoarece coliziunile devin mai frecvente.
* - Efortul mediu al cautarilor negasite creste si mai rapid cu alpha, deoarece se parcurg mai multe sloturi pana la intalnirea unei pozitii goale.
* - Efortul maxim creste odata cu factorul de umplere.
*
* Tabele:
* -Valorile din tabele confirma teoretic comportamentul quadric probing.
* -Performanta scade pt valori ft mari ale factorului de umplere.
*
* Algoritmul este implementat corect si testat pe exemple mici in functia demo
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define empty -1
#define deleted -2
#define N 10007
#define M 3000

typedef struct {
	int id;
	char name[30];
}Entry;


int hash(int key, int size) {
	return key % size;
}

void init(Entry* hashTable, int size) {
	for (int i = 0; i < size; i++) hashTable[i].id = empty;
}

int insert(Entry* table, int size, int key, const char* name) {
	int i = 0;
	int poz;
	while (i < size) {
		poz = (hash(key, size) + i * i) % size;
		if (table[poz].id == empty || table[poz].id == deleted) {
			table[poz].id = key;
			if(name!=NULL)strcpy_s(table[poz].name, name);
			return 1;
		}
		i++;
	}
	return 0;
}

int search(Entry* table, int size, int key, int* effort) {
	int i = 0;
	int poz;
	*effort = 0;
	while (i < size) {
		poz = (hash(key, size) + i * i) % size;
		(*effort)++;
		if (table[poz].id == empty) {
			//printf("negasit\n");
			return -1;
		}
		if (table[poz].id == key) {
			//printf("%s \n", table[poz].name);
			return poz;
		}
		i++;
	}
	return 0;
}

int deleteH(Entry* table, int size, int key) {
	int i = 0;
	int poz;
	while (i < size) {
		poz = (hash(key, size) + i * i) % size;
		if (table[poz].id == empty) return 0;
		if (table[poz].id == key) {
			table[poz].id = deleted;
			table[poz].name[0] = '\0';
			return 1;
		}
		i++;
	}
	return 0;
}


void afisare(Entry* table, int size) {
	printf("Tabela hash este:\n");
	for (int i = 0; i < size; i++) {
		if (table[i].id != -1 && table[i].id != -2) {
			printf("[%d]-key: %d, Name: %s\n", i, table[i].id, table[i].name);
		}
		else printf("[%d]-gol\n", i);
	}
}

void demo(int *efforts) {
	Entry table[10];
	init(table, 10);
	insert(table, 10, 3, "Ana");
	insert(table, 10, 4, "Alex");
	insert(table, 10, 50, "Andi");
	insert(table, 10, 99, "Anca");

	afisare(table, 10);
	printf("\n");

	if (search(table, 10, 50, efforts) != -1) printf("%s \n", table[search(table, 10, 50, efforts)].name);
	else printf("negasit \n");

	if (search(table, 10, 3, efforts) != -1) printf("%s \n", table[search(table, 10, 3, efforts)].name);
	else printf("negasit \n");

	if (search(table, 10, 125, efforts) != -1) printf("%s \n", table[search(table, 10, 125, efforts)].name);
	else printf("negasit \n");
	printf("\n");

	search(table, 10, 3, efforts);
	search(table, 10, 125, efforts);

	deleteH(table, 10, 3);
	afisare(table, 10);
}

void amesteca(int* a, int n) {
	for (int i = n - 1; i > 0; i--) {
		int j = rand() % (i + 1);
		int tmp = a[i];
		a[i] = a[j];
		a[j] = tmp;
	}
}

void perf(double alpha) {
	Entry* table = (Entry*)malloc(N * sizeof(Entry));
	int fu = (int)(alpha * N);
	int* keys = (int*)malloc(fu * sizeof(int));
	int* cautari = (int*)malloc(M * sizeof(int));

	double avg_found = 0, avg_notfound = 0;
	int max_found_fin = 0, max_notfound_fin = 0;
	
	
	
	for (int r = 0; r < 5; r++) {
		init(table, N);
		for (int i = 0; i < fu; i++) {
			int key;
			int ok = 0;
			while (ok == 0) {
				key = rand() % (10 * N);
				ok = 1;
				for (int j = 0; j < i; j++) {
					if (keys[j] == key) {
						ok = 0;
						break;
					}
				}
			}
			keys[i] = key;
			char name[30];
			sprintf_s(name, "val%d", key);
			insert(table, N, key, name);
		}

		for (int i = 0; i < M / 2; i++) {
			cautari[i] = keys[rand() % fu];//cele gasite
		}

		for (int i = M / 2; i < M; i++) {
			cautari[i] = 10 * N + rand() % (10 * N);//cele negasite
		}

		amesteca(cautari, M);
		int total_ef_found = 0, max_found=0;
		int total_ef_notfound = 0, max_notfound = 0;

		for (int i = 0; i < M; i++) {
			int efort=0;
			int found=search(table, N, cautari[i], &efort);
			if (found != -1) {
				total_ef_found += efort;
				if (efort > max_found) max_found = efort;
			}
			else {
				total_ef_notfound += efort;
				if (max_notfound < efort) max_notfound = efort;
			}
		}
		 avg_found += double(total_ef_found) / (M / 2);
		 avg_notfound += double(total_ef_notfound) / (M / 2);
		 if (max_found_fin < max_found) {
			 max_found_fin = max_found;
		 }
		 if (max_notfound_fin < max_notfound) {
			 max_notfound_fin = max_notfound;
		 }
	}
	printf("%-14.2f | %-18.2f | %-17d | %-15.2f         | %-17d", alpha, avg_found / 5, max_found_fin, avg_notfound / 5, max_notfound_fin);

	free(table);
	free(keys);
	free(cautari);
}

void perfDelete(double alphaInsert, double alphaDelete) {
	Entry* table = (Entry*)malloc(N * sizeof(Entry));
	int fuInsert = (int)(alphaInsert * N);
	int fuDelete = (int)(alphaDelete * N);
	int* keys = (int*)malloc(fuInsert * sizeof(int));
	int* cautari = (int*)malloc(M * sizeof(int));

	init(table, N);

	for (int i = 0; i < fuInsert; i++) {
		int key;
		int ok = 0;
		while (ok == 0) {
			key = rand() % (10 * N);
			ok = 1;
			for (int j = 0; j < i; j++) {
				if (keys[j] == key) {
					ok = 0;
					break;
				}
			}
		}
		keys[i] = key;
		char name[30];
		sprintf_s(name, "val%d", key);
		insert(table, N, key, name);
	}

	int toDelete = fuInsert - fuDelete;
	for (int i = 0; i < toDelete; i++) {
		deleteH(table, N, keys[i]);
	}
	for (int i = 0; i < M / 2; i++) {
		cautari[i] = keys[toDelete+rand() % fuDelete];//cele gasite
	}

	for (int i = M / 2; i < M; i++) {
		cautari[i] = 10 * N + rand() % (10 * N);//cele negasite
	}
	amesteca(cautari, M);
	int total_ef_found = 0, max_found = 0;
	int total_ef_notfound = 0, max_notfound = 0;

	for (int i = 0; i < M; i++) {
		int efort = 0;
		int found = search(table, N, cautari[i], &efort);
		if (found !=- 1) {
			total_ef_found += efort;
			if (efort > max_found) max_found = efort;
		}
		else {
			total_ef_notfound += efort;
			if (max_notfound < efort) max_notfound = efort;
		}
	}
	printf("%-14.2f | %-18.2f | %-17d | %-15.2f         | %-17d", alphaDelete, (double)total_ef_found/(M/2), max_found, (double)total_ef_notfound/(M/2), max_notfound);

	free(table);
	free(keys);
	free(cautari);

}

void afisareTabel(double factori[], int n) {
	printf("Filling factor | Avg. Effort(found) | Max Effort(found) |  Avg. Effort(not-found) | Max Effort(not-found) \n");
	printf("---------------------------------------------------------------------------------------------------------\n");
	for (int i = 0; i < n; i++) {
		perf(factori[i]);
		printf("\n");
	}
	printf("\n");
}

void afisareTabelDelete(double alphaInsert, double alphaDelete) {
	printf("Filling factor | Avg. Effort(found) | Max Effort(found) |  Avg. Effort(not-found) | Max Effort(not-found) \n");
	printf("---------------------------------------------------------------------------------------------------------\n");
	perfDelete(alphaInsert, alphaDelete);
	printf("\n");
	
}


int main() {
	//int* efforts=(int*)malloc(n*sizeof(int));
	int efforts=0;
	//demo(&efforts);
	srand((unsigned int)time(NULL));
	
	double alphaInsert = 0.99;
	double alphaDelete = 0.80;
	double factori1[] = { 0.95 };
	double factori2[] = { 0.8, 0.85, 0.9, 0.95, 0.99 };
	int size1 = sizeof(factori1) / sizeof(double);
	int size2 = sizeof(factori2) / sizeof(double);
	//afisareTabel(factori1, 1);
	//afisareTabel(factori2, size2);
	afisareTabelDelete(alphaInsert, alphaDelete);
	

	return 0;
}
