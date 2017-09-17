#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct unit { 
	int* gene;
	int fitness;
} unit;

typedef struct item { 
	int cost;
	int volume;
} item;

typedef struct pop {
	unit* arr;
	int size;
} pop;

typedef struct shop {
	int backpack_volume;
	int item_num;
	item* items;
} shop;

void randomize (unit* unit1, shop* pater);

void mutation (unit* unit1, shop* pater);

unit crossover (unit* unit1, unit* unit2, shop* pater);

int comp_units (unit* u1, unit* u2) {
	return u2->fitness - u1->fitness;
}

unit* unit_memory (int a) {
	unit *unit1 = malloc (sizeof(unit)); 
	unit1 -> gene = malloc (sizeof(int) * a);
	return unit1;
}

item item_memory (int a) {
	item *item1 = malloc (sizeof(item));
	return *item1;
}

 pop* pop_memory (int a) {
	pop *pop1 = malloc(sizeof(pop));
	pop1 -> arr = malloc (sizeof(unit) * a);
	pop1->size = a;
	return pop1;
}

shop* shop_memory (int a) {
	shop *shop1 = malloc(sizeof(shop));
	shop1->item_num = a;
	shop1 -> items = malloc (sizeof(item) * a);
	return shop1;
}

void shop_free (shop* shop1) {
	free (shop1->items);
	free (shop1);	
}

void pop_free (pop* pop1) {
	int i;
	for(i = 0; i < pop1->size;i++) {
		free(pop1->arr[i].gene);
	}
	free (pop1->arr);
	free (pop1);	
}

void next_generation (pop* cels, shop* pater) {
	int i;
	int g = cels->size - cels->size / 3;
	for (i = g; i < cels->size; i++) {
		free(cels->arr[i].gene);
		cels->arr[i] = crossover(&(cels->arr[rand()%g]), &(cels->arr[rand()%g]), pater);
	}
	for (i = cels->size / 3; i < cels->size; i++) {
		if (rand()%10 == 0) {
			mutation(&(cels->arr[i]), pater);
		}
	}	
	for (i = 0; i < cels->size; i++) {
		cels->arr[i].fitness = fitness (cels->arr[i], pater);	
	}
	qsort (cels->arr,cels->size,sizeof(unit), (int (*) (const void*, const void*)) comp_units);
}

int fitness (unit unit1, shop* shop1) {
	int fitness = 0;
	int a = 0;
	while (a < shop1->item_num)  {
		if (unit1.gene[a] == 1 ) {
			fitness = fitness + shop1->items[a].cost;			
		}
		a++;
	}
	return fitness;
}

void fix (unit unit1, shop* shop1 ) {
	int volume = 0;
	int a = 0;
	while (a < shop1->item_num)  {
		if (unit1.gene[a] == 1 ) {
			volume = volume + shop1->items[a].volume;
		}
		a++;			
	}
	a = 0;
	while (1)  {
		if (volume <= shop1->backpack_volume) {
			break;			
		}
		else {
			a = rand()%shop1 -> item_num;
		
			if (unit1.gene[a] == 1) {
				unit1.gene[a] = 0;
				volume = volume - shop1->items[a].volume;
			}
		}
	}
}
void  your_backpack (unit* it) {
	int i;
	for (i = 0; i < 5; i++) {
		printf("%d\n", it->gene[i]);
	}
}

unit crossover (unit* unit1, unit* unit2, shop* pater) {
	int g;
	int i = rand()%pater->item_num;
	unit potomok; 
	potomok.gene = malloc (sizeof (int) * pater->item_num);
	for (g = 0; g < i; g++) {
		potomok.gene[g] = unit1->gene[g];
	}
	for (g = i; g < pater->item_num; g++) {
		potomok.gene[g] = unit2->gene[g];
	}
	fix(potomok, pater);
	return potomok;
}

void mutation (unit* unit1, shop* pater) {
	int a;
	a = rand()%pater -> item_num; 
	if (unit1->gene[a] == 1) {
		unit1->gene[a] = 0;
	}
	else {
		unit1->gene[a] = 1;
	}
	fix(*unit1, pater);
}

void randomize (unit* unit1, shop* pater) {
	int a;
	int g;
	for (g = 0; g < pater->item_num; g++) {
		a = rand()%2;
		unit1->gene[g] = a;
	}
	fix(*unit1, pater);
}

void print_unit (pop* pop1, shop* shop1, int a) {
	int i;
	printf("               __BACKPACK__\n");
	for (i = 0; i < shop1->item_num; i++) {
		if (pop1->arr[a].gene[i] == 1) {
			printf("%d ", i + 1);
			printf("ey/oy, predmet\t");
			printf("value = %d\t", shop1->items[i].cost);
			printf("volume = %d\n", shop1->items[i].volume);
		}
	}
}
void print_items (shop* shop1) {
	int i;
	printf("               __SHOP__\n");
	for (i = 0; i < shop1->item_num; i++) {
		printf("%d ", i + 1);
		printf("ey/oy, predmet\t");
		printf("value = %d\t", shop1->items[i].cost);
		printf("volume = %d\n", shop1->items[i].volume);
	}
}

int main () {
	srand(time(0));
	
	int item_num = 10;
	int unit_num = 1000;
	int i;
	shop* pater = shop_memory(item_num);
	pater->backpack_volume = 100;
	pop* cels = pop_memory(unit_num);
	for (i = 0; i < pater->item_num; i++) {
		pater->items[i].cost = rand()%32 + 1;
		pater->items[i].volume = rand()%20 + 1;
	}
	for (i = 0; i < cels->size; i++) {
		cels->arr[i].gene = malloc (sizeof(int) * pater->item_num);
		randomize(&(cels->arr[i]), pater); 
		cels->arr[i].fitness = fitness (cels->arr[i], pater);	
	}
	qsort (cels->arr,cels->size,sizeof(unit), (int (*) (const void*, const void*)) comp_units);
	for (i = 1; i < 1001; i++) {
		printf("New pokolenia:\n");
		printf("%d", i);
		next_generation(cels, pater);
		printf("...");
		printf ("fitness best cel:\n");
		printf("%d\n", cels->arr[0].fitness);
//		char c;
//		scanf("%c", &c);
	}
	print_items(pater);
	print_unit(cels, pater, 0);
	shop_free(pater);
	pop_free(cels);
	return 0;
}
